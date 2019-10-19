#include "Maze.h"



Maze::Maze(std::string fileName)
{
	OpenFile(fileName);
}


Maze::~Maze()
{
}

void Maze::OpenFile(std::string fileName)
{
	std::string fileLoc = directory + fileName;
	std::cout << "Looking for maze in file location " << fileLoc << std::endl;


	std::ifstream myFile;
	myFile.open(fileLoc, std::ios::out);

	if (myFile.is_open())
	{
		std::cout << "Found maze" << std::endl;
		buffer << myFile.rdbuf();
		mazeData = buffer.str();

		std::string num;
		while (buffer.good() && buffer >> num)
		{
			numberCount++;
		}
		std::cout << "Maze Data: " << mazeData << ". There are " << numberCount << " numbers" << std::endl;

		myFile.close();
		SplitString(numberCount);
	}
	else
	{
		std::cout << "Couldnt find maze" << std::endl;
	}
}

void Maze::SplitString(int numCount)
{
	std::stringstream data(mazeData);
	int i = 0;

	tempArray = new int[numCount] {0}; //doesnt quite work. will need to be looked into

	while (data.good() && i < numCount)
	{
		data >> tempArray[i];
		i++;
	}

	col = tempArray[0];
	row = tempArray[1];

	for (int i = 0; i < numberCount; i++)
	{
		tempArray[i] = tempArray[i + 2];
	}

	OrganiseData();
}

void Maze::OrganiseData() //not fully working, need to work out how to get 1d array to fit into 2d array
{
	int* cleanArray = new int[numberCount - 2];
	for (int i = 0; i < numberCount - 2; i++)
	{
		cleanArray[i] = tempArray[i];
	}

	mazeArray = new int*[col];
	for (int i = 0; i < col; i++)
	{
		mazeArray[i] = new int[row];
	}

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			mazeArray[y][x] = cleanArray[]; //first two numbers are being put into the maze which is in correct
		}
	}

	for (int y = 0; y < row; y++)
	{
		for (int x = 0; x < col; x++)
		{
			std::cout << mazeArray[y][x] << ' ';
		}
		std::cout << std::endl;
	}
}