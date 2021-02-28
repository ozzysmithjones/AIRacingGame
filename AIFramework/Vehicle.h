#pragma once
#include "DrawableGameObject.h"
#include "Vector2D.h"

//#define 

struct Circle 
{
	float radius;
	Vector2D center;
};

class Vehicle : public DrawableGameObject
{
public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
	virtual void update(const float deltaTime);



	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setPositionTo(Vector2D positionTo); // a position to move to
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo

	void Accelerate(float deltaTime);
	void Break(float deltaTime);

	Circle GetTurnCircle() const { return m_turnCircle; }

protected:

	float m_currentSpeedAlpha;
	float m_targetRotation;
	float m_maxSpeed;
	float m_currentSpeed;
	float m_currentAngularVelocity;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;

	Circle m_turnCircle;

	void SlowInTurnCircle(float deltaTime, float clockwise, Vector2D& direction);
};

