#include "MoveToCheckPoint.h"

void MoveToCheckPoint::Reset()
{
	m_checkPointIndex = 0;
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
	m_checkPoints.GetNearestLinePoint(position, startNode, endNode);
	int diff = abs(endNode - m_checkPointIndex);

	if (endNode > m_checkPointIndex && diff < m_checkPoints.size() / 2)
	{
		m_checkPointIndex = endNode;
	}
	else if (endNode < m_checkPointIndex && diff > m_checkPoints.size() / 2)
	{
		m_checkPointIndex = endNode;
	}

	//go to next checkpoint.
	Vector2D nextCheckPoint = m_checkPoints[m_checkPointIndex];
	float distance = (nextCheckPoint - position).Length();

	//move towards checkpoint.
	if (distance > 16.0f)
	{
		//m_vehicle->Accelerate(deltaTime);
		m_vehicle->MoveTowards(deltaTime, nextCheckPoint, false,true);
		return BehaviourState::SUCCESS;
	}

	//update next checkpoint
	m_checkPointIndex++;
	if (m_checkPointIndex >= m_checkPoints.size())
		m_checkPointIndex = 0;

	return BehaviourState::SUCCESS;
}

MoveToCheckPoint::MoveToCheckPoint(Vehicle* vehicle, std::vector<Vector2D>& checkpoints) : m_checkPoints(checkpoints)
{
	m_vehicle = vehicle;
}
