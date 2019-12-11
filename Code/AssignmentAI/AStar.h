#pragma once
#ifndef ASTAR_H_
#define ASTAR_H_

#include <iostream>
#include <vector>
#include <SDL.h>
#include <chrono>

#include "Maze.h"
#include "Node.h"

class Node;

class AStar
{
private:

	Maze* currentMaze;
	std::vector<Node*> nodeList;
	int nodeAmount;

	bool pathFound = false;

public:

	Node* startNode;
	Node* endNode;

	AStar(Maze* _maze);
	~AStar();

	void SetupNodeList();
	Node* FindNode(int _x, int _y);

	void Algorithm();
	void RenderPath(SDL_Renderer* _renderer);
};

#endif

