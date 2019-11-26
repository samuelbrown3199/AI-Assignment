#pragma once
#ifndef MAZE_H_
#define MAZE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <math.h>

#include <SDL.h>

struct MazeTile
{
	int xPos, yPos;
	int tileType;
};

struct Node
{
	int x, y;

	bool passable = false;
	int xPos, yPos;
	int nodeType;

	float f, h, g;

	Node* parentNode;//could make this an array with size 4 which makes it easier to handle.
	Node* neighbours[4]{nullptr};

	void SetupSuccessors(Node** nodeArray, int col, int row)
	{
		if (y-1 >= 0 && y-1 <= row)
		{
			neighbours[0] = &nodeArray[y - 1][x];
		}
		if (y+1 >= 0 && y+1 <= row)
		{
			neighbours[1] = &nodeArray[y + 1][x];
		}
		if (x-1 >= 0 && x-1 <= col)
		{
			neighbours[2] = &nodeArray[y][x - 1];
		}
		if (x+1 >= 0 && x+1 <= col)
		{
			neighbours[3] = &nodeArray[y][x + 1];
		}
	}

	void CalculateGValue(Node* startNode)
	{
		float distance;

		if (parentNode != nullptr)
		{
			distance = sqrt(((parentNode->xPos - xPos)*(parentNode->xPos - xPos)) + ((parentNode->yPos - yPos)*(parentNode->yPos - yPos)));
		}
		else
		{
			distance = sqrt(((startNode->xPos - xPos)*(startNode->xPos - xPos)) + ((startNode->yPos - yPos)*(startNode->yPos - yPos)));
			parentNode = startNode;
		}
		g = distance;
	}

	void CalculateFValue(Node* startNode)
	{
		CalculateGValue(startNode);
		f = h + g;
	}
};

class Maze
{
private:

	const std::string directory = "Mazes/";
	std::stringstream buffer;

	int numberCount = 0;
	std::string mazeData;
	int* tempArray = nullptr;

	int** mazeArray; 
	MazeTile** mazeTileArray;

public:

	int col, row;

	Node** nodeArray;
	Node* startNode;
	Node* endNode;

	Maze(std::string fileName);
	~Maze();

	void OpenFile(std::string fileName);
	void SplitString(int numCount);
	void OrganiseData();

	void SetupRenderMaze();
	void RenderMaze(SDL_Renderer* _renderer);

	void SetupNodeDistances();
};

#endif // !MAZE_H_