#include "AIController.h"



AIController::AIController(Vehicle* vehicle,std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path, std::vector<Vector2D>& checkPoints, float pathWidth)
{

	Behaviour* behaviours[2];
	behaviours[0] = new ReturnToTrack(vehicle, path, 80.0f);
	behaviours[1] = new MoveToCheckPoint(vehicle, checkPoints);
	SequencerBehaviour* sequencer = new SequencerBehaviour(behaviours,2);

	m_behaviourTree = new BehaviourTree(sequencer);
}

AIController::~AIController()
{
	delete m_behaviourTree; m_behaviourTree = nullptr;
}

void AIController::OnArrive(Vector2D point)
{
	
}

void AIController::Control(Vehicle* vehicle, const float deltaTime)
{
	m_behaviourTree->Update(deltaTime);
}
