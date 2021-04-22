#include "ChaseState.h"
#include "Vehicle.h"

void ChaseState::Initialise()
{
}

void ChaseState::Enter()
{
	m_target = m_targets[rand() % m_targets.size()];
}

void ChaseState::Update(const float deltaTime)
{
	float speed = m_vehicle->getCurrentSpeed();
	if (speed <= 0.0f)
		return;

	float distance = m_vehicle->getVehiclePosition().Distance(m_target->getVehiclePosition());
	Vector2D predictedPosition = m_target->getPredictedPosition(distance / speed);
	m_vehicle->RotateTowards(deltaTime,predictedPosition,false);
	m_vehicle->Move(deltaTime);
}

void ChaseState::Leave()
{

}

bool ChaseState::CanTransition()
{
	return true;
}

ChaseState::ChaseState(Vehicle* vehicle, Vehicle* targets[], int numTargets)
{
	m_vehicle = vehicle;

	for (unsigned int i = 0; i < numTargets; i++)
	{
		m_targets.push_back(targets[i]);
	}

	m_target = targets[0];
}

