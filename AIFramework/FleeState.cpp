#include "FleeState.h"
#include "Vehicle.h"

void FleeState::Initialise()
{
}

void FleeState::Enter()
{
	m_target = m_targets[rand() % m_targets.size()];
}

void FleeState::Update(const float deltaTime)
{
	float speed = m_vehicle->getCurrentSpeed();
	if (speed <= 0.0f)
		return;

	float distance = m_vehicle->getVehiclePosition().Distance(m_target->getVehiclePosition());
	Vector2D predictedPosition = m_target->getPredictedPosition(distance / speed);
	m_vehicle->RotateAwayFrom(deltaTime, predictedPosition);
	m_vehicle->Move(deltaTime);
}

void FleeState::Leave()
{
}

bool FleeState::CanTransition()
{
	return true;
}

FleeState::FleeState(Vehicle* vehicle, Vehicle* targets[], int numTargets)
{
	m_vehicle = vehicle;

	for (unsigned int i = 0; i < numTargets; i++)
	{
		m_targets.push_back(targets[i]);
	}

	m_target = m_targets[0];
}
