#include "ObstacleAvoidance.h"

void ObstacleAvoidance::Reset()
{
}

BehaviourState ObstacleAvoidance::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState::RUNNING;
}

BehaviourState ObstacleAvoidance::Update(const float deltaTime, Behaviour*& childToPush)
{
	Vector2D position = m_vehicle->getVehiclePosition();
	Vector2D direction = m_vehicle->getVehicleDirection();

	for (auto other : m_vehicles)
	{
        if (other == m_vehicle)
            continue;

		Vector2D otherPosition = other->getVehiclePosition();
		Vector2D toOther = otherPosition - position;
		float distance = toOther.LengthSq();

		if (distance < (m_radius * m_radius) && toOther.Dot(direction) > 0.0f) 
		{
			m_vehicle->RotateAwayFrom(deltaTime, otherPosition);
			m_vehicle->Break(deltaTime, 0.1f);
			m_vehicle->Move(deltaTime);
			return BehaviourState::RUNNING;
		}
	}

	return BehaviourState::SUCCESS;
}

void ObstacleAvoidance::CheckForCollisions()
{
}

ObstacleAvoidance::ObstacleAvoidance(Vehicle* vehicle, std::vector<Vehicle*>& vehicles) : m_vehicles(vehicles)
{
	m_vehicle = vehicle;
}

void ObstacleAvoidance::Start()
{

}
