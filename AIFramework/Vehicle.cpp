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
	setVehiclePosition(Vector2D(0, 0));

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	
	/*
	if (m_isPath)
	{
		int start = 0;
		int end = 0;

		m_pathMiddle = m_path.GetNearestLinePoint(m_currentPosition, start, end);
		m_currentPosition = m_path.GetSplinePoint(m_pathProgress);
		Vector2D gradient = m_path.GetSplineGradient(m_pathMiddle,start,end);
		m_radianRotation = atan2f(gradient.y,gradient.x);

		m_pathProgress += deltaTime * 5.0f;
		
	}
	*/

	if (m_controller != nullptr)
		m_controller->Control(this,deltaTime);

	if (m_isPositionTo) 
	{
		MoveTowardsTarget(deltaTime,m_positionTo);
	}
	else 
	{
		m_currentPosition += m_direction * deltaTime * m_currentSpeed;
	}


	// set the current position for the drawable gameobject
	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));
	DrawableGameObject::update(deltaTime);
}

void Vehicle::MoveTowardsTarget(const float& deltaTime, Vector2D positionTo)
{
	Vector2D diff = positionTo - m_currentPosition;
	if (diff.LengthSq() > 9.0f)
	{
		m_arrived = false;

		float distance = diff.Length();
		float breakDistance = (1.0f / breakSpeed) * m_currentSpeed;

		if (m_slowToTarget &&  distance <= breakDistance)
		{
			SetNormalisedSpeed(m_currentNormalisedSpeed - breakSpeed * deltaTime);
		}
		else 
		{
			SetNormalisedSpeed(m_currentNormalisedSpeed + accelerateSpeed * deltaTime);
		}


		//calculate target angle
		float m_targetRotation = atan2f(diff.y, diff.x);
		float angleStep = deltaTime * PI * m_currentAngularVelocity;
		float clockwise = getClockwise(m_radianRotation, m_targetRotation);

		//rotation, direction and position
		m_radianRotation = addRadian(clockwise * angleStep, m_radianRotation);
		m_direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));

		//work out turning circle, and slow down if the target position is in it.
		//SlowInTurnCircle(deltaTime, clockwise, m_direction);

		m_currentPosition += m_direction * deltaTime * m_currentSpeed;
	}
	else if (!m_arrived)
	{
		m_currentSpeed = 0.0f;
		m_isPositionTo = false;
		m_arrived = true;

		if (m_controller != nullptr)
		{
			m_controller->OnArrive(positionTo);
		}
	}
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

// a position to move to
void Vehicle::setPositionTo(Vector2D position, bool slowToTarget)
{
	m_positionTo = position;
	m_isPositionTo = true;
	m_slowToTarget = slowToTarget;
}

// the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	m_positionTo = position;
	setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
}

void Vehicle::SetPath(std::vector<Vector2D>& path)
{
	m_isPath = true;
	m_path = SplineCurve(path);
	m_currentPosition = m_path[0];
	m_positionTo = m_path[1];
}


Vector2D Vehicle::getPredictedPosition(float time)
{
	return m_currentPosition + m_direction * time * m_currentSpeed;
}

void Vehicle::Accelerate(float deltaTime)
{
	SetNormalisedSpeed(m_currentNormalisedSpeed + deltaTime * accelerateSpeed);
}

void Vehicle::Break(float deltaTime)
{
	SetNormalisedSpeed(m_currentNormalisedSpeed - deltaTime * breakSpeed);
}


