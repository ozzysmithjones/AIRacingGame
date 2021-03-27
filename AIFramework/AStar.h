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

	void GetOrthogonalNeighbours(Point point, Point neighbours[]);
	void GetDiagonalNeighbours(Point point, Point neighbours[]);

	int grid[COLLUMS * ROWS]; // -1 means wall. Otherwise it's a weight value.
	int Heuristic(int start, int goal);

	inline bool InBounds(int x, int y) { return x >= 0 && x < COLLUMS && y >= 0 && y < ROWS; }
	inline int ToIndex(int x, int y) { return x * ROWS + y; }

	inline Point ToPoint(int index) { return Point(index / ROWS, index % ROWS); }

public:

	void Init(int gridWeights[COLLUMS * ROWS]);
	std::vector<int> PathFind(Point start, Point goal, bool includeStart = true);
	std::vector<int> PathFind(int startIndex,int goalIndex, bool includeStart = false);
};


