#pragma once
#include "FiniteStateMachine.h"
class Vehicle;

class VehicleStateMachine :
	public FiniteStateMachine
{

private:
	// Inherited via FiniteStateMachine
	virtual void ManageTranstition(State* currentState, int typeID) override;
public:

	VehicleStateMachine(Vehicle* vehicle);
};

