#pragma once
#include <limits>
#include <vector>
#include "Vector2D.h"

struct SplineCurve
{
private:

	std::vector<Vector2D> points;

public:

	static Vector2D GetPoint(float normalisedDistance, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel);
	static Vector2D GetGradient(float normalisedDistance, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel);

	Vector2D GetNearestMidPoint(Vector2D point, int& startIndex, int& endIndex);

	Vector2D GetPoint(Vector2D midPoint, int startIndex, int endIndex);
	Vector2D GetGradient(Vector2D midPoint, int startIndex, int endIndex);

	Vector2D GetPoint(float normalisedDistance, int startIndex, int endIndex);
	Vector2D GetGradient(float normalisedDistance, int startIndex, int endIndex);

	Vector2D GetPoint(float distance);
	Vector2D GetGradient(float distance);

	Vector2D operator[](int index) { return points[index]; };
	size_t Size() { return points.size(); };

	SplineCurve();
	SplineCurve(std::vector<Vector2D>& points);
};