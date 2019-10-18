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

	std::string directory = "Mazes/";
	std::stringstream buffer;

	std::string mazeData;
	std::string* mazeSplitData;

public:
	Maze();
	~Maze();

	void OpenFile(std::string fileName);
	void SplitString(int numCount);
};

#endif // !MAZE_H_