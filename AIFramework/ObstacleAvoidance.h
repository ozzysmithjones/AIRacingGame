#pragma once
#include "Vehicle.h"

#include "BehaviourTree.h"
class ObstacleAvoidance :
	public Behaviour
{
private:

	std::vector<Vehicle*>& m_vehicles;
	Vehicle* m_vehicle;
	float m_radius = 60.0f;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

	void CheckForCollisions();

public:

	ObstacleAvoidance(Vehicle* vehicle, std::vector<Vehicle*>& vehicles);
};

