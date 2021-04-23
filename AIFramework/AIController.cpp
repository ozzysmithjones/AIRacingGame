#include "AIController.h"



AICarController::AICarController(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path, std::vector<Vector2D>& checkPoints, float pathWidth)
{
	m_vehicle = vehicle;
	m_mode = AIMode::COMMAND;

	Behaviour* behaviours[4];
	behaviours[0] = new ObstacleAvoidance(vehicle, vecVehicles);
	behaviours[1] = new ReturnToTrack(vehicle, path, 80.0f);
	behaviours[2] = new Overtake(vehicle, vecVehicles, path);
    behaviours[3] = new MoveToCheckPoint(vehicle, path,checkPoints);
	SelectorBehaviour* root = new SelectorBehaviour(behaviours,4);

	m_behaviourTree = new BehaviourTree(root);
	m_stateMachine = new VehicleStateMachine(vehicle, vecVehicles[0]);
}

AICarController::~AICarController()
{
	delete m_behaviourTree; m_behaviourTree = nullptr;
	delete m_stateMachine; m_stateMachine = nullptr;
}

void AICarController::OnArrive(Vector2D point)
{
	
}

void AICarController::OnMouseUp(int x, int y)
{
	if (m_mode == AIMode::COMMAND) {
		m_stateMachine->MouseUp(x, y);
	}
}

void AICarController::OnKeyPress(WPARAM key)
{
	if (m_mode == AIMode::COMMAND) {
		m_stateMachine->KeyPress(key);
	}
}

void AICarController::Control(Vehicle* vehicle, const float deltaTime)
{
	switch (m_mode)
	{
	case AIMode::RACING:
		m_behaviourTree->Update(deltaTime);
		break;
	case AIMode::COMMAND:
		m_stateMachine->Update(deltaTime);
		break;
	}
}
