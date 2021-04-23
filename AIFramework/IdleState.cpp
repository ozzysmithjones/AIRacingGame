#include "IdleState.h"
#include "Vehicle.h"

void IdleState::Initialise()
{
}

void IdleState::Enter()
{
}

void IdleState::Update(const float deltaTime)
{
	m_vehicle->Move(deltaTime);
}

void IdleState::Leave()
{
}

bool IdleState::CanTransition()
{
	return false;
}

IdleState::IdleState(Vehicle* vehicle)
{
	m_vehicle = vehicle;
}
