#pragma once
#include <vector>
#include "MoveToCheckPoint.h"
#include "Vehicle.h"
#include "BehaviourTree.h"


class Overtake :
	public Behaviour
{
private:

	const int maxOvertakeRange = 6;
	Vehicle* m_vehicleToOvertake;
	Vehicle* m_vehicle;
	std::vector<Vehicle*>& m_vehicles;
	SplineCurve m_splinePath;


	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

	Vehicle* FindVehicleToOvertake();

	Vector2D NearestPointOnLine(Vector2D lineStart, Vector2D lineEnd, Vector2D point);

public:

	Overtake(Vehicle* vehicle, std::vector<Vehicle*>& vehicles, std::vector<Vector2D>& path);
};

