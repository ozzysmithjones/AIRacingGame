#pragma once
#include "BehaviourTree.h"
#include "Vector2D.h"
#include "Spline.h"
#include "Vehicle.h"

class ReturnToTrack :
	public Behaviour
{


private:

	float m_pathWidth = 40.0f;
	Vector2D m_pathResumePoint;
	Vehicle* m_vehicle;
	SplineCurve m_path;

	// Inherited via Behaviour
	virtual void Reset() override;
	virtual void Start() override;
	virtual BehaviourState Resume(Behaviour* child, BehaviourState childBehaviourState) override;
	virtual BehaviourState Update(const float deltaTime, Behaviour*& childToPush) override;

public:

	ReturnToTrack(Vehicle* vehicle, std::vector<Vector2D>& path, float pathWidth);
};

