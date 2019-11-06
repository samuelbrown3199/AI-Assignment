#include "Maze.h"



Maze::Maze(std::string fileName) //maze constructor takes a string for finding the maze file
{
	OpenFile(fileName);
}


Maze::~Maze()
{
	delete mazeArray;
	delete mazeTileArray;
}

void Maze::OpenFile(std::string fileName) //function used to find the maze files
{
	std::string fileLoc = directory + fileName; //creates a string to use for searching for the maze.
	std::cout << "Looking for maze in file location " << fileLoc << std::endl;


	std::ifstream myFile; //use an ifstream as code only needs to read from file
	myFile.open(fileLoc, std::ios::out); //open the file

	if (myFile.is_open()) //if the maze file is found,
	{
		std::cout << "Found maze" << std::endl;
		buffer << myFile.rdbuf();
		mazeData = buffer.str(); //put maze data into a string

		std::string num;
		while (buffer.good() && buffer >> num)
		{
			numberCount++; //get a count for the amount of numbers in the maze data. used for later dynamically allocating an int array
		}

		std::cout << "Maze data: " << buffer.str() << std::endl;

		myFile.close(); //close the file
		SplitString(numberCount); //call the split string function
	}
	else
	{
		std::cout << "Couldnt find maze" << std::endl; //file is not found close the stream and print a warning
		myFile.close();
	}
}

void Maze::SplitString(int numCount) //function to sort the data into a 1D int array
{
	std::stringstream data(mazeData);
	int i = 0;

	tempArray = new int[numCount] {0}; //initialises 1D array to use for later

	std::cout << "Temp Array Data: ";
	while (data.good() && i < numCount)
	{
		data >> tempArray[i]; //gets the data from the stream and puts into the array at index I
		std::cout << tempArray[i];
		i++;
	}
	std::cout << std::endl;

	col = tempArray[0]; //gets the first two numbers, used for determining the amount of rows and columns in the maze
	row = tempArray[1];


	std::cout << "Temp Array Data: ";
	for (int i = 0; i < numberCount - 2; i++)
	{
		tempArray[i] = tempArray[i + 2]; //overwrites the array without the needed first 2 numbers
		std::cout << tempArray[i];
	}
	std::cout << std::endl;

	OrganiseData();
}

void Maze::OrganiseData() //function to sort 1D array into a 2D array
{
	std::cout << "Should create maze with " << col << " columns and " << row << " rows" << std::endl;

	std::cout << "Clean Array Data: ";
	int* cleanArray = new int[numberCount - 2];
	for (int i = 0; i < numberCount - 2; i++)
	{
		cleanArray[i] = tempArray[i]; //gets rid of the first two numbers of the temp array. the first two numbers are not needed for the 2d maze array later.
		std::cout << cleanArray[i];
	}
	std::cout << std::endl;
	delete tempArray;

	mazeArray = new int*[col];
	for (int i = 0; i < col; i++)
	{
		mazeArray[i] = new int[row]; //initialises the 2d maze array with the right amount of columns and rows.
	}

	int i = 0;
	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			mazeArray[y][x] = cleanArray[i++]; //this loops puts the array into a 2d int array for use later
		}
	}

	SetupRenderMaze();
}

void Maze::SetupRenderMaze()
{
	mazeTileArray = new MazeTile*[col];
	for (int i = 0; i < col; i++)
	{
		mazeTileArray[i] = new MazeTile[row]; //initialises the 2d maze array with the right amount of columns and rows.
	}

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			mazeTileArray[y][x].tileType = mazeArray[y][x];
			mazeTileArray[y][x].xPos = 0 + (x * 50);
			mazeTileArray[y][x].yPos = 0 + (y * 50);
		}
	}
}

void Maze::RenderMaze(SDL_Renderer* _renderer)
{	
	if (mazeTileArray != nullptr)
	{
		for (int y = 0; y < row; y++)
		{
			for (int x = 0; x < col; x++)
			{
				SDL_Rect tempRect;
				tempRect.x = mazeTileArray[y][x].xPos;
				tempRect.y = mazeTileArray[y][x].yPos;
				tempRect.w = 50;
				tempRect.h = 50;

				switch (mazeTileArray[y][x].tileType)
				{
				case 0:
					SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
					break;
				case 3:
					SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
					break;
				}

				SDL_RenderFillRect(_renderer, &tempRect);
			}
		}
	}
}