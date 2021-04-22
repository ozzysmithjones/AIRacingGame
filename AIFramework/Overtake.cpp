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

	if (m_vehicleToOvertake != nullptr)
	{
		m_vehicle->RotateTowards(deltaTime, m_vehicleToOvertake->getVehiclePosition() + m_vehicle->getVehicleDirection() * 30.0f, false);
		m_vehicle->Move(deltaTime);
		return BehaviourState::RUNNING;
	}

	return BehaviourState::SUCCESS;
}

Vehicle* Overtake::FindVehicleToOvertake()
{
	Vector2D position = m_vehicle->getVehiclePosition();
	
	int startIndex, endIndex;
	m_splinePath.GetNearestLinePoint(position, startIndex, endIndex);

	Vector2D nextCheckpoint = m_splinePath[endIndex];
	Vector2D toCheckpoint = nextCheckpoint - position;
	float length = toCheckpoint.Length();
	toCheckpoint = toCheckpoint / length;

	if (toCheckpoint.Dot(m_vehicle->getVehicleDirection()) < 0.0)
		return nullptr;

	if (length <= 32)
		return nullptr;

	float closestDistance = length * length;
	Vehicle* closestVehicle = nullptr;

	for (auto vehicle : m_vehicles)
	{
		if (vehicle == m_vehicle)
			continue;

		Vector2D otherPosition = vehicle->getVehiclePosition();
		Vector2D otherToCheckPoint = nextCheckpoint - otherPosition;
		float otherLength = otherToCheckPoint.Length();
		otherToCheckPoint = otherToCheckPoint / otherLength;

		if (toCheckpoint.Dot(otherToCheckPoint) <= 0.0)
			continue;

		if (otherToCheckPoint.Dot(vehicle->getVehicleDirection()) <= 0.5)
			continue;

		if (otherLength > length)
			continue;

		float distanceToOther = position.DistanceSq(otherPosition);
		if (distanceToOther <= closestDistance)
		{
			closestVehicle = vehicle;
			closestDistance = distanceToOther;
		}
	}

	return closestVehicle;
}

Overtake::Overtake(Vehicle* vehicle, std::vector<Vehicle*>& vehicles, std::vector<Vector2D>& waypoints) : m_vehicles(vehicles)
{
	m_vehicleToOvertake = nullptr;
	m_vehicle = vehicle;
	m_splinePath = SplineCurve(waypoints);
}
