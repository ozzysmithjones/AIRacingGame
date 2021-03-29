#include "ReturnToTrack.h"

void ReturnToTrack::Reset()
{
}

void ReturnToTrack::Start()
{
	int startNode;
	int endNode;
	Vector2D position = m_vehicle->getVehiclePosition();
	m_pathResumePoint = m_path.GetNearestLinePoint(position,startNode,endNode);
	m_pathDirection = m_path[endNode] - m_path[startNode];
}

BehaviourState ReturnToTrack::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState::RUNNING;
}

BehaviourState ReturnToTrack::Update(const float deltaTime, Behaviour*& childToPush)
{
	Vector2D position = m_vehicle->getVehiclePosition();
	Vector2D predicted = m_vehicle->getPredictedPosition(deltaTime);
	Vector2D toRoad = (m_pathResumePoint - predicted);

	if (toRoad.Length() > m_pathWidth)
	{
		m_vehicle->MoveTowards(deltaTime, m_pathResumePoint, true,false);
		return BehaviourState::RUNNING;
	}

	return BehaviourState::SUCCESS;
}

ReturnToTrack::ReturnToTrack(Vehicle* vehicle, std::vector<Vector2D>& path, float pathWidth)
{
	m_vehicle = vehicle;
	m_path = path;
	m_pathWidth = pathWidth;
}
