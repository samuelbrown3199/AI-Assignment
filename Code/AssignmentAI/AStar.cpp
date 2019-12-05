#include "AStar.h"



AStar::AStar(Maze* _maze)
{
	currentMaze = _maze;
	nodeAmount = currentMaze->tileData.size();

	SetupNodeList();
	Algorithm();
}

AStar::~AStar()
{
}

void AStar::SetupNodeList()
{
	for (int i = 0; i < nodeAmount; i++)
	{
		Node* temp = new Node(currentMaze->tileData[i].x, currentMaze->tileData[i].y, currentMaze->tileData[i].type, i, this);
		nodeList.push_back(temp);

		if (temp->type == 2)
		{
			startNode = temp;
		}
		else if (temp->type == 3)
		{
			endNode = temp;
		}
	}
}

Node* AStar::FindNode(int _x, int _y)
{
	for (int i = 0; i < nodeAmount; i++)
	{
		if (nodeList[i]->x == _x && nodeList[i]->y == _y)
		{
			return nodeList[i];
		}
	}

	return nullptr;
}

void AStar::Algorithm()
{
	Node** openList = new Node*[nodeAmount]; //not allocating properly
	Node** closedList = new Node*[nodeAmount];

	Node* currentNode;
	openList[startNode->posInList] = startNode;

	while (!pathFound)
	{
		currentNode = nullptr;

		for (int i = 0; i < nodeAmount; i++)
		{
			if (currentNode != nullptr)
			{
				if (openList[i] != nullptr)
				{
					currentNode = openList[i];
				}
				else if (currentNode->f <= openList[i]->f)
				{
					currentNode = openList[i];
				}
			}
		}
		openList[currentNode->posInList] = nullptr;
		closedList[currentNode->posInList] = currentNode;

		if (currentNode->type == 3)
		{
			std::cout << "Found the end of the path, breaking loop." << std::endl;
			pathFound = true;
			break;
		}
		else
		{
			currentNode->SetupNeighbours();
			for (int i = 0; i < 8; i++)
			{
				if (currentNode->neighbours[i] != nullptr)
				{
					if (openList[currentNode->neighbours[i]->posInList] != nullptr)
					{
						continue;
					}
					else
					{
						if (closedList[currentNode->neighbours[i]->posInList] != nullptr)
						{
							continue;
						}
						else
						{
							currentNode->neighbours[i]->ChangeParent(currentNode);
							currentNode->neighbours[i]->CalculateCost();
							openList[currentNode->neighbours[i]->posInList] = currentNode->neighbours[i];
						}
					}
				}
			}
		}
	}
}
