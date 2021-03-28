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
public:

	virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
	virtual void update(const float deltaTime);

	void setController(Controller* controller) { m_controller = controller; }
	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void SetNormalisedSpeed(float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setPositionTo(Vector2D positionTo, bool slowToTarget); // a position to move to
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void SetPath(std::vector<Vector2D>& path);

	Controller* getController() const { return m_controller; }
	float getMaxSpeed() const { return m_maxSpeed; }
	float getCurrentSpeed() const { return m_currentSpeed; }
	float getAngularVelocity() const { return m_currentAngularVelocity; }
	Vector2D getVehiclePosition() const { return m_currentPosition; }
	Vector2D getVehicleDirection() const { return m_direction; }
	Vector2D getPredictedPosition(float time);

	void Accelerate(float deltaTime);
	void Break(float deltaTime);
	Circle GetTurnCircle() const { return m_turnCircle; }

	~Vehicle();

private:

	float getDegrees(float radians);
	float getRadians(float degrees);
	float addRadian(float a, float b); //adds two radians and returns the result.(includes wrapping)
	float getClockwise(float a, float b, float maxProximity = 0.01f); //get's the shortest path between two angles.(1 = clockwise, -1 = anti-clockwise)

	float accelerateSpeed = 1.0f;
	float breakSpeed = 1.0f;

	float m_maxSpeed;
	float m_currentNormalisedSpeed;
	float m_currentSpeed;
	float m_currentAngularVelocity;
	
	Vector2D m_currentPosition;
	Vector2D m_positionTo;
	Vector2D m_direction;

	bool  m_slowToTarget = false;
	bool  m_isPositionTo = false;
	Circle m_turnCircle;

	void MoveTowardsTarget(const float& deltaTime, Vector2D positionTo);
	void SlowInTurnCircle(float deltaTime, float clockwise, Vector2D& direction);


	Controller* m_controller = nullptr;

	float m_pathProgress;
	bool m_arrived = false;
	bool m_isPath = false;
	SplineCurve m_path;
};

