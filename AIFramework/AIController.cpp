#include "AIController.h"



AIController::AIController(std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path, std::vector<Vector2D>& checkPoints, float pathWidth)
	: m_vecVehicles(vecVehicles), m_vecPickUps(vecPickUps), m_vecCheckPoints(checkPoints)
{
	m_pathWidth = 40.0f;
	m_path = SplineCurve(path);
}

void AIController::OnArrive(Vector2D point)
{
	
}

void AIController::Control(Vehicle* vehicle, const float deltaTime)
{
	vehicle->StopMovingTowardsPoint();

	int startNode = 0;
	int endNode = 0;

	Vector2D position = vehicle->getVehiclePosition();
	Vector2D predicted = vehicle->getPredictedPosition(deltaTime);
	Vector2D pathPoint = m_path.GetNearestLinePoint(predicted,startNode,endNode);

	Vector2D nextCheckPoint = m_vecCheckPoints[m_checkPointIndex];

	float distanceToRoad = (predicted - pathPoint).Length();
	float distanceToCheckPoint = (nextCheckPoint - position).Length();

	if (distanceToRoad > m_pathWidth)
	{
		if (!m_movingOffTrack) 
		{
			m_movingOffTrack = true;
			m_pathResume = pathPoint;
		}

		vehicle->RotateTowards(deltaTime, m_pathResume);
		
	}
	else 
	{
		m_movingOffTrack = false;
		vehicle->RotateTowards(deltaTime, nextCheckPoint);
	}
	
	vehicle->Accelerate(deltaTime);

	if (distanceToCheckPoint < 9.0f)
	{
		m_checkPointIndex++;
		if (m_checkPointIndex >= m_vecCheckPoints.size())
			m_checkPointIndex = 0;
	}
	
}
