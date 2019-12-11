#include "Application.h"



Application::Application(int FPSTarget)
{
	FPS = FPSTarget;
}


Application::~Application()
{
}

int Application::InitialiseApplication(const char* windowName, int posX, int posY, int sizeW, int sizeH, Uint32 flags) //starts up the application with a window and renderer
{
	srand(time(NULL)); //seeds for random numbers

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) //initialises SDL for rendering mazes
	{
		std::cout << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

	window = SDL_CreateWindow(windowName, posX, posY, sizeW, sizeH, flags); //creates a window
	if (window == nullptr)
	{
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC); //creates a renderer for use later
	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_RenderSetLogicalSize(renderer, sizeW, sizeH); //sets up the renderer for the screen size

	ApplicationLoop(); //starts up the application loop
}

void Application::CleanUp() //Cleans up the program for when the program closes.
{
	std::cout << "Application is quitting" << std::endl;

	SDL_DestroyRenderer(renderer); //destroys the renderer
	SDL_DestroyWindow(window); //destroys the window

	SDL_Quit(); //quits SDL
}

void Application::ApplicationLoop() //the application loop
{
	const int frameDelay = 1000 / FPS; //used to slow down the program
	Uint32 frameStart;
	int frameTime;
	int frameCount = 0;

	while (loop) //the while loop
	{
		frameStart = SDL_GetTicks();

		CheckForPlayerGenInput(); //check track of the input from the user
		RenderApplication(); //renders the application

		if (gene != nullptr)
		{
			gene->GeneticLoop(renderer); //handles the genetic loop if the genetic algorithm is in use
		}

		frameTime = SDL_GetTicks() - frameStart; //calculates how long the frame took to process
		if (frameDelay > frameTime) //if frame was too quick then delay
		{
			SDL_Delay(frameDelay - frameTime); //delay so we get a frame limiter to fps
		}
	}
}

SDL_Renderer* Application::GetRenderer()
{
	return renderer; //returns the renderer
}
SDL_Window* Application::GetWindow()
{
	return window; //returns the window
}

void Application::CheckForPlayerGenInput() //used for general input from the user
{
	SDL_Event event; //used to store the current event

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: //the SDL_quit case is used when the close button is used
			loop = false;
			break;

		case SDL_KEYDOWN: //checks for keyboard input
			switch (event.key.keysym.sym)
			{
			case SDLK_n: //press the n key to delete maze
				DeleteMaze();
				break;
			case SDLK_m: //press the m key to create maze
				CreateMaze();
				break;
			}

		default:
			break;
		}
	}
}

void Application::RenderApplication() //the application render function
{
	SDL_RenderClear(renderer); //clears the renderer for rendering the next frame

	if (currentMaze != nullptr)
	{
		currentMaze->RenderMaze(renderer); //renders the maze
	}
	if (gene != nullptr)
	{
		if (gene->chromosomes.size() > 0)
		{
			if (gene->currentChromo < gene->chromosomes.size())
			{
				gene->chromosomes[gene->currentChromo]->RenderChromosome(renderer); //renders the chromosome currently being processed
			}
		}
	}
	if (aStar != nullptr)
	{
		aStar->RenderPath(renderer); //renders the path foo a*
	}

	SDL_SetRenderDrawColor(renderer, 138, 138, 138, 255); //set the background colour to a light grey
	SDL_RenderPresent(renderer); //present the graphics to the renderer
}

void Application::DeleteMaze() //used to delete the current maze
{
	if (currentMaze != nullptr)
	{
		Maze* tempMaze = currentMaze; //store the maze
		if (gene != nullptr)		//delete the relevant AI
		{
			Genetic* tempGene = gene;
			gene = nullptr;
			delete tempGene;
		}
		if (aStar != nullptr)
		{
			AStar* tempA = aStar;
			aStar = nullptr;
			delete tempA;
		}

		currentMaze = nullptr; //nullptr the current maze
		delete tempMaze; //delete the stored maze
	}
}

void Application::CreateMaze() //used to create a maze with AI
{
	std::string fileLoc; //used for inputting the file location
	int aiType; //used to determine the AI

	if (currentMaze == nullptr)
	{
		std::cout << "Enter a file name with ANSI from the maze folder in the program files: ";
		std::cin >> fileLoc; //enter the file name
		currentMaze = new Maze(); //create the maze

		if (currentMaze->LoadData(fileLoc)) //loads the file and checks if it loads successfully
		{
			currentMaze->SetupRenderMaze(); //sets up the renderable maze to render

			std::cout << "Enter the AI type you want to test. 0 = Genetic Algorithm. 1 = A* Pathfinding: ";
			std::cin >> aiType; //used to choose the AI
			while (aiType != 0 && aiType != 1) //checks input was valid
			{
				std::cout << "Please enter a valid number: ";
				std::cin >> aiType;
			}

			if (aiType == 0) //if genetic algorithm is chosen
			{
				int chromosomeCount = 0;
				int geneCount = 0;
				float crossOver = 0;
				float mutationChance = 0;
				int mode = 0;

				std::cout << "Please enter a chromosome count that is a multiple of 4: ";
				std::cin >> chromosomeCount;

				std::cout << "Please enter a gene count that is a multiple of 2: ";
				std::cin >> geneCount;

				std::cout << "Please enter a crossover rate between 0-1: ";
				std::cin >> crossOver;

				std::cout << "Please enter a mutation rate between 0-1: ";
				std::cin >> mutationChance;											//allows the user the enter the desired settings fot the genetic algorithm

				std::cout << "Please enter a mode, details in readme file for what this means: ";
				std::cin >> mode;

				gene = new Genetic(currentMaze, chromosomeCount, geneCount, crossOver, mutationChance, mode); //create the genetic algorithm with the settings
			}
			else if (aiType == 1)
			{
				aStar = new AStar(currentMaze);	//creates the AI for a*
			}
		}
		else
		{
			std::cout << "Couldn't load file: " << fileLoc << ", double check spelling and the file format." << std::endl; //couldn't load the file
			
			Maze* temp = currentMaze;
			currentMaze = nullptr;
			delete temp;			 //clears up memory
		}
	}
}