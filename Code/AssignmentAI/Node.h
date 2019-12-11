#pragma once
#ifndef NODE_H_
#define NODE_H_

#include <math.h>
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "AStar.h"

class AStar;

class Node
{
private:

	AStar* ai;

public:

	int posInList;

	int x, y, type;
	float f, g, h;

	Node* parent;
	Node* neighbours[8];
	
	SDL_Rect nodeRect;

	Node();
	Node(int _x, int _y, int _type, int i, AStar* _ai);
	~Node();

	void SetupNeighbours();
	void ChangeParent(Node* _newParent);
	void CalculateCost();

	int GetDistanceBetweenNodes(Node* a);
};

#endif

