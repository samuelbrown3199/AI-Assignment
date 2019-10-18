#include <iostream>

#include "Neuron.h"
#include "Maze.h"

int main()
{
	Neuron test;
	test.threshold = -0.7;

	test.AddInputSignal(0.1, 0.7);
	test.AddInputSignal(0.5, 0.2);
	test.AddInputSignal(0.8, 0.6);

	test.CalculateOutput();

	std::cout << test.xValue << std::endl;
	std::cout << test.SigmoidFunction() << std::endl;


	Maze maze;
	maze.OpenFile("TestMazeA.txt");

	return 0;
}