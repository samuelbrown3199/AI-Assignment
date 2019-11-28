#include "AStar.h"

AStar::AStar(Maze _currentMaze)
{
	currentMaze = &_currentMaze;

	SetupNodes();
	Algorithm();
}

AStar::~AStar()
{
}

void AStar::SetupNodes()
{
	int i = 0;
	for (; i < currentMaze->tileData.size(); i++)
	{
		Node* tempNode = new Node(currentMaze->tileData[i], this);
		nodeList.push_back(*tempNode);
	}

	for (i = 0; i < nodeList.size(); i++)
	{
		nodeList[i].CalculateHValue();
	}

	std::cout << "Setup " << i << " nodes." << std::endl;
}

Node* AStar::FindNodeAtPos(int x, int y)
{
	if (x >= 0 && x <= currentMaze->col)
	{
		if (y >= 0 && y <= currentMaze->row)
		{
			for (int i = 0; i < nodeList.size(); i++)
			{
				if (nodeList[i].x == x && nodeList[i].y == y)
				{
					return &nodeList[i];
					break;
				}
			}
		}
	}
	else
	{
		return nullptr;
	}
}

void AStar::Algorithm() //some form of memory issue
{
	openList.push_back(startNode);
	Node* currentNode;

	int indexOfCurNode = 0;

	while (!pathFound)
	{
		indexOfCurNode = 0;
		currentNode = nullptr;

		for (int i = 0; i < openList.size(); i++)
		{
			if (currentNode == nullptr)
			{
				currentNode = openList[i];
				indexOfCurNode = i;
			}
			else if (currentNode->f < openList[i]->f)
			{
				currentNode = openList[i];
				indexOfCurNode = i;
			}
		}

		openList.erase(openList.begin() + indexOfCurNode);
		closedList.push_back(currentNode);

		if (currentNode == endNode)
		{
			pathFound = true;
			std::cout << "Path is found, loop is over." << std::endl;
			break;
		}

		currentNode->SetupNeighbours();
		for (int i = 0; i < 8; i++)
		{
			if (currentNode->neighbours[i] != nullptr)
			{
				if (currentNode->neighbours[i]->type == 1 || std::count(closedList.begin(), closedList.end(), currentNode->neighbours[i]))
				{
					continue;
				}

				if (currentNode->neighbours[i]->parentNode == nullptr)
				{
					currentNode->neighbours[i]->ChangeParentNode(currentNode);
					currentNode->neighbours[i]->CalculateFValue();

					if (!std::count(openList.begin(), openList.end(), currentNode->neighbours[i]))
					{
						openList.push_back(currentNode->neighbours[i]);
					}
				}
				else
				{
					Node* oldParent = currentNode->neighbours[i]->parentNode;
					float oldF = currentNode->neighbours[i]->f;

					currentNode->neighbours[i]->ChangeParentNode(currentNode);

					if (oldF > currentNode->neighbours[i]->f)
					{
						currentNode->neighbours[i]->CalculateFValue();

						if (!std::count(openList.begin(), openList.end(), currentNode->neighbours[i]))
						{
							openList.push_back(currentNode->neighbours[i]);
						}
					}
					else
					{
						currentNode->neighbours[i]->ChangeParentNode(oldParent);
						currentNode->neighbours[i]->CalculateFValue();

						if (!std::count(openList.begin(), openList.end(), currentNode->neighbours[i]))
						{
							openList.push_back(currentNode->neighbours[i]);
						}
					}
				}
			}
		}
	}
}

