#pragma once
#include "Controller.h"
#include "VehicleStateMachine.h"
#include "BehaviourTree.h"

//behaviours for behaviour tree
#include "Overtake.h"
#include "MoveToCheckPoint.h"
#include "ObstacleAvoidance.h"
#include "ReturnToTrack.h"

//vehicle state machine.

enum class AIMode
{
	IDLE,
	RACING,
	COMMAND
};

class AICarController : public Controller
{
public:

	AICarController(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path,std::vector<Vector2D>& checkPoints, float pathWidth);
	~AICarController();

private:

	Vehicle* m_vehicle;
	AIMode m_mode;
	BehaviourTree* m_behaviourTree;
	FiniteStateMachine* m_stateMachine;

	// Inherited via Controller
	virtual void Control(Vehicle* vehicle, const float deltaTime) override;
	virtual void OnArrive(Vector2D point) override;
	virtual void OnMouseUp(int x, int y) override;
	virtual void OnKeyPress(WPARAM key) override;
};


