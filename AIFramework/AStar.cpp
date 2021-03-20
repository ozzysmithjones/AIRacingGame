#include "AStar.h"

void GridAStar::GetOrthogonalNeighbours(Point point, Point neighbours[])
{
	for (int i = 0; i < 4; i++)
	{
		int xOffset;
		int yOffset;

		//using binary operations to for assignment of neighbouring position.
		if ((i & 1) == 0)
		{
			yOffset = 0;
			if ((i & 2) == 0)
			{
				xOffset = 1;
			}
			else
			{
				xOffset = -1;
			}
		}
		else
		{
			xOffset = 0;
			if ((i & 2) == 0)
			{
				yOffset = 1;
			}
			else
			{
				yOffset = -1;
			}
		}

		neighbours[i] = Point(point.x + xOffset, point.y + yOffset);
	}
}

void GridAStar::GetDiagonalNeighbours(Point point, Point neighbours[])
{
	for (int i = 0; i < 4; i++)
	{
		//using binary operations to speed up assignment of neighbouring position.
		int xOffset = (i & 1) == 0 ? 1 : -1;
		int yOffset = (i & 2) == 0 ? 1 : -1;

		neighbours[i] = Point(point.x + xOffset, point.y + yOffset);
	}
}

int GridAStar::Heuristic(int start, int goal)
{
	Point startPoint = ToPoint(start);
	Point goalPoint = ToPoint(goal);
	Point diff = Point(goalPoint.x - startPoint.x, goalPoint.y - startPoint.y);

	return sqrt(diff.x * diff.x + diff.y * diff.y) * 10;
}

void GridAStar::Init(int gridWeights[COLLUMS * ROWS], float xGap, float yGap, float xStart, float yStart)
{
	m_xStart = xStart;
	m_yStart = yStart;
	m_xGapping = xGap;
	m_yGapping = yGap;

	for (int i = 0; i < COLLUMS * ROWS; i++)
	{
		grid[i] = gridWeights[i];
	}
}

std::vector<int> GridAStar::PathFind(Point start, Point end)
{
	//set up frontier and closed collections.
	auto compare = [this](AStarNode a, AStarNode b) { return a.distance > b.distance; };
	std::priority_queue<AStarNode, std::vector<AStarNode>, decltype(compare)> priorityQueue(compare);
	std::unordered_map<int,AStarNode> frontier;
	std::unordered_map<int,AStarNode> closed;

	//convert start and end positions to indecies in the grid.
	int startIndex = ToIndex(start.x, start.y);
	int endIndex = ToIndex(end.x, end.y);

	//Create the start node.
	AStarNode currentNode;
	currentNode.position = endIndex;
	currentNode.breadcrumb = -1;
	currentNode.distanceToStart = 0;
	currentNode.distance = Heuristic(endIndex, startIndex);

	priorityQueue.push(currentNode);

	while (!priorityQueue.empty() && currentNode.position != startIndex)
	{
		currentNode = priorityQueue.top();
		priorityQueue.pop();

		closed[currentNode.position] = currentNode;

		Point orthogonal[4];
		Point diagonal[4];

		GetOrthogonalNeighbours(ToPoint(currentNode.position), orthogonal);
		GetDiagonalNeighbours(ToPoint(currentNode.position),diagonal);

		for (int i = 0; i < 4; i++)
		{
			Point& orthoNeighbour = orthogonal[i];

			if (InBounds(orthoNeighbour.x, orthoNeighbour.y))
			{
				int index = ToIndex(orthoNeighbour.x, orthoNeighbour.y);

				if (grid[index] < 0)
				{
					continue;
				}

				if (closed.find(index) != closed.end())
				{
					continue;
				}

				int distanceToStart = currentNode.distanceToStart + 10 * grid[index];
				int distance = distanceToStart + Heuristic(index, startIndex);

				if (frontier.find(index) != frontier.end())
				{
					if (frontier[index].distanceToStart <= distanceToStart)
					{
						continue;
					}
				}

				AStarNode node;
				node.breadcrumb = currentNode.position;
				node.position = index;
				node.distanceToStart = distanceToStart;
				node.distance = distance;

				priorityQueue.push(node);
				frontier[node.position] = node;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			Point& diagNeighbour = diagonal[i];

			if (InBounds(diagNeighbour.x, diagNeighbour.y))
			{
				int index = ToIndex(diagNeighbour.x, diagNeighbour.y);

				if (grid[index] < 0)
				{
					continue;
				}

				if (closed.find(index) != closed.end())
				{
					continue;
				}

				int distanceToStart = currentNode.distanceToStart + 14 * grid[index];
				int distance = distanceToStart + Heuristic(index, startIndex);

				if (frontier.find(index) != frontier.end())
				{
					if (frontier[index].distanceToStart <= distanceToStart) 
					{
						continue;
					}
				}

				AStarNode node;
				node.breadcrumb = currentNode.position;
				node.position = index;
				node.distanceToStart = distanceToStart;
				node.distance = distance;

				priorityQueue.push(node);
				frontier[node.position] = node;
			}
		}

		Point point = ToPoint(currentNode.position);
	}

	if (currentNode.position == endIndex)
		return std::vector<int>();

	//Reading path backwards
	std::vector<int> path;
	path.push_back(currentNode.position);
	while (currentNode.position != endIndex && currentNode.position >= 0)
	{
		currentNode = closed[currentNode.breadcrumb];
		path.push_back(currentNode.position);
	}

	return path;
}

