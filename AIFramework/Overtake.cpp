#include "Overtake.h"

void Overtake::Reset()
{
}

void Overtake::Start()
{
	
}

BehaviourState Overtake::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState::RUNNING;
}

BehaviourState Overtake::Update(const float deltaTime, Behaviour*& childToPush)
{
	m_vehicleToOvertake = FindVehicleToOvertake();

	int checkPoint = m_vehicle->getNextCheckPointIndex();
	int previousCheckPoint = checkPoint > 0 ? checkPoint - 1 : m_splinePath.size() - 1;
	Vector2D position = m_vehicle->getVehiclePosition();
	Vector2D roadDirection = m_splinePath[checkPoint] - m_splinePath[previousCheckPoint];

	if (m_vehicleToOvertake != nullptr)
	{
		Vector2D otherPosition = m_vehicleToOvertake->getVehiclePosition();
		Vector2D otherDirection = m_vehicleToOvertake->getVehicleDirection();
		Vector2D toOther = otherPosition - position;

		if (toOther.Dot(roadDirection) > 0.0f)
		{
			Vector2D roadCenter = m_splinePath.GetNearestLinePoint(otherPosition);
			Vector2D toRoadCenter = otherPosition - roadCenter;
			toRoadCenter.Normalize();

			Vector2D overtakePoint = otherPosition - toRoadCenter * 40.0f;

			if (overtakePoint.DistanceSq(roadCenter) > (40.0f * 40.0f)) 
			{
				return BehaviourState::FAIL;
			}

			m_vehicle->MoveTowards(deltaTime, overtakePoint, false,false);
			return BehaviourState::SUCCESS;
		}
	}

	return BehaviourState::FAIL;
}

Vehicle* Overtake::FindVehicleToOvertake()
{
	Vector2D position = m_vehicle->getVehiclePosition();
	int checkPoint = m_vehicle->getNextCheckPointIndex();

	for (auto vehicle : m_vehicles)
	{
		if (vehicle == m_vehicle)
			continue;

		int otherCheckPoint = vehicle->getNextCheckPointIndex();
		int diffCheckpoint = abs(otherCheckPoint - checkPoint);
		bool ahead = (otherCheckPoint > checkPoint && diffCheckpoint < m_splinePath.size() / 2) || (otherCheckPoint < checkPoint && diffCheckpoint > m_splinePath.size() / 2);

		if (ahead && diffCheckpoint < maxOvertakeRange) 
		{
			return vehicle;
		}
	}

	return  nullptr;
}

Vector2D Overtake::NearestPointOnLine(Vector2D lineStart, Vector2D lineEnd, Vector2D point)
{
	//Get nearest point on line.
	Vector2D line = lineEnd - lineStart;
	Vector2D hypotemuse = point - lineStart;

	if (isEqual(hypotemuse.x, 0) && isEqual(hypotemuse.y, 0))
		return point;

	double lineDot = hypotemuse.Dot(line);
	double linePercent = lineDot / line.Length();

	if (linePercent > 1)
		linePercent = 1;
	else if (linePercent < 0)
		linePercent = 0;

	return lineStart + line * linePercent;
}

Overtake::Overtake(Vehicle* vehicle, std::vector<Vehicle*>& vehicles, std::vector<Vector2D>& path) : m_vehicles(vehicles)
{
	m_vehicleToOvertake = nullptr;
	m_vehicle = vehicle;
	m_splinePath = SplineCurve(path);
}
