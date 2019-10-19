#pragma once
#ifndef MAZE_H_
#define MAZE_H_

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class Maze
{
private:

	const std::string directory = "Mazes/";
	std::stringstream buffer;

	int numberCount = 0;
	std::string mazeData;
	int* tempArray = nullptr;
	
	int col, row;
	int** mazeArray;

public:
	Maze(std::string fileName);
	~Maze();

	void OpenFile(std::string fileName);
	void SplitString(int numCount);
	void OrganiseData();
};

#endif // !MAZE_H_