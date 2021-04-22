#include "MoveToCheckPoint.h"

void MoveToCheckPoint::Reset()
{
	m_pathIndex = 0;
}

void MoveToCheckPoint::Start()
{

}

BehaviourState MoveToCheckPoint::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState::RUNNING;
}

BehaviourState MoveToCheckPoint::Update(const float deltaTime, Behaviour*& childToPush)
{
	Vector2D position = m_vehicle->getVehiclePosition();

	//if we have moved around the checkpoint, re-calculate our next checkpoint.
	int startNode; int endNode;
	m_path.GetNearestLinePoint(position, startNode, endNode);
	int diff = abs(endNode - m_pathIndex);

	if (endNode > m_pathIndex && diff < m_path.size() / 2)
	{
		m_pathIndex = endNode;
	}
	else if (endNode < m_pathIndex && diff > m_path.size() / 2)
	{
		m_pathIndex = endNode;
	}

	//go to next checkpoint.
	Vector2D nextPathPoint = m_path[m_pathIndex];
	float distance = (nextPathPoint - position).Length();

	//move towards checkpoint.
	if (distance > 16.0f)
	{
		//m_vehicle->Accelerate(deltaTime);
		m_vehicle->MoveTowards(deltaTime, nextPathPoint, false,true);
		return BehaviourState::SUCCESS;
	}

	m_pathIndex++;
	if (m_pathIndex >= m_path.size())
		m_pathIndex = 0;

	return BehaviourState::SUCCESS;
}

MoveToCheckPoint::MoveToCheckPoint(Vehicle* vehicle, std::vector<Vector2D>& path, std::vector<Vector2D>& checkpoints) : m_path(path), m_checkpoints(checkpoints)
{
	m_vehicle = vehicle;
}
