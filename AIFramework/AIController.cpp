#include "AIController.h"



AIController::AIController(std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path, float pathWidth) : m_vecVehicles(vecVehicles), m_vecPickUps(vecPickUps)
{
	m_pathWidth = 300.0f;
	m_path = SplineCurve(path);
}

void AIController::OnArrive(Vector2D point)
{
}

void AIController::Control(Vehicle* vehicle, const float deltaTime)
{
	Vector2D position = vehicle->getVehiclePosition();
	Vector2D predicted = vehicle->getPredictedPosition(deltaTime);
	Vector2D pathPoint = m_path.GetNearestLinePoint(predicted);

	float distance = (predicted - pathPoint).Length();

	if (distance > m_pathWidth)
	{
		vehicle->setPositionTo(pathPoint,true);
	}
}
