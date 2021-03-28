#pragma once
#include "BehaviourTree.h"
#include "Vector2D.h"
#include "Vehicle.h"

class MoveToCheckPoint :
	public Behaviour
{
private:

	Vehicle* m_vehicle;
	std::vector<Vector2D>& m_checkPoints;
	int m_checkPointIndex = 0;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:

	MoveToCheckPoint(Vehicle* vehicle, std::vector<Vector2D>& checkpoints);
};

