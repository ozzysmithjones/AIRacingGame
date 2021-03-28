#include "ObstacleAvoidance.h"

void ObstacleAvoidance::Reset()
{
}

BehaviourState ObstacleAvoidance::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState();
}

BehaviourState ObstacleAvoidance::Update(const float deltaTime, Behaviour*& childToPush)
{

	return BehaviourState();
}

ObstacleAvoidance::ObstacleAvoidance(Vehicle* vehicle)
{
	m_vehicle = vehicle;
}

void ObstacleAvoidance::Start()
{

}
