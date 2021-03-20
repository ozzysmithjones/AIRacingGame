#pragma once
#include <vector>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include "Vector2D.h"

#define COLLUMS 20
#define ROWS 20

struct Point 
{
	int x;
	int y;

	Point(int _x, int _y) 
	{
		x = _x;
		y = _y;
	}

	Point()
	{
		x = 0;
		y = 0;
	}
};

struct AStarNode
{
	int position = 0;
	int distanceToStart = 0;
	int distance = 0;
	int breadcrumb = -1;
};

class GridAStar
{
private:

	float m_xGapping = 1.0f;
	float m_yGapping = 1.0f;
	float m_xStart = 1.0f;
	float m_yStart = 1.0f;


	void GetOrthogonalNeighbours(Point point, Point neighbours[]);
	void GetDiagonalNeighbours(Point point, Point neighbours[]);

	int grid[COLLUMS * ROWS]; // -1 means wall. Otherwise it's a weight value.
	int Heuristic(int start, int goal);

	inline bool InBounds(int x, int y) { return x >= 0 && x < COLLUMS && y >= 0 && y < ROWS; }
	inline int ToIndex(int x, int y) { return x * ROWS + y; }
	inline Point ToPoint(int index) { return Point(index / ROWS, index % ROWS); }
	inline Vector2D ToVector(Point point) { return Vector2D((double)point.x * m_xGapping + m_xStart, ((double)point.y * m_yGapping + m_yStart)); }

public:

	void Init(int gridWeights[COLLUMS * ROWS], float xGap, float yGap, float xStart, float yStart);
	std::vector<int> PathFind(Point start, Point goal);
};


