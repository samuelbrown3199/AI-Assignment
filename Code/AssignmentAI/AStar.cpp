#include "AStar.h"



AStar::AStar(Maze* _maze)
{
	currentMaze = _maze; //keeps a pointer of the current maze
	nodeAmount = currentMaze->tileData.size(); //works out the amount of nodes in the maze

	SetupNodeList(); //sets up the node data
	Algorithm(); //starts the algorithm
}

AStar::~AStar()
{
}

void AStar::SetupNodeList()
{
	for (int i = 0; i < nodeAmount; i++) //loops through the amount of nodes
	{
		Node* temp = new Node(currentMaze->tileData[i].x, currentMaze->tileData[i].y, currentMaze->tileData[i].type, i, this); //pushes a node with the relevant data into the list
		nodeList.push_back(temp);

		if (temp->type == 2)
		{
			startNode = temp; //keeps a pointer of the start node
		}
		else if (temp->type == 3)
		{
			endNode = temp; //keeps a pointer of the end node
		}
	}
}

Node* AStar::FindNode(int _x, int _y) //finds a node with an x and y value
{
	for (int i = 0; i < nodeAmount; i++) //loops through the node list
	{
		if (nodeList[i]->x == _x && nodeList[i]->y == _y) //if the values are correct we have found the node
		{
			return nodeList[i];
		}
	}

	return nullptr; //if we get through the full list the node doesnt exist
}

void AStar::Algorithm() //the a* algorithm
{
	auto t1 = std::chrono::high_resolution_clock::now();

	std::vector<Node*> openList; //creates the open list
	std::vector<Node*> closedList; //creates the closed list

	for (int i = 0; i < nodeAmount; i++) //pushes back some nullptrs into the closed and open lists
	{
		openList.push_back(nullptr);
		closedList.push_back(nullptr);
	}

	Node* currentNode; //used to store the current node
	openList[startNode->posInList] = startNode; //puts the start node into the openlist

	while (!pathFound) //while the path is found continue
	{
		currentNode = nullptr; //sets the current node to nullptr

		for (int i = 0; i < nodeAmount; i++) //loop through the node lists
		{
			if (openList[i] != nullptr) //if the current open list element is nullptr
			{
				if (currentNode == nullptr) //if the current node is null just set the value to the first non-null openlist element
				{
					currentNode = openList[i];
				}
				else if (currentNode->f < openList[i]->f) //if the f cost is lower than the current node, this is better and we will explore this first
				{
					currentNode = openList[i];
				}
			}
		}

		openList[currentNode->posInList] = nullptr; //remove the current node from the list
		closedList[currentNode->posInList] = currentNode; //add the current node to the closed list

		if (currentNode->type == 3) //if the currentnode is the endnode the path is found and the loop is broken
		{
			std::cout << "Found the end of the path, breaking loop." << std::endl;
			pathFound = true;

			auto t2 = std::chrono::high_resolution_clock::now();
			std::cout << "Finding the path took: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " milliseconds." << std::endl;

			break;
		}
		else //path is not found, we will explore the neighbours
		{
			currentNode->SetupNeighbours(); //sets up the node neighbours
			for (int i = 0; i < 8; i++) //loop through the neighbours
			{
				if (currentNode->neighbours[i] != nullptr) //if the neighbour isnt null continue
				{
					if (openList[currentNode->neighbours[i]->posInList] != nullptr) //ifs its in the open list ignore it
					{
						continue;
					}
					else
					{
						if (closedList[currentNode->neighbours[i]->posInList] != nullptr) //if its in the closed list we ignore it
						{
							continue;
						}
						else
						{
							if (currentNode->neighbours[i]->type != 1) //if the node is a wall/obstacle we ignore it
							{
								if (currentNode->neighbours[i]->parent == nullptr)
								{
									currentNode->neighbours[i]->ChangeParent(currentNode); //we set the nodes parent to the current node
									currentNode->neighbours[i]->CalculateCost(); //we calculate the cost of the node

									openList[currentNode->neighbours[i]->posInList] = currentNode->neighbours[i]; //we put the neighbour into the openList
								}
							}
						}
					}
				}
			}
		}
	}
}

void AStar::RenderPath(SDL_Renderer* _renderer) //used to render the finished path to the screen
{
	Node* currentNode; //used to store current node
	currentNode = endNode; //start at the end node
	
	SDL_SetRenderDrawColor(_renderer, 0, 100, 100, 255); //render the start node
	SDL_RenderFillRect(_renderer, &startNode->nodeRect);

	while (currentNode->parent != nullptr) //while the current nodes parent does exist we render this node 
	{
		SDL_SetRenderDrawColor(_renderer, 0, 100, 100, 255);
		SDL_RenderFillRect(_renderer, &currentNode->nodeRect);

		currentNode = currentNode->parent; //make the current node equal to the parent of the currentnode
	}
}