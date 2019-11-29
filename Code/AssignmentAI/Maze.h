#pragma once
#ifndef MAZE_H_
#define MAZE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <SDL.h>

struct MazeTile
{
	int type;
	int x, y;
	SDL_Rect tileRect;

	MazeTile(int _type, int _x, int _y)
	{
		type = _type;
		x = _x;
		y = _y;

		tileRect.x = 0 + (50 * x);
		tileRect.y = 0 + (50 * y);
		tileRect.w = 50;
		tileRect.h = 50;
	}
};

class Maze
{
private:

	std::vector<int> mazeData;

public:

	int col, row;
	std::vector<MazeTile> tileData;
	int startX, startY, endX, endY;

	Maze(std::string fileName);
	~Maze();

	void LoadData(std::string fileName);
	void SetupRenderMaze();
	void RenderMaze(SDL_Renderer* _renderer);

	MazeTile FindTileAtPos(int _y, int _x);
};

#endif

