#include "ReturnToTrack.h"

void ReturnToTrack::Reset()
{
}

void ReturnToTrack::Start()
{
	Vector2D position = m_vehicle->getVehiclePosition();
	m_pathResumePoint = m_path.GetNearestLinePoint(position);
}

BehaviourState ReturnToTrack::Resume(Behaviour* child, BehaviourState childBehaviourState)
{
	return BehaviourState::RUNNING;
}

BehaviourState ReturnToTrack::Update(const float deltaTime, Behaviour*& childToPush)
{
	Vector2D position = m_vehicle->getVehiclePosition();
	Vector2D predicted = m_vehicle->getPredictedPosition(deltaTime);
	float distanceToRoad = (predicted - m_pathResumePoint).Length();

	if (distanceToRoad > m_pathWidth)
	{
		m_vehicle->MoveTowardsPoint(m_pathResumePoint, false);
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
