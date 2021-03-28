#pragma once
#include "Controller.h"
#include "Spline.h"

class AIController : public Controller
{

public:

	AIController(std::vector<Vehicle*>& vecVehicles, std::vector<PickupItem*>& vecPickUps, std::vector<Vector2D>& path,std::vector<Vector2D>& checkPoints, float pathWidth);

private:

	bool m_movingOffTrack = false;
	Vector2D m_pathResume;
	int m_checkPointIndex = 0;

	std::vector<Vehicle*>& m_vecVehicles;
	std::vector<PickupItem*>& m_vecPickUps;
	std::vector<Vector2D>& m_vecCheckPoints;
	SplineCurve m_path;
	float m_pathWidth;

	// Inherited via Controller
	virtual void Control(Vehicle* vehicle, const float deltaTime) override;
	virtual void OnArrive(Vector2D point) override;
};

