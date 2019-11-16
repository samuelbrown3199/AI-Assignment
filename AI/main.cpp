#include <iostream>

#include "Application.h"
#include "Maze.h"

#include "Network.h"

#undef main

int main()
{
	//temp Code start
	
	Network net;

	//temp code end



	const int winX = 1366, winY = 768, xPos = 25, yPos = 25;
	Application app;
	app.InitialiseApplication("AI", xPos, yPos, winX, winY, SDL_WINDOW_SHOWN);

	app.CleanUp();
	return 0;
}