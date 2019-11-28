#include "Application.h"

#undef main

int main()
{
	const int winX = 1366, winY = 768, xPos = 25, yPos = 25;
	Application app;
	app.InitialiseApplication("AI", xPos, yPos, winX, winY, SDL_WINDOW_SHOWN);

	app.CleanUp();

	return 0;
}