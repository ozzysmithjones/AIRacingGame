#include "Spline.h"

Vector2D SplineCurve::GetPoint(float t, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel)
{
	double tt = (double)t * t;
	double ttt = tt * t;

	//four seperate curves indicate the percentage of each point on the final result.
	double percentStart = 3.0 * ttt - 5.0 * tt + 2.0;
	double percentStartRepel = -ttt + 2.0 * tt - t;
	double percentEnd = -3.0 * ttt + 4.0 * tt + t;
	double percentEndRepel = ttt - tt;

	double x = 0.5 * ((start.x * percentStart) + (startRepel.x * percentStartRepel)  + (end.x * percentEnd)  + (endRepel.x * percentEndRepel));
	double y = 0.5 * ((start.y * percentStart) + (startRepel.y * percentStartRepel)  + (end.y * percentEnd)  + (endRepel.y * percentEndRepel));

	return Vector2D(x,y);
}

Vector2D SplineCurve::GetGradient(float t, Vector2D start, Vector2D end, Vector2D startRepel, Vector2D endRepel)
{
	double tt = (double)t * t;

	//four seperate curves indicate the percentage of each point on the final result.
	double percentStart = 9.0 * tt - 10.0 * t;
	double percentStartRepel = -3 * tt + 4.0 * t - 1.0;
	double percentEnd = -9.0 * tt + 8.0 * t + 1.0;
	double percentEndRepel = 3.0 * tt - 2.0 * t;

	double x = 0.5 * ((start.x * percentStart) + (startRepel.x * percentStartRepel) + (end.x * percentEnd) + (endRepel.x * percentEndRepel));
	double y = 0.5 * ((start.y * percentStart) + (startRepel.y * percentStartRepel) + (end.y * percentEnd) + (endRepel.y * percentEndRepel));

	return Vector2D(x,y);
}

Vector2D SplineCurve::GetNearestMidPoint(Vector2D point, int& startIndex, int& endIndex)
{
	double minLength = 0.0;
	Vector2D mid = points[0];
	for (int i = 0; i < points.size(); i++)
	{
		startIndex = i;
		endIndex = i + 1;
		if (endIndex >= points.size())
			endIndex = 0;

		//Get nearest point on line.
		Vector2D line = points[endIndex] - points[startIndex];
		Vector2D hypotemuse = point - points[startIndex];

		if (isEqual(line.x, 0) && isEqual(line.y, 0))
			return point;

		if (isEqual(hypotemuse.x,0) && isEqual(hypotemuse.y,0))
			return point;

		double lineDot = hypotemuse.Dot(line);
		double linePercent = lineDot / line.LengthSq();

		Vector2D midPoint = points[startIndex] + line * linePercent;
		double lengthToMidPoint = (point - midPoint).LengthSq();
		if (lengthToMidPoint < minLength || i == 0)
		{
			minLength = lengthToMidPoint;
			mid = midPoint;
		}
	}

	return mid;
}



Vector2D SplineCurve::GetPoint(Vector2D midPoint, int startIndex, int endIndex)
{
	Vector2D start = points[startIndex];
	Vector2D end = points[endIndex];
	Vector2D startRepel = points[(startIndex - 1) >= 1 ? startIndex - 1 : points.size() - 1];
	Vector2D endRepel = points[(endIndex + 1) % points.size()];

	float distance = start.Distance(midPoint) / start.Distance(end);

	return GetPoint(distance,start,end,startRepel,endRepel);
}

Vector2D SplineCurve::GetGradient(Vector2D midPoint, int startIndex, int endIndex)
{
	Vector2D start = points[startIndex];
	Vector2D end = points[endIndex];
	Vector2D startRepel = points[(startIndex - 1) >= 1 ? startIndex - 1 : points.size() - 1];
	Vector2D endRepel = points[(endIndex + 1) % points.size()];

	float distance = start.Distance(midPoint) / start.Distance(end);
	return GetGradient(distance, start, end, startRepel, endRepel);
}

Vector2D SplineCurve::GetPoint(float normalisedDistance, int startIndex, int endIndex)
{
	Vector2D start = points[startIndex];
	Vector2D end = points[endIndex];
	Vector2D startRepel = points[(startIndex - 1) >= 1 ? startIndex - 1 : points.size() - 1];
	Vector2D endRepel = points[(endIndex + 1) % points.size()];

	return GetPoint(normalisedDistance,start,end,startRepel,endRepel);
}

Vector2D SplineCurve::GetGradient(float normalisedDistance, int startIndex, int endIndex)
{
	Vector2D start = points[startIndex];
	Vector2D end = points[endIndex];
	Vector2D startRepel = points[(startIndex - 1) >= 1 ? startIndex - 1 : points.size() - 1];
	Vector2D endRepel = points[(endIndex + 1) % points.size()];

	return GetGradient(normalisedDistance, start, end, startRepel, endRepel);
}

Vector2D SplineCurve::GetPoint(float distance)
{
	int start = ((int)distance + 1) % points.size();
	int startRepel = start >= 1 ? start - 1 : points.size() - 1;
	int end = (start + 1) % points.size();
	int endRepel = (start + 2) % points.size();

	return SplineCurve::GetPoint(distance - (int)distance, points[start], points[end], points[startRepel], points[endRepel]);
}

Vector2D SplineCurve::GetGradient(float distance)
{
	int start = ((int)distance + 1) % points.size();
	int startRepel = start >= 1 ? start - 1 : points.size() - 1;
	int end = (start + 1) % points.size();
	int endRepel = (start + 2) % points.size();

	return SplineCurve::GetGradient(distance - (int)distance, points[start], points[end], points[startRepel], points[endRepel]);
}

SplineCurve::SplineCurve()
{
}

SplineCurve::SplineCurve(std::vector<Vector2D>& _points)
{
	points = _points;
}
