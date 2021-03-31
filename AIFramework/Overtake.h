#pragma once
#include "BehaviourTree.h"
#include "Spline.h"
#include "Vehicle.h"

class Overtake :
	public Behaviour
{

private:
	float m_roadRadius = 100.0f;
	Vehicle*     m_vehicle;
	SplineCurve m_splinePath;
	std::vector<Vehicle*>& m_vehicles;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

	bool OnTrack(Vector2D point);

public:

	Overtake(Vehicle* m_vehicle,std::vector<Vector2D>& checkpoints, std::vector<Vehicle*>& vehicles);
};

