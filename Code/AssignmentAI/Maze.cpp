#include "Maze.h"

Maze::Maze(std::string fileName)
{
	LoadData(fileName);
}

Maze::~Maze()
{
}

void Maze::LoadData(std::string fileName)
{
	std::string fileLoc = "Mazes/" + fileName;
	std::ifstream myFile;
	myFile.open(fileLoc);

	std::stringstream buffer;

	if (myFile.is_open())
	{
		buffer << myFile.rdbuf();

		std::string num;
		while (buffer.good() && buffer >> num)
		{
			mazeData.push_back(std::stoi(num));
		}

		col = mazeData[0];
		row = mazeData[1];
		mazeData.erase(mazeData.begin(), mazeData.begin() + 2);
		
		std::cout << "There are " << col << " columns and " << row << " rows. The data for the maze is as follows: " << std::endl;

		std::vector<int>::iterator itr;
		for (itr = mazeData.begin(); itr != mazeData.end(); itr++)
		{
			std::cout << *itr;
		}
		std::cout << std::endl;

		myFile.close();

		SetupRenderMaze();
	}
	else
	{
		myFile.close();
	}
}

void Maze::SetupRenderMaze()
{
	int i = 0;
	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			MazeTile temp(mazeData[i], x, y);
			tileData.push_back(temp);

			i++;
		}
	}

	std::cout << "Setup " << i << " tiles from the maze data." << std::endl;
}

void Maze::RenderMaze(SDL_Renderer* _renderer)
{
	int i = 0;
	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			switch (tileData[i].type)
			{
			case 0:
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				break;
			case 1:
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				break;
			case 2:
				SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
				break;
			case 3:
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
				break;
			}

			SDL_RenderFillRect(_renderer, &tileData[i].tileRect);
			i++;
		}
	}
}
