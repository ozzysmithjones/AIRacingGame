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

    // create a pickup item ----------------------------------------------

    PickupItem* pPickup = new PickupItem();
    HRESULT hr = pPickup->initMesh(pd3dDevice);
    m_pickups.push_back(pPickup);


    // create the vehicle ------------------------------------------------

    float xPos = 0;
    float yPos = 500;

    int numCars = 3;

    for (unsigned int i = 0; i < numCars; i++) 
    {
        Vehicle* car = new Vehicle();
        hr = car->initMesh(pd3dDevice);
        car->setVehiclePosition(Vector2D(xPos + i, yPos));

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
        weights[i] = m_waypoints[i]->isOnTrack() ? 1 : 1000;

        
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

    std::vector<Vector2D> pathPointCoords;

    for (auto p : m_path)
    {
        XMFLOAT3* pos = m_waypoints[p]->getPosition();
        pathPointCoords.push_back(Vector2D(pos->x,pos->y));
    }

    m_cars[0]->setController(new AIController(m_cars, m_pickups, pathPointCoords,m_checkPointCoords, 100));
    return hr;
}

void AIManager::update(const float fDeltaTime)
{
    
    for (unsigned int i = 0; i < m_waypoints.size(); i++) {
        m_waypoints[i]->update(fDeltaTime);
       // AddItemToDrawList(m_waypoints[i]); // if you comment this in, it will display the waypoints
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
    m_cars[carMoved]->MoveTowardsPoint(Vector2D(x, y),true);
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

    // a pickup - !! NOTE it is only referring the first one in the list !!
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
        return true;
    }

    return false;
}



