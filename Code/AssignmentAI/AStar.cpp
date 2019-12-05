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
	for (int i = 0; i < currentMaze->tileData.size(); i++)
	{
		Node* tempNode = new Node(currentMaze->tileData[i], this, i);
		nodeList.push_back(*tempNode);
	}

	for (int i = 0; i < nodeList.size(); i++)
	{
		nodeList[i].CalculateHValue();
		nodeList[i].CalculateGValue();

		if (nodeList[i].type == 2)
		{
			startNode = &nodeList[i];
		}
		else if (nodeList[i].type == 3)
		{
			endNode = &nodeList[i];
		}
	}
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
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	openList.reserve(nodeList.size());
	closedList.reserve(nodeList.size());

	openList.push_back(startNode);
	Node* currentNode = startNode;

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
		closedList.push_back(currentNode);
		std::cout << "Pushed back the current node into the closed list, size of closed list is now: " << closedList.size() << std::endl;

		openList[indexOfCurNode] = nullptr;
		openList.erase(openList.begin() + indexOfCurNode);

		if (currentNode == endNode)
		{
			pathFound = true;
			std::cout << "Path is found." << std::endl;
			break;
		}
		else
		{
			currentNode->SetupNeighbours();
			for (int i = 0; i < 8; i++)
			{
				if (currentNode->neighbours[i] != nullptr)
				{
					if (CheckListForNode(&openList, currentNode->neighbours[i]))
					{
						std::cout << "Neighbour was in the open list, continuing." << std::endl;
						continue;
					}
					else
					{
						std::cout << "About to check for the current neighbour in the closed list, the size of the list is currently: " << closedList.size() << std::endl;
						if (CheckListForNode(&closedList, currentNode->neighbours[i])) //size of closed list goes up really high for unknown reason, might consider using lists from now on.
						{
							std::cout << "Neighbour was in the closed list, continuing." << std::endl;
							continue;
						}
						else
						{
							if (currentNode->neighbours[i]->type != 1)
							{
								currentNode->neighbours[i]->CalculateFValue();
								openList.push_back(currentNode->neighbours[i]);
							} 
						}
					}
				}
			}
		}
	}
}

bool AStar::CheckListForNode(std::vector<Node*>* _list, Node* _element)
{

	std::cout << "Open list size: " << (*_list).size() << std::endl;

	std::vector<Node*>::iterator nItr;
	for (nItr = (*_list).begin(); nItr != (*_list).end(); nItr++)
	{
		if (*nItr == _element)
		{
			return true;
			break;
		}
	}
	return false;
}
