#pragma once
#ifndef ASTAR_H_
#define ASTAR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

#include "Maze.h"

struct Node;

class AStar
{
private:

	Maze* currentMaze;

	std::vector<Node> nodeList;
	bool pathFound = false;

public:

	Node* startNode;
	Node* endNode;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	AStar(Maze _currentMaze);
	~AStar();

	void SetupNodes();
	Node* FindNodeAtPos(int x, int y);

	void Algorithm();
};

#endif

struct Node
{
	Node* parentNode = nullptr;
	Node* neighbours[8]{ nullptr };

	int x, y;
	float g, h, f;
	int type;
	int posInList;

	AStar* ai;

	Node()
	{

	}
	Node(MazeTile _tile, AStar* _ai, int i)
	{
		x = _tile.x;
		y = _tile.y;

		type = _tile.type;
		posInList = i;

		ai = _ai;

		if (type == 2)
		{
			ai->startNode = this;
			g = 0;
			std::cout << "Found start node, adding pointer to it." << std::endl;
		}
		else if (type == 3)
		{
			ai->endNode = this;
			std::cout << "Found end node, adding pointer to it." << std::endl;
		}
	}

	void SetupNeighbours()
	{
		neighbours[0] = ai->FindNodeAtPos(x - 1, y - 1);
		neighbours[1] = ai->FindNodeAtPos(x, y - 1);
		neighbours[2] = ai->FindNodeAtPos(x + 1, y - 1);
		neighbours[3] = ai->FindNodeAtPos(x + 1, y);
		neighbours[4] = ai->FindNodeAtPos(x + 1, y + 1);
		neighbours[5] = ai->FindNodeAtPos(x, y + 1);
		neighbours[6] = ai->FindNodeAtPos(x - 1, y + 1);
		neighbours[7] = ai->FindNodeAtPos(x - 1, y);
	}

	void CalculateFValue()
	{
		f = h + g;
	}
	void CalculateHValue()
	{
		float distance = sqrt(((ai->endNode->x - x)*(ai->endNode->x - x)) + ((ai->endNode->y - y)*(ai->endNode->y - y)));
		h = distance;
	}
	void CalculateGValue()
	{
		if (this != ai->startNode)
		{
			float distance = sqrt(((ai->startNode->x - x)*(ai->startNode->x - x)) + ((ai->startNode->y - y)*(ai->startNode->y - y)));
			g = distance;
		}
		else
		{
			g = 0;
		}
	}

	void ChangeParentNode(Node* _newNode)
	{
		parentNode = _newNode;
	}
};
