#include "Maze.h"

Maze::Maze()
{
}

Maze::~Maze()
{
}

bool Maze::LoadData(std::string fileName) //used to load the data into something usable in the program
{
	std::string fileLoc = "Mazes/" + fileName; //sets up the directory for the maze file location
	std::ifstream myFile; //sets up the file stream to load the data
	myFile.open(fileLoc); //opens the file and stores it in the stream

	std::stringstream buffer; //buffer will be used to read data into

	if (myFile.is_open()) //checks opening the file was successful
	{
		buffer << myFile.rdbuf(); //reads the file into the buffer

		std::string num; //used to loop through the numbers in the file and add to vector
		while (buffer.good() && buffer >> num) //while theres data to read in the file, continue
		{
			mazeData.push_back(std::stoi(num)); //push into the vector the numbers
		}

		col = mazeData[0]; //get the column count from the vector
		row = mazeData[1]; //get the row count from the vector
		mazeData.erase(mazeData.begin(), mazeData.begin() + 2); //erase column and row count from the vector
		
		std::cout << "There are " << col << " columns and " << row << " rows. The data for the maze is as follows: " << std::endl;

		std::vector<int>::iterator itr;
		for (itr = mazeData.begin(); itr != mazeData.end(); itr++)
		{
			std::cout << *itr; //loops through the vector to print out the data
		}
		std::cout << std::endl;

		myFile.close(); //closes the file stream
		return true;
	}
	else
	{
		myFile.close(); //the file didnt open so close the stream
		return false;
	}
}

void Maze::SetupRenderMaze() //used to setup the maze to render later
{
	int i = 0; //used to represent the 1d vector into a 2d grid
	for (int y = 0; y < row; y++) //loop through the rows
	{
		for (int x = 0; x < col; x++) //loop through the columns
		{
			MazeTile temp(mazeData[i], x, y); //create a new tile with the right data
			tileData.push_back(temp); //push the new tile into the tile vector

			if (mazeData[i] == 2)
			{
				startX = x; //used to record the start position of the maze
				startY = y;
			}
			else if (mazeData[i] == 3)
			{
				endX = x; //used to record the end position of the mazes
				endY = y;
			}

			i++; //increment i to move on
		}
	}

	std::cout << "Setup " << i << " tiles from the maze data." << std::endl;
}

void Maze::RenderMaze(SDL_Renderer* _renderer) //function used to render the maze
{
	int i = 0; //used to represent the 1d vector into a 2d grid 
	for (int y = 0; y < row; y++) //loop through the rows
	{
		for (int x = 0; x < col; x++) //loop through the columns
		{
			switch (tileData[i].type)
			{
			case 0:
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255); //if the tile is an empty space render in white
				break;
			case 1:
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); //if the tile is a wall, render in black
				break;
			case 2:
				SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255); // if the tile is the start tile, render in green
				break;
			case 3:
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255); //if the tile is the end, render in red
				break;
			}

			SDL_RenderFillRect(_renderer, &tileData[i].tileRect); //fill the tile position with the relevant colour
			i++; //increment I
		}
	}
}

MazeTile Maze::FindTileAtPos(int _x, int _y) //used to find a tile at an x or y position as its not stored in 2d vector
{
	if (_y >= 0 && _y < row) //checks if the requested y is in the maze
	{
		if (_x >= 0 && _x < col) //checks if the requested x is actually in the maze
		{
			for (int i = 0; i < tileData.size(); i++) //loops through the tile list
			{
				if (tileData[i].x == _x && tileData[i].y == _y) //if the tile x and y is the requested x and y
				{
					return tileData[i]; //return this tile
					break; //break the loop
				}
			}
		}
	}
}