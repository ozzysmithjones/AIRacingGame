#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>

using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;
class GridAStar;

typedef vector<DrawableGameObject*> vecDrawables;
typedef vector<Vehicle*> vecVehicles;
typedef vector<Waypoint*> vecWaypoints;
typedef vector<PickupItem*> vecPickups;

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

	GridAStar*				m_AStar;
	vecWaypoints            m_waypoints;
	vecPickups              m_pickups;
	vecVehicles				m_cars;

};

