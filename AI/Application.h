#pragma once
#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <iostream>
#include <SDL.h>

#include "Maze.h"
#include "AStar.h"

class Application
{
private:

	SDL_Renderer* renderer;
	SDL_Window* window;

	bool loop = true;

	Maze* currentMaze;

public:
	Application();
	~Application();

	int InitialiseApplication(const char* windowName, int posX, int posY, int sizeW, int sizeH, Uint32 flags);
	void CleanUp();

	void ApplicationLoop();

	SDL_Renderer* GetRenderer();
	SDL_Window* GetWindow();

	void CheckForPlayerGenInput();
	void RenderApplication();
	void CreateMaze();
	void DeleteMaze();
};

#endif