#include "Application.h"

#undef main

int main()
{
	int targetFPS = 8;
	std::cout << "Please enter a framerate you want the program to run at: ";
	std::cin >> targetFPS; //allows the user to enter the desired speed of the program
	while (std::cin.fail())
	{
		std::cout << "Please enter a valid number: ";

		std::cin.clear();
		std::cin.ignore(256, '\n');

		std::cin >> targetFPS;
	} //checks input is valid

	const int winX = 1366, winY = 768, xPos = 25, yPos = 25; //sets up values for the window size
	Application app(targetFPS); //creates the application class to handle the program further
	app.InitialiseApplication("AI", xPos, yPos, winX, winY, SDL_WINDOW_SHOWN); //initialises the application

	app.CleanUp(); //cleans up the program

	return 0;
}