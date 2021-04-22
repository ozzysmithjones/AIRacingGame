#pragma once
#include "DrawableGameObject.h"
#include "Vector2D.h"
#include "Spline.h"

class Controller;
//#define 

struct Circle 
{
	float radius;
	Vector2D center;
};

class Vehicle : public DrawableGameObject
{
public://[Movement based commands to give to the vehicle from AI or Input]

	void Move(float deltaTime);
	void MoveTowards(float deltaTime,Vector2D positionTo, bool slowToTarget, bool correctTurnCircle); // a position to move to
	void Accelerate(float deltaTime, float maxNormalisedSpeed = 1.0f);
	void Break(float deltaTime, float minNormalisedSpeed = 0.0f);
	void Rotate(float deltaTime, int direction);
	void RotateAwayFrom(float deltaTime, Vector2D point);
	void RotateTowards(float deltaTime,Vector2D point, bool correctTurnCircle);

public://[getters/setters for velocity, position, direction, or AI controller]

	virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
	virtual void update(const float deltaTime);
	void setController(Controller* controller) { m_controller = controller; }
	void setVehiclePosition(Vector2D position);
	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void SetNormalisedSpeed(float speed);
	Controller* getController() const { return m_controller; }
	Circle GetTurnCircle() const { return m_turnCircle; }
	Circle CalculateTurnCircle(float rotationDirection);
	Vector2D getVehiclePosition() const { return m_currentPosition; }
	Vector2D getVehicleDirection() const { return m_direction; }
	Vector2D getPredictedPosition(float time);
	float getMaxSpeed() const { return m_maxSpeed; }
	float getCurrentSpeed() const { return m_currentSpeed; }
	float getAngularVelocity() const { return m_currentAngularVelocity; }
	float getBreakDistance() { return (1.0f / breakSpeed) * m_currentSpeed; }
	int getNextCheckPointIndex() const { return m_NextCheckpoint; }
	void setNextCheckPointIndex(int index) { m_NextCheckpoint = index; }

	~Vehicle();

private://[radian math functions]

	float getDegrees(float radians);
	float getRadians(float degrees);
	float addRadian(float a, float b); //adds two radians and returns the result.(includes wrapping)
	float getClockwise(float a, float b, float maxProximity = 0.01f); //get's the shortest path between two angles.(1 = clockwise, -1 = anti-clockwise)

	bool PointInTurnCircle(Vector2D point, Vector2D moveDirection, float rotationDirection, float speed);
	

private:

	int  m_NextCheckpoint = 0;
	float accelerateSpeed = 0.5f;
	float breakSpeed = 2.0f;
	float m_maxSpeed;
	float m_currentNormalisedSpeed;
	float m_currentSpeed;
	float m_currentAngularVelocity;
	
	Controller* m_controller = nullptr;
	Vector2D m_currentPosition;
	Vector2D m_direction;
	Circle m_turnCircle;

	//void SlowInTurnCircle(float deltaTime, float clockwise, Vector2D& direction);
	
};

