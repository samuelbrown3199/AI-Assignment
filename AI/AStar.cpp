#include "AStar.h"



AStar::AStar(Maze* _curMaze)
{
	maze = _curMaze;
	AddStartToOpenList();
	Algorithm();
}


AStar::~AStar()
{
}

void AStar::Algorithm() //not working due to memory issue with successors will need to be debugged quite heavily
{
	std::vector<Node*>::iterator nItr;
	int index = 0, posOfCurNode = 0;

	while (!pathFound)
	{
		for (nItr = openList.begin(); nItr != openList.end(); nItr++, index++)
		{
			if (curNode == nullptr)
			{
				curNode = *nItr;
				posOfCurNode = index;
			}
			if ((*nItr)->f < curNode->f)
			{
				curNode = *nItr;
				posOfCurNode = index;
			}
		}

		openList.erase(openList.begin() + posOfCurNode);
		closedList.push_back(curNode);

		if (curNode->nodeType == 3)
		{
			std::cout << "Path is found, breaking out of loop." << std::endl;
			pathFound = true;
			break;
		}

		for (int i = 0; i < 4; i++)
		{
			if (curNode->neighbours[i] != nullptr)
			{
				if (!curNode->neighbours[i]->passable || std::find(closedList.begin(), closedList.end(), curNode->neighbours[i]) != closedList.end())
				{
					continue;
				}

				if (curNode->neighbours[i]->passable)
				{
					curNode->neighbours[i]->testParent = curNode;
					curNode->neighbours[i]->CalculateTestG();

					curNode->neighbours[i]->CalculateGValue(maze->startNode);

					if (curNode->neighbours[i]->testG <= curNode->neighbours[i]->g)
					{
						if (std::find(openList.begin(), openList.end(), curNode->neighbours[i]) != openList.end())
						{

						}
						else
						{
							curNode->neighbours[i]->parentNode = curNode;
							curNode->neighbours[i]->CalculateFValue(maze->startNode);
							openList.push_back(curNode->neighbours[i]);
							//set f cost of neighbour
							//set parent of neighbour to current
							//if this neighbour is not in open list, add to open list
						}
					}
				}
			}
		}
	}
}

void AStar::AddStartToOpenList()
{
	openList.push_back(maze->startNode);
}