#pragma once
#include "Vector2D.h"
#include "FiniteStateMachine.h"

class Vehicle;
class ArriveState :
	public State
{
private:

	Vector2D& m_mousePosition;
	Vehicle* m_vehicle;


	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

public:

	ArriveState(Vehicle* vehicle,Vector2D& mousePosition);
};

