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
	Vector2D position = m_vehicle->getPredictedPosition(deltaTime);
	Circle turnCircle = m_vehicle->CalculateTurnCircle(1.0f);
	Vector2D direction = m_vehicle->getVehicleDirection();
	float breakDistance = m_vehicle->getBreakDistance();

	for (auto other : m_vehicles)
	{
        if (other == m_vehicle)
            continue;

		Vector2D otherPosition = other->getVehiclePosition();
		Vector2D toOther = otherPosition - position;
		float distance = toOther.LengthSq();

		bool infront = LineIntersectCircle(position, direction, otherPosition, m_radius);
		if (infront && distance < breakDistance * breakDistance)
		{
			m_vehicle->Break(deltaTime);
		}

		if ((distance < m_radius * m_radius || (distance < turnCircle.radius * turnCircle.radius && infront)) && toOther.Dot(direction) >= 0.0f) 
		{
			m_vehicle->RotateAwayFrom(deltaTime, otherPosition);
			m_vehicle->Move(deltaTime);
			return BehaviourState::RUNNING;
		}
	}

	return BehaviourState::SUCCESS;
}

void ObstacleAvoidance::CheckForCollisions()
{
}

bool ObstacleAvoidance::LineIntersectCircle(Vector2D lineOrigin, Vector2D lineVector, Vector2D point, double radius)
{
	//Get nearest point on line.
	Vector2D hypotemuse = point - lineOrigin;

	if (isEqual(hypotemuse.x, 0) && isEqual(hypotemuse.y, 0))
		return true;

	double lineDot = hypotemuse.Dot(lineVector);
	if (lineDot < 0)
		return false;

	Vector2D midPoint = lineOrigin + lineVector * lineDot;
	double lengthToMidPoint = (point - midPoint).LengthSq();

	return (lengthToMidPoint <= radius * radius);
}

ObstacleAvoidance::ObstacleAvoidance(Vehicle* vehicle, std::vector<Vehicle*>& vehicles) : m_vehicles(vehicles)
{
	m_vehicle = vehicle;
}

void ObstacleAvoidance::Start()
{

}
