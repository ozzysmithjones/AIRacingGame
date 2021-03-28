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
	Vector2D nextCheckPoint = m_checkPoints[m_checkPointIndex];
	float distance = (nextCheckPoint - position).Length();

	//move towards checkpoint.
	if (distance > 16.0f)
	{
		m_vehicle->MoveTowardsPoint(nextCheckPoint, false);
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
