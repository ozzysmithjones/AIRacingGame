#include "WanderState.h"
#include "Vehicle.h"

void WanderState::Initialise()
{
}

void WanderState::Enter()
{
}

void WanderState::Update(const float deltaTime)
{
	m_targetRotation += 2 * (RandVal() - 0.5f) * deltaTime * m_aimSpeed;
	m_vehicle->RotateTowards(deltaTime, m_targetRotation);
	m_vehicle->Move(deltaTime);
}

void WanderState::Leave()
{
}

bool WanderState::CanTransition()
{
	return true;
}

WanderState::WanderState(Vehicle* vehicle)
{
	m_vehicle = vehicle;
	m_targetRotation = 0.0f;
}
