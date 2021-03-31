#pragma once
#include "Controller.h"
#include "FiniteStateMachine.h"
#include "BehaviourTree.h"

//behaviours for behaviour tree
#include "MoveToCheckPoint.h"
#include "ObstacleAvoidance.h"
#include "Overtake.h"
#include "ReturnToTrack.h"

class AICarBehaviourTree : public Controller
{
public:

	AICarBehaviourTree(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path,std::vector<Vector2D>& checkPoints, float pathWidth);
	~AICarBehaviourTree();

private:

	BehaviourTree* m_behaviourTree;

	// Inherited via Controller
	virtual void Control(Vehicle* vehicle, const float deltaTime) override;
	virtual void OnArrive(Vector2D point) override;
};

class AICarStateMachine : public Controller
{
	FiniteStateMachine* m_stateMachine;

	// Inherited via Controller
	virtual void Control(Vehicle* vehicle, const float deltaTime) override;
	virtual void OnArrive(Vector2D point) override;
};

