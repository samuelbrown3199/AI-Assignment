#include "Maze.h"



Maze::Maze()
{
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

		int numberCount = 0;
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

	mazeSplitData = new std::string[numCount]; //doesnt quite work. will need to be looked into

	while (data.good() && i < numCount)
	{
		data >> mazeSplitData[i];
		i++;
	}

	for (int i = 0; i < mazeSplitData->length(); i++)
	{
		std::cout << mazeSplitData[i] << std::endl;
	}
}