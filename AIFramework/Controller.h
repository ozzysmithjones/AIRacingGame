#pragma once
#include "Vehicle.h"
#include "Vector2D.h"

class Controller
{
public:
	virtual void Control(Vehicle* vehicle, const float deltaTime) = 0;
	virtual void OnArrive(Vector2D point) = 0;
	virtual void OnMouseUp(int x, int y) = 0;
	virtual void OnKeyPress(WPARAM key) = 0;
};

