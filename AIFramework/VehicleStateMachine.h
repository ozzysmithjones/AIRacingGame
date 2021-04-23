#pragma once
#include "Vector2D.h"
#include "FiniteStateMachine.h"


class Vehicle;

class VehicleStateMachine :
	public FiniteStateMachine
{

private:

	Vector2D m_mousePosition;
	Vehicle* m_vehicle;
	Vehicle* m_target;

	// Inherited via FiniteStateMachine
	virtual void ManageTranstition(State* currentState, int typeID) override;
public:

	VehicleStateMachine(Vehicle* vehicle, Vehicle* target);

	// Inherited via FiniteStateMachine
	virtual void KeyPress(WPARAM key) override;
	virtual void MouseUp(int x, int y) override;
};

