#include "Application.h"



Application::Application()
{
}


Application::~Application()
{
}

int Application::InitialiseApplication(const char* windowName, int posX, int posY, int sizeW, int sizeH, Uint32 flags)
{
	srand(time(NULL));

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Failed to initialise SDL: " << SDL_GetError() << std::endl;
		return -1;
	}

	window = SDL_CreateWindow(windowName, posX, posY, sizeW, sizeH, flags);
	if (window == nullptr)
	{
		std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
		return -1;
	}

	SDL_RenderSetLogicalSize(renderer, sizeW, sizeH);

	currentMaze = new Maze("TestMazeA_ANSI.txt"); //temporary until i add more control for the users
	//aStar = new AStar(*currentMaze);
	gene = new Genetic(currentMaze);

	ApplicationLoop();
}

void Application::CleanUp()
{
	std::cout << "Application is quitting" << std::endl;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Application::ApplicationLoop()
{
	const int FPS = 8; //used in frame limiter
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
	int frameCount = 0;

	while (loop)
	{
		frameStart = SDL_GetTicks();

		CheckForPlayerGenInput();
		RenderApplication();

		if (gene != nullptr)
		{
			gene->GeneticLoop(renderer);
		}

		frameTime = SDL_GetTicks() - frameStart; //calculates how long the frame took to process
		if (frameDelay > frameTime) //if frame was too quick then delay
		{
			SDL_Delay(frameDelay - frameTime); //delay so we get a frame limiter to 60 fps
		}
	}
}

SDL_Renderer* Application::GetRenderer()
{
	return renderer;
}
SDL_Window* Application::GetWindow()
{
	return window;
}

void Application::CheckForPlayerGenInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			loop = false;
			break;
		default:
			break;
		}
	}
}

void Application::RenderApplication()
{
	SDL_RenderClear(renderer);

	if (currentMaze != nullptr)
	{
		currentMaze->RenderMaze(renderer);
	}
	if (gene != nullptr)
	{
		if (gene->chromosomes.size() > 0)
		{
			if (gene->currentChromo < gene->chromosomes.size())
			{
				gene->chromosomes[gene->currentChromo]->RenderChromosome(renderer);
			}
		}
	}

	SDL_SetRenderDrawColor(renderer, 138, 138, 138, 255);
	SDL_RenderPresent(renderer);
}