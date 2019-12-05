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
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	for (int i = 0; i < nodeAmount; i++)
	{
		openList.push_back(nullptr);
		closedList.push_back(nullptr);
	}

	Node* currentNode;
	openList[startNode->posInList] = startNode;

	while (!pathFound)
	{
		currentNode = nullptr;

		for (int i = 0; i < nodeAmount; i++)
		{
			if (openList[i] != nullptr)
			{
				if (currentNode == nullptr)
				{
					currentNode = openList[i];
				}
				else if (currentNode->f < openList[i]->f)
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
							if (currentNode->neighbours[i]->type != 1)
							{
								currentNode->neighbours[i]->ChangeParent(currentNode); //fcost needs to be calculated better, as currently the g cost is the distance from the start node to the neighbour node, which isnt particularly great
								currentNode->neighbours[i]->CalculateCost();

								openList[currentNode->neighbours[i]->posInList] = currentNode->neighbours[i];
							}
						}
					}
				}
			}
		}
	}
}

void AStar::RenderPath(SDL_Renderer* _renderer)
{
	Node* currentNode;
	currentNode = endNode;

	while (currentNode->parent != nullptr)
	{
		SDL_SetRenderDrawColor(_renderer, 0, 100, 100, 255);
		SDL_RenderFillRect(_renderer, &currentNode->nodeRect);

		currentNode = currentNode->parent;
	}
}
