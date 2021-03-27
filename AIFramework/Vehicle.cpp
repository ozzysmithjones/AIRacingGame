#include "Vehicle.h"


HRESULT	Vehicle::initMesh(ID3D11Device* pd3dDevice)
{
	m_scale = XMFLOAT3(30, 20, 1);
	setTextureName(L"Resources\\car_blue.dds");

	HRESULT hr = DrawableGameObject::initMesh(pd3dDevice);

	m_maxSpeed = 200;
	m_currentSpeed = m_maxSpeed;
	m_currentSpeedAlpha = 1;
	m_currentAngularVelocity = 4.0f;
	setVehiclePosition(Vector2D(0, 0));

	return hr;
}

void Vehicle::update(const float deltaTime)
{
	if (m_isPath)
	{
		m_pathProgress += deltaTime * 5.0f;
		m_currentPosition = m_path.GetPoint(m_pathProgress);
		Vector2D gradient = m_path.GetGradient(m_pathProgress);
		m_radianRotation = atan2f(gradient.y,gradient.x);
		
	}
	else 
	{
		Vector2D diff = m_positionTo - m_currentPosition;
		if (diff.LengthSq() > 9.0f)
		{


			//calculate target angle
			m_targetRotation = atan2f(diff.y, diff.x);
			float angleStep = deltaTime * PI * m_currentAngularVelocity;
			float clockwise = getClockwise(m_radianRotation, m_targetRotation);

			//rotation, direction and position
			m_radianRotation = addRadian(clockwise * angleStep, m_radianRotation);
			Vector2D direction = Vector2D(cosf(m_radianRotation), sinf(m_radianRotation));

			//work out turning circle, and slow down if the target position is in it.
			SlowInTurnCircle(deltaTime, clockwise, direction);


			m_currentPosition += direction * deltaTime * m_currentSpeed;
		}
	}

	// set the current position for the drawable gameobject
	setPosition(XMFLOAT3((float)m_currentPosition.x, (float)m_currentPosition.y, 0));

	DrawableGameObject::update(deltaTime);
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
		setCurrentSpeed(alpha);
	}
	else
	{
		setCurrentSpeed(1);
		//Accelerate(deltaTime);
	}
	
}

// a ratio: a value between 0 and 1 (1 being max speed)
void Vehicle::setCurrentSpeed(const float speed)
{
	m_currentSpeed = m_maxSpeed * speed;
	m_currentSpeed = max(0, m_currentSpeed);
	m_currentSpeed = min(m_maxSpeed, m_currentSpeed);
	m_currentSpeedAlpha = speed;
}

// a position to move to
void Vehicle::setPositionTo(Vector2D position)
{
	m_startPosition = m_currentPosition;
	m_positionTo = position;
}

// the current position
void Vehicle::setVehiclePosition(Vector2D position)
{
	m_currentPosition = position;
	m_positionTo = position;
	m_startPosition = position;
	setPosition(XMFLOAT3((float)position.x, (float)position.y, 0));
}

void Vehicle::SetPath(std::vector<Vector2D>& path)
{
	m_isPath = true;
	m_path = SplineCurve(path);
	m_currentPosition = m_path[0];
	m_positionTo = m_path[1];
}


void Vehicle::Accelerate(float deltaTime)
{
	setCurrentSpeed(m_currentSpeedAlpha + deltaTime * 20.0f);
}

void Vehicle::Break(float deltaTime)
{
	setCurrentSpeed(m_currentSpeedAlpha - deltaTime * 20.0f);
}


