#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"
#include "AStar.h"

#include "main.h"
#include "AIController.h"

void AIManager::Release()
{
    for (auto p : m_cars)
        delete p;
    for (auto p : m_waypoints)
        delete p;
    for (auto p : m_pickups)
        delete p;
    delete m_AStar;
}

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{

    HRESULT hr;
    // create pickup items ----------------------------------------------

    for (unsigned int i = 0; i < 10; i++)
    {

        PickupItem* pPickup = new PickupItem();
        hr = pPickup->initMesh(pd3dDevice);
        m_pickups.push_back(pPickup);
    }

    // create the vehicle ------------------------------------------------

    float xPos = 0;
    float yPos = 200;

    int numCars = 10;

    for (unsigned int i = 0; i < numCars; i++) 
    {
        Vehicle* car = new Vehicle();
        hr = car->initMesh(pd3dDevice);
        car->setVehiclePosition(Vector2D(xPos + i * 40, yPos));

        if (FAILED(hr))
            return hr;

        m_cars.push_back(car);

    }

    // create the waypoints ----------------------------------------------

    
    float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
    float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
    float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
    float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);

    unsigned int index = 0;
    for (unsigned int y = 0; y < WAYPOINT_RESOLUTION; y++) {
        for (unsigned int x = 0; x < WAYPOINT_RESOLUTION; x++) {
            Waypoint* wp = new Waypoint();
            hr = wp->initMesh(pd3dDevice, index++);
            wp->setPosition(XMFLOAT3(xStart + (xGap * x), yStart + (yGap * y), 0));
            m_waypoints.push_back(wp);
        }
    }

    // create A star Grid ------------------------------------------------


    int weights[COLLUMS * ROWS];
    
    for (unsigned int i = 0; i < m_waypoints.size(); i++) 
    {
        weights[i] = m_waypoints[i]->isOnTrack() ? 10 : -1;

        
        //add check point to array of checkpoints.
        if (m_waypoints[i]->isCheckPoint())
        {
            int checkPointIndex = m_waypoints[i]->getCheckpointIndex();

            while (checkPointIndex >= m_checkPoints.size()) 
            {
                m_checkPoints.push_back(0);
                m_checkPointCoords.push_back(Vector2D(0, 0));
            }

            m_checkPoints[checkPointIndex] = i;
            XMFLOAT3* pos = m_waypoints[i]->getPosition();
            m_checkPointCoords[checkPointIndex] = Vector2D(pos->x, pos->y);
        }
    }

    //go through each pickup and set it to a random position.
    for (auto pickup : m_pickups)
    {
        int index = rand() % m_waypoints.size();
        XMFLOAT3* pos = m_waypoints[index]->getPosition();
        pickup->setPosition(XMFLOAT3(pos->x,pos->y,pickup->getPosition()->z));
        weights[index] = 3;
    }

    //Pathfind from checkpoint to checkpoint.
    m_AStar = new GridAStar();
    m_AStar->Init(weights);

    for (int i = 0; i < m_checkPoints.size(); i++)
    {
        int indexNext = (i + 1) < m_checkPoints.size() ? (i+1) : 0;
        std::vector<int> path = m_AStar->PathFind(m_checkPoints[i],m_checkPoints[indexNext],false);

        for (auto p : path)
        {
            m_path.push_back(p);
        }
    }

    for (auto p : m_path)
    {
        XMFLOAT3* pos = m_waypoints[p]->getPosition();
        m_pathPointCoords.push_back(Vector2D(pos->x,pos->y));
    }


    //add controlllers to the cars using the path finding stuff.
    for (auto c : m_cars)
    {
        c->setController(new AICarBehaviourTree(c, m_cars, m_pickups, m_pathPointCoords, m_checkPointCoords, 100));
    }

    return hr;
}

void AIManager::update(const float fDeltaTime)
{
    for (unsigned int i = 0; i < m_waypoints.size(); i++) {
        m_waypoints[i]->update(fDeltaTime);// if you comment this in, it will display the waypoints
        AddItemToDrawList(m_waypoints[i]);
    }

    for (unsigned int i = 0; i < m_pickups.size(); i++) {
        m_pickups[i]->update(fDeltaTime);
        AddItemToDrawList(m_pickups[i]);
    }

    for (unsigned int i = 0; i < m_cars.size(); i++) 
    {
        m_cars[i]->update(fDeltaTime);
        AddItemToDrawList(m_cars[i]);
    }
    
    /*
    //Test draw turn circle.
    Vector2D c = m_cars[0]->GetTurnCircle().center;
    XMFLOAT3 circleCenter = XMFLOAT3(c.x, c.y, 0);
    m_waypoints[0]->setPosition(circleCenter);
    AddItemToDrawList(m_waypoints[0]);
    */

    checkForCollisions();
}

void AIManager::mouseUp(int x, int y)
{
    int carMoved = rand() % m_cars.size();
    //m_cars[carMoved]->MoveTowards(,Vector2D(x, y),true);
}

void AIManager::keyPress(WPARAM param)
{
    switch (param)
    {
        case VK_NUMPAD0:
        {
            OutputDebugStringA("0 pressed \n");

            break;
        }
        case VK_NUMPAD1:
        {
            OutputDebugStringA("1 pressed \n");
            break;
        }
        case VK_NUMPAD2:
        {
            break;
        }
        case VK_SPACE:
        {
            OutputDebugStringA("space pressed \n");
            m_checkPointIndex++;
            if (m_checkPointIndex >= m_checkPoints.size())
                m_checkPointIndex = 0;
            break;
        }

        // etc
        default:
            break;
    }
}

bool AIManager::checkForCollisions()
{
    if (m_pickups.size() == 0)
        return false;

    XMVECTOR dummy;

    for (auto car : m_cars)
    {
        // the car
        XMVECTOR carPos;
        XMVECTOR carScale;
        XMMatrixDecompose(
            &carScale,
            &dummy,
            &carPos,
            XMLoadFloat4x4(m_cars[0]->getTransform())
        );

        XMFLOAT3 scale;
        XMStoreFloat3(&scale, carScale);
        BoundingSphere boundingSphereCar;
        XMStoreFloat3(&boundingSphereCar.Center, carPos);
        boundingSphereCar.Radius = scale.x;


        for (auto pickup : m_pickups)
        {
            XMVECTOR puPos;
            XMVECTOR puScale;
            XMMatrixDecompose(
                &puScale,
                &dummy,
                &puPos,
                XMLoadFloat4x4(m_pickups[0]->getTransform())
            );

            XMStoreFloat3(&scale, puScale);
            BoundingSphere boundingSpherePU;
            XMStoreFloat3(&boundingSpherePU.Center, puPos);
            boundingSpherePU.Radius = scale.x;

            // test
            if (boundingSphereCar.Intersects(boundingSpherePU))
            {
                OutputDebugStringA("Collision!\n");
            }

        }
    }

    return false;
}



