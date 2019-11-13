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
	bool passable = false;
	int xPos, yPos;
	float distanceToEnd;
	float distanceFromCurPos;
};

class Maze
{
private:

	const std::string directory = "Mazes/";
	std::stringstream buffer;

	int numberCount = 0;
	std::string mazeData;
	int* tempArray = nullptr;
	
	int col, row;
	int** mazeArray;

	MazeTile** mazeTileArray;

public:

	Node** nodeArray;

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