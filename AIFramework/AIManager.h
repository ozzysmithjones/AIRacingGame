#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>

//using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;
class GridAStar;

typedef std::vector<DrawableGameObject*> vecDrawables;
typedef std::vector<Vehicle*> vecVehicles;
typedef std::vector<Waypoint*> vecWaypoints;
typedef std::vector<PickupItem*> vecPickups;
typedef std::vector<int> vecPath;

class AIManager
{
public:

	void Release();

	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);

protected:
	bool	checkForCollisions();

private:

	int m_checkPointIndex   = 0;
	GridAStar*				m_AStar;
	vecWaypoints            m_waypoints;
	vecPickups              m_pickups;
	vecVehicles				m_cars;
	vecPath					m_path;
	vecPath                 m_checkPoints;
};

