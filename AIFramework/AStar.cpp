#include "AStar.h"

void GridAStar::GetOrthogonalNeighbours(Point point, Point neighbours[])
{
	for (int i = 0; i < 4; i++)
	{
		int xOffset;
		int yOffset;

		//using binary operations for assignment of neighbouring position.
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

void GridAStar::Init(int gridWeights[COLLUMS * ROWS])
{
	for (int i = 0; i < COLLUMS * ROWS; i++)
	{
		grid[i] = gridWeights[i];
	}
}

std::vector<int> GridAStar::PathFind(Point start, Point end, bool includeStart)
{
	//set up frontier and closed collections.
	auto compare = [this](AStarNode a, AStarNode b) { return a.distance > b.distance; };
	std::priority_queue<AStarNode, std::vector<AStarNode>, decltype(compare)> priorityQueue(compare);
	std::unordered_map<int,int> frontierDistance;
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
		//Always explore from the current best path node in the frontier(the top of the priority queue).
		currentNode = priorityQueue.top();
		priorityQueue.pop();

		//add it to closed list.
		closed[currentNode.position] = currentNode;

		Point orthogonal[4];
		Point diagonal[4];

		GetOrthogonalNeighbours(ToPoint(currentNode.position), orthogonal);
		GetDiagonalNeighbours(ToPoint(currentNode.position),diagonal);

		//Expansion of frontier to neighbouring positions.
		for (int i = 0; i < 8; i++)
		{
			Point& neighbour = i >= 4 ? diagonal[i - 4] : orthogonal[i];
			
			if (!InBounds(neighbour.x, neighbour.y))
			{
				continue;
			}

			int index = ToIndex(neighbour.x, neighbour.y);

			//if the node is a wall (negative move cost) then skip. 
			if (grid[index] < 0)
			{
				continue;
			}

			//if we have already visited this node, skip.
			if (closed.find(index) != closed.end())
			{
				continue;
			}

			//Calculate distance to neighbouring node.
			int distanceToNeighbour = i >= 4 ? 14 : 10;
			int distanceToStart = currentNode.distanceToStart + distanceToNeighbour * grid[index];
			int distance = distanceToStart + Heuristic(index, startIndex);

			//if there is already shorter path to this node in the frontier, skip.
			if (frontierDistance.find(index) != frontierDistance.end())
			{
				if (frontierDistance[index] <= distanceToStart)
				{
					continue;
				}
			}

			//create a new a star node and add it to the frontier.
			AStarNode node;
			node.breadcrumb = currentNode.position;
			node.position = index;
			node.distanceToStart = distanceToStart;
			node.distance = distance;

			priorityQueue.push(node);
			frontierDistance[node.position] = node.distanceToStart;
		}
	}

	if (currentNode.position == endIndex)
		return std::vector<int>();

	//Reading path from start to end
	std::vector<int> path;
	if (includeStart)
	{
		path.push_back(currentNode.position);
	}

	while (currentNode.position != endIndex && currentNode.position >= 0)
	{
		currentNode = closed[currentNode.breadcrumb];
		path.push_back(currentNode.position);
	}

	return path;
}

std::vector<int> GridAStar::PathFind(int startIndex, int goalIndex, bool includeStart)
{
	return PathFind(ToPoint(startIndex),ToPoint(goalIndex),includeStart);
}

