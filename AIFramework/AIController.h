#pragma once
#include "Controller.h"
#include "BehaviourTree.h"

//behaviours for behaviour tree
#include "MoveToCheckPoint.h"
#include "ReturnToTrack.h"

class AIController : public Controller
{

public:

	AIController(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path,std::vector<Vector2D>& checkPoints, float pathWidth);
	~AIController();

private:

	BehaviourTree* m_behaviourTree;

	// Inherited via Controller
	virtual void Control(Vehicle* vehicle, const float deltaTime) override;
	virtual void OnArrive(Vector2D point) override;
};

