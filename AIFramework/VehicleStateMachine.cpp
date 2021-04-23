#include "VehicleStateMachine.h"
#include "FleeState.h"
#include "ChaseState.h"
#include "ArriveState.h"
#include "WanderState.h"
#include "IdleState.h"



void VehicleStateMachine::ManageTranstition(State* currentState, int typeID)
{
	/*
	if (typeID == TypeID::GetID<State>())
	{

	}
	*/
}

VehicleStateMachine::VehicleStateMachine(Vehicle* vehicle, Vehicle* target)
{
	AddState(new FleeState(vehicle,target));
	AddState(new WanderState(vehicle));
	AddState(new ChaseState(vehicle, target));
	AddState(new ArriveState(vehicle, m_mousePosition));
	AddState(new IdleState(vehicle));

	m_mousePosition = Vector2D(0, 0);
	m_target = target;
	m_vehicle = vehicle;

	if (vehicle == m_target) 
	{
		TransitionTo<WanderState>();
	}
}

void VehicleStateMachine::KeyPress(WPARAM key)
{
	if (m_target == m_vehicle)
		return;

	switch (key)
	{
	case VK_LEFT:
		TransitionTo<FleeState>();
		break;
	case VK_UP:
		TransitionTo<WanderState>();
		break;
	case VK_RIGHT:
		TransitionTo<ChaseState>();
		break;
	case VK_DOWN:
		TransitionTo<ArriveState>();
		break;
	default:
		break;
	}
}

void VehicleStateMachine::MouseUp(int x, int y)
{
	m_mousePosition = Vector2D(x, y);
}
