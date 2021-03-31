#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include <vector>
#include "Vector2D.h"

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
typedef std::vector<int> VecPathIndecies;
typedef std::vector<Vector2D> vecPathCoordinates;

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
	VecPathIndecies		    m_path;
	VecPathIndecies         m_checkPoints;
	vecPathCoordinates      m_checkPointCoords;
	vecPathCoordinates      m_pathPointCoords;
};

