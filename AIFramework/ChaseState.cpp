#include "ChaseState.h"
#include "Vehicle.h"

void ChaseState::Initialise()
{
}

void ChaseState::Enter()
{

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

ChaseState::ChaseState(Vehicle* vehicle, Vehicle* target)
{
	m_vehicle = vehicle;
	m_target = target;
}

