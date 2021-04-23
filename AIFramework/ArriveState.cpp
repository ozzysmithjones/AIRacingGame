#include "ArriveState.h"
#include "Vehicle.h"

void ArriveState::Initialise()
{
}

void ArriveState::Enter()
{
}

void ArriveState::Update(const float deltaTime)
{
	m_vehicle->MoveTowards(deltaTime, m_mousePosition, true, true);
}

void ArriveState::Leave()
{
}

bool ArriveState::CanTransition()
{
	return false;
}

ArriveState::ArriveState(Vehicle* vehicle, Vector2D& mousePosition) : m_mousePosition(mousePosition)
{
	m_vehicle = vehicle;
}
