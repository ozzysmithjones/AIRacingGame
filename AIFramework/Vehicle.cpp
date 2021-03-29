#include "Vehicle.h"
#include "Controller.h"


HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setTextureName(L"Resources\\car_blue.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	m_maxSpeed = 200;
	m_currentSpeed = m_maxSpeed;
	m_currentNormalisedSpeed = 1;
	m_currentAngularVelocity = 1.0f;
	m_direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));
	//setVehiclePosition(Vector2D(0, 0));
	return hr;
}

void Vehicle::update(const float deltaTime)
{
	if (m_controller != nullptr)
		m_controller->Control(this,deltaTime);
	DrawableGameObject::update(deltaTime);
}

Vehicle::~Vehicle()
{
	delete m_controller;
}

float Vehicle::getDegrees(float radians)
{
	return (radians / PI) * 180.0f;
}

float Vehicle::getRadians(float degrees)
{
	return (degrees / 180.0f) * PI;
}

float Vehicle::addRadian(float a, float b)
{
	float result = a + b;

	while (result < 0.0f)
	{
		result += 2.0f * PI;
	}
	while (result > 2.0f * PI)
	{
		result -= 2.0f * PI;
	}

	return result;
}

float Vehicle::getClockwise(float a, float b, float maxProximity)
{
	float diff = addRadian(b, -a);

	if (abs(diff) < maxProximity)
		return 0.0f;

	if (diff >= 0.0f && diff <= PI)
	{
		return 1.0f;
	}

	if (diff < 0.0f && diff >= -PI)
	{
		return -1.0f;
	}

	if (diff < -PI)
	{
		return 1.0f;
	}
	else {
		return -1.0f;
	}
}

bool Vehicle::PointInTurnCircle(Vector2D point, Vector2D moveDirection, float rotationDirection, float speed)
{
	float radius = speed / (m_currentAngularVelocity * PI);

	Vector2D directionToCenter = rotationDirection < 0 ? Vector2D(moveDirection.y, -moveDirection.x) : Vector2D(-moveDirection.y, moveDirection.x);
	Vector2D center = (directionToCenter * radius) + m_currentPosition;

	m_turnCircle.radius = radius;
	m_turnCircle.center = center;

	float distSqr = point.DistanceSq(m_turnCircle.center);
	if (distSqr < (radius * radius))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
void Vehicle::SlowInTurnCircle(float deltaTime,float clockwise, Vector2D& direction)
{
	float radius = m_maxSpeed / (m_currentAngularVelocity * PI);

	Vector2D directionToCenter = clockwise < 0 ? Vector2D(direction.y, -direction.x) : Vector2D(-direction.y, direction.x);
	Vector2D center = (directionToCenter * radius) + m_currentPosition;

	m_turnCircle.radius = radius;
	m_turnCircle.center = center;

	float distSqr = m_positionTo.DistanceSq(m_turnCircle.center);
	if (distSqr < (radius * radius))
	{
		float distanceTocenter = sqrt(distSqr);
		float alpha = 1.0f - (distanceTocenter / radius);
		SetNormalisedSpeed(alpha);
	}
	else
	{
		SetNormalisedSpeed(1);
		//Accelerate(deltaTime);
	}
	
}
*/

// a ratio: a value between 0 and 1 (1 being max speed)
void Vehicle::SetNormalisedSpeed(float speed)
{
	if (speed > 1.0f)
		speed = 1.0f;
	else if (speed < 0.0f)
		speed = 0.0f;

	m_currentSpeed = m_maxSpeed * speed;
	m_currentSpeed = max(0, m_currentSpeed);
	m_currentSpeed = min(m_maxSpeed, m_currentSpeed);
	m_currentNormalisedSpeed = speed;
}

void Vehicle::MoveTowards(float deltaTime, Vector2D position, bool slowToTarget, bool correctTurnCircle)
{
	Vector2D diff = position - m_currentPosition;
	if (diff.LengthSq() > 9.0f)
	{
		float distance = diff.Length();
		float breakDistance = (1.0f / breakSpeed) * m_currentSpeed;

		if (slowToTarget && distance <= breakDistance)
		{
			SetNormalisedSpeed(m_currentNormalisedSpeed - breakSpeed * deltaTime);
		}
		else if(!correctTurnCircle)
		{
			SetNormalisedSpeed(m_currentNormalisedSpeed + accelerateSpeed * deltaTime);
		}

		RotateTowards(deltaTime, position,correctTurnCircle);
		m_currentPosition += m_direction * deltaTime * m_currentSpeed;
	}

	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));
}

// the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
}


Vector2D Vehicle::getPredictedPosition(float time)
{
	return m_currentPosition + m_direction * time * m_currentSpeed;
}

void Vehicle::Move(float deltaTime)
{
	m_currentPosition += m_direction * deltaTime * m_currentSpeed;

	// set the current position for the drawable gameobject
	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));
}

void Vehicle::Accelerate(float deltaTime, float maxNormalisedSpeed)
{
	if (m_currentNormalisedSpeed < maxNormalisedSpeed)
	{
		SetNormalisedSpeed(m_currentNormalisedSpeed + deltaTime * accelerateSpeed);
	}
	else 
	{
		SetNormalisedSpeed(maxNormalisedSpeed);
	}
}

void Vehicle::Break(float deltaTime, float minNormalisedSpeed)
{
	if (m_currentNormalisedSpeed > minNormalisedSpeed)
	{
		SetNormalisedSpeed(m_currentNormalisedSpeed - deltaTime * breakSpeed);
	}
	else 
	{
		SetNormalisedSpeed(minNormalisedSpeed);
	}
}

void Vehicle::Rotate(float deltaTime, int direction)
{
	m_radianRotation = addRadian(m_radianRotation,m_currentAngularVelocity * PI * deltaTime * direction);
	m_direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));
}

void Vehicle::RotateAwayFrom(float deltaTime, Vector2D point)
{
	Vector2D diff = point - m_currentPosition;

	//calculate target angle
	float m_targetRotation = atan2f(diff.y, diff.x);
	float angleStep = deltaTime * PI * m_currentAngularVelocity;
	float clockwise = getClockwise(m_radianRotation, m_targetRotation) * -1;

	//rotation and direction
	m_radianRotation = addRadian(clockwise * angleStep, m_radianRotation);
	m_direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));
}

void Vehicle::RotateTowards(float deltaTime, Vector2D point, bool correctTurnCircle)
{
	Vector2D diff = point - m_currentPosition;

	//calculate target angle
	float m_targetRotation = atan2f(diff.y, diff.x);
	float angleStep = deltaTime * PI * m_currentAngularVelocity;
	float clockwise = getClockwise(m_radianRotation, m_targetRotation);

	if (correctTurnCircle)
	{
		if (PointInTurnCircle(point, m_direction, clockwise, m_currentSpeed)) 
		{
			Break(deltaTime);
		}
		else 
		{
			Accelerate(deltaTime);
		}
	}

	//rotation and direction
	m_radianRotation = addRadian(clockwise * angleStep, m_radianRotation);
	m_direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));
}


