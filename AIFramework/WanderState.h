#pragma once
class Vehicle;

#include "FiniteStateMachine.h"
class WanderState :
	public State
{

private:

	const float m_aimSpeed = 100.0f;
	float m_targetRotation;
	Vehicle* m_vehicle;

	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

	inline float RandVal() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }

public:

	WanderState(Vehicle* vehicle);
};

