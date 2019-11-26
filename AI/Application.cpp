#include "Application.h"

Application::Application()
{
	std::cout << "Starting application" << std::endl;
}

Application::~Application()
{
	std::cout << "Closing application" << std::endl;
}

int Application::InitialiseApplication(const char* windowName, int posX, int posY, int sizeW, int sizeH, Uint32 flags)
{
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
	CreateMaze();

	while (loop)
	{
		RenderApplication();
		//CheckForPlayerGenInput();
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
		/*case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_m)
			{
				CreateMaze();
				break;
			}
			else if(event.key.keysym.sym == SDLK_n)
			{
				DeleteMaze();
				break;
			}*/
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

	SDL_SetRenderDrawColor(renderer, 138, 138, 138, 255);
	SDL_RenderPresent(renderer);
}

void Application::CreateMaze()
{
	if (currentMaze == nullptr)
	{
		std::cout << "Enter a maze file name : ";

		std::string fileName;
		std::cin >> fileName;

		currentMaze = new Maze(fileName);
		AStar* ai = new AStar(currentMaze);
	}
	else
	{
		std::cout << "There is already a maze, consider deleting using the N key" << std::endl;
	}
}

void Application::DeleteMaze()
{
	if (currentMaze != nullptr)
	{
		delete currentMaze;
	}
	else
	{
		std::cout << "No maze to delete" << std::endl;
	}
}