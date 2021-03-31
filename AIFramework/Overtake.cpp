#include "Overtake.h"

void Overtake::Reset()
{
}

void Overtake::Start()
{
}

BehaviourState Overtake::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState();
}

BehaviourState Overtake::Update(const float deltaTime, Behaviour*& childToPush)
{
	Vector2D position = m_vehicle->getVehiclePosition();
	int startNode; int endNode;

	Vector2D road = m_splinePath.GetNearestLinePoint(position,startNode,endNode);
	Vector2D roadDirection = (m_splinePath[endNode] - m_splinePath[startNode]);
	roadDirection.Normalize();
	Vector2D roadRight = Vector2D(-roadDirection.y, roadDirection.x);

	for (auto other : m_vehicles)
	{
		if (other == m_vehicle)
			continue;

		int otherStartNode; int otherEndNode;
		Vector2D otherPosition = other->getPredictedPosition(deltaTime);
		Vector2D otherRoad = m_splinePath.GetNearestLinePoint(otherPosition,otherStartNode,otherEndNode);

		if (otherEndNode != endNode)
			continue;

		if (!OnTrack(otherPosition))
			continue;

		if(position.DistanceSq(m_splinePath[endNode]) <= otherPosition.DistanceSq(m_splinePath[endNode]))
			continue;

		Vector2D rightTarget = otherPosition + (roadRight * 40.0f);
		Vector2D leftTarget = otherPosition + (roadRight * -40.0f);

		if (OnTrack(rightTarget))
		{
			m_vehicle->Accelerate(deltaTime);
			m_vehicle->RotateTowards(deltaTime,rightTarget,false);
			m_vehicle->Move(deltaTime);
			return BehaviourState::RUNNING;
		}
		else if (OnTrack(leftTarget))
		{
			m_vehicle->Accelerate(deltaTime);
			m_vehicle->RotateTowards(deltaTime, rightTarget, false);
			m_vehicle->Move(deltaTime);
			return BehaviourState::RUNNING;
		}
	}

	//m_vehicle->Move(deltaTime);
	return BehaviourState::SUCCESS;
}

bool Overtake::OnTrack(Vector2D point)
{
	Vector2D road = m_splinePath.GetNearestLinePoint(point);
	return road.DistanceSq(point) < m_roadRadius * m_roadRadius;
}

Overtake::Overtake(Vehicle* vehicle, std::vector<Vector2D>& checkpoints, std::vector<Vehicle*>& vehicles) : m_vehicles(vehicles)
{
	m_vehicle = vehicle;
	m_splinePath = SplineCurve(checkpoints);
}
