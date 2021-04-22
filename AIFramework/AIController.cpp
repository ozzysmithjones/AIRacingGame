#include "AIController.h"



AICarBehaviourTree::AICarBehaviourTree(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path, std::vector<Vector2D>& checkPoints, float pathWidth)
{

	Behaviour* behaviours[3];
	behaviours[0] = new ObstacleAvoidance(vehicle, vecVehicles);
	behaviours[1] = new ReturnToTrack(vehicle, path, 80.0f);
	//behaviours[2] = new Overtake(vehicle, vecVehicles, checkPoints);
    behaviours[2] = new MoveToCheckPoint(vehicle, path,checkPoints);
	SelectorBehaviour* root = new SelectorBehaviour(behaviours,3);

	m_behaviourTree = new BehaviourTree(root);
}

AICarBehaviourTree::~AICarBehaviourTree()
{
	delete m_behaviourTree; m_behaviourTree = nullptr;
}

void AICarBehaviourTree::OnArrive(Vector2D point)
{
	
}

void AICarBehaviourTree::Control(Vehicle* vehicle, const float deltaTime)
{
	m_behaviourTree->Update(deltaTime);
}

void AICarStateMachine::Control(Vehicle* vehicle, const float deltaTime)
{
}

void AICarStateMachine::OnArrive(Vector2D point)
{
}
