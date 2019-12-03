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
		Node* tempNode = new Node(currentMaze->tileData[i], this, i);
		nodeList.push_back(*tempNode);
	}

	for (i = 0; i < nodeList.size(); i++)
	{
		nodeList[i].CalculateHValue();
		nodeList[i].CalculateGValue();
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

void AStar::Algorithm()
{
	openList.push_back(startNode);
	Node* currentNode;

	int indexOfCurNode = 0;

	while (!pathFound)
	{
		currentNode = nullptr;
		std::vector<Node*>::iterator nItr;
		int i = 0;

		for (nItr = openList.begin(); nItr != openList.end(); nItr++)
		{
			if (*nItr != nullptr)
			{
				if (currentNode == nullptr)
				{
					currentNode = *nItr;
					indexOfCurNode = i;
				}
				else if (currentNode->f < (*nItr)->f)
				{
					currentNode = *nItr;
					indexOfCurNode = i;
				}
			}
			i++;
		}

		openList[i-1] = nullptr;
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
				std::vector<Node*>::iterator searchItr;
				searchItr = std::find(closedList.begin(), closedList.end(), currentNode->neighbours[i]);
				if (currentNode->neighbours[i]->type == 1 || searchItr != closedList.end())
				{
					continue;
				}

				if (currentNode->neighbours[i]->parentNode == nullptr)
				{
					currentNode->neighbours[i]->ChangeParentNode(currentNode);
					currentNode->neighbours[i]->CalculateFValue();

					searchItr = std::find(openList.begin(), openList.end(), currentNode->neighbours[i]);
					if (searchItr == openList.end())
					{
						openList.push_back(currentNode->neighbours[i]);
					}
				}
				/*else
				{
					Node* oldParent = currentNode->neighbours[i]->parentNode;
					float oldF = currentNode->neighbours[i]->f;

					currentNode->neighbours[i]->ChangeParentNode(currentNode);
					currentNode->neighbours[i]->CalculateFValue();

					if (oldF > currentNode->neighbours[i]->f)
					{
						searchItr = std::find(openList.begin(), openList.end(), currentNode->neighbours[i]);
						if (searchItr == openList.end())
						{
							openList.push_back(currentNode->neighbours[i]);
						}
					}
					else
					{
						currentNode->neighbours[i]->ChangeParentNode(oldParent);
						currentNode->neighbours[i]->CalculateFValue();

						searchItr = std::find(openList.begin(), openList.end(), currentNode->neighbours[i]);
						if (searchItr == openList.end())
						{
							openList.push_back(currentNode->neighbours[i]);
						}
					}
				}*/
			}
		}
	}
}

