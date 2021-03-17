#include "AStar.h"

int GridAStar::Heuristic(int start, int goal)
{
	Point startPoint = ToPoint(start);
	Point goalPoint = ToPoint(goal);
	Point diff = Point(goalPoint.x - startPoint.x, goalPoint.y - startPoint.y);

	return sqrt(diff.x * diff.x + diff.y * diff.y);
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

std::vector<int> GridAStar::PathFind(Point start, Point goal)
{
	auto compare = [this](AStarNode a, AStarNode b) { return a.distance > b.distance; };
	std::priority_queue<AStarNode, std::vector<AStarNode>, decltype(compare)> frontier(compare);
	std::unordered_map<int, AStarNode> closed;

	int startIndex = ToIndex(start.x, start.y);
	int goalIndex = ToIndex(goal.x, goal.y);

	AStarNode currentNode;
	currentNode.position = goalIndex;
	currentNode.breadcrumb = -1;
	currentNode.distanceToStart = 0;
	currentNode.distance = Heuristic(goalIndex, startIndex);

	frontier.push(currentNode);
	closed[currentNode.position] = currentNode;

	//Frontier expansion.
	while (!frontier.empty() && currentNode.position != startIndex) 
	{
		currentNode = frontier.top();
		frontier.pop();

		Point point = ToPoint(currentNode.position);

		//iterate through each orthogonal neighbour and add them to frontier.
		for (int i = 0; i < 4; i++) 
		{
			int xOffset;
			int yOffset;

			//using binary operations to speed up assignment of neighbouring position.
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

			if (InBounds(point.x + xOffset, point.y + yOffset))
			{
				int index = ToIndex(point.x + xOffset, point.y + yOffset);
				if (grid[index] < 0)
					continue;

				if (closed.find(index) != closed.end())
					continue;

				int distanceToStart = currentNode.distanceToStart + 10;
				int distance = distanceToStart + Heuristic(index, startIndex) + grid[index];

				//Create new node at this position:
				
				AStarNode node;
				node.breadcrumb = currentNode.position;
				node.position = index;
				node.distanceToStart = distanceToStart;
				node.distance = distance;
				frontier.push(node);

				//Add node to visited list.
				closed[node.position] = node;
			}
		}

		//iterate through each diagonal neighbour and add them to frontier.
		for (int i = 0; i < 4; i++)
		{
			//using binary operations to speed up assignment of neighbouring position.
			int xOffset = (i & 1) == 0 ? 1 : -1;
			int yOffset = (i & 2) == 0 ? 1 : -1;

			if (InBounds(point.x + xOffset, point.y + yOffset)) 
			{
				int index = ToIndex(point.x + xOffset, point.y + yOffset);
				if (grid[index] < 0)
					continue;

				if (closed.find(index) != closed.end())
					continue;

				int distanceToStart = currentNode.distanceToStart + 10;
				int distance = distanceToStart + Heuristic(index, startIndex) + grid[index];

				AStarNode node;
				node.breadcrumb = currentNode.position;
				node.position = index;
				node.distanceToStart = distanceToStart;
				node.distance = distance;
				frontier.push(node);

				//Add node to visited list.
				closed[node.position] = node;
			}
		}

		
	}

	if (currentNode.position == goalIndex)
		return std::vector<int>();

	//Reading path backwards
	std::vector<int> path;
	path.push_back(currentNode.position);
	while(currentNode.position != goalIndex && currentNode.position >= 0)
	{
		currentNode = closed[currentNode.breadcrumb];
		path.push_back(currentNode.position);
	}

	return path;
}
