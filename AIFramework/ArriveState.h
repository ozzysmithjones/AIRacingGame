#pragma once
#include "FiniteStateMachine.h"

class Vehicle;
class ArriveState :
	public State
{
private:

	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

public:
};

