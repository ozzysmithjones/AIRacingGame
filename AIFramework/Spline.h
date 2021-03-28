#pragma once
#include <limits>
#include <vector>
#include "Vector2D.h"

struct SplineCurve
{
private:

	std::vector<Vector2D> points;

public:

	static Vector2D GetSplinePoint(float normalisedDistance, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel);
	static Vector2D GetSplineGradient(float normalisedDistance, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel);

	Vector2D GetNearestLinePoint(Vector2D point);
	Vector2D GetNearestLinePoint(Vector2D point, int& startIndex, int& endIndex);
	Vector2D GetLineGradient(Vector2D point, int endIndex);

	Vector2D GetSplinePoint(Vector2D midPoint, int startIndex, int endIndex);
	Vector2D GetSplinePoint(float normalisedDistance, int startIndex, int endIndex);
	Vector2D GetSplinePoint(float distance);

	Vector2D GetSplineGradient(Vector2D midPoint, int startIndex, int endIndex);
	Vector2D GetSplineGradient(float normalisedDistance, int startIndex, int endIndex);
	Vector2D GetSplineGradient(float distance);

	Vector2D operator[](int index) { return points[index]; };
	size_t Size() { return points.size(); };

	//SplineCurve();
	SplineCurve(std::vector<Vector2D>& points);
	SplineCurve();
};