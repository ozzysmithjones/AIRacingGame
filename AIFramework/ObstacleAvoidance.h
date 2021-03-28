#pragma once
#include "Vehicle.h"

#include "BehaviourTree.h"
class ObstacleAvoidance :
	public Behaviour
{
private:

	Vehicle* m_vehicle;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:

	ObstacleAvoidance(Vehicle* vehicle);

	
};

