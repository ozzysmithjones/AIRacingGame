#pragma once
#include "FiniteStateMachine.h"
class Vehicle;

class FleeState :
	public State
{
private:

	Vehicle* m_vehicle;
	Vehicle* m_target;

	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

public:

	FleeState(Vehicle* vehicle, Vehicle*  target);
};

