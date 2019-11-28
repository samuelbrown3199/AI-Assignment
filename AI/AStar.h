#pragma once
#ifndef ASTAR_H_
#define ASTAR_H_

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

#include "Maze.h"

class AStar
{
private:

	Maze* maze;

	std::vector<Node*> openList; //look into using lists rather than vectors
	std::vector<Node*> closedList;
	Node* curNode;

	std::vector<Node*> route;
	bool pathFound = false;

public:
	AStar(Maze* _curMaze);
	~AStar();

	void Algorithm();
	void AddStartToOpenList();
};

#endif

