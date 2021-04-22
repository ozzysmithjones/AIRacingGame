#pragma once
#include <vector>
#include "FiniteStateMachine.h"

class Vehicle;
class ChaseState :
	public State
{

private:

	Vehicle* m_vehicle;
	Vehicle* m_target;
	std::vector<Vehicle*> m_targets;
	
	// Inherited via State
	virtual void Initialise() override;
	virtual void Enter() override;
	virtual void Update(const float deltaTime) override;
	virtual void Leave() override;
	virtual bool CanTransition() override;

public:

	ChaseState(Vehicle* vehicle, Vehicle* targets[], int numTargets);

};

