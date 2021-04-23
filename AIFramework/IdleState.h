#pragma once
class Vehicle;

#include "FiniteStateMachine.h"
class IdleState :
	public State
{
private:

	Vehicle* m_vehicle;

	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

public:

	IdleState(Vehicle* vehicle);
};

