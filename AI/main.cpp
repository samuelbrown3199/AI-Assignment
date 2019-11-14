#include <iostream>

#include "Application.h"
#include "Maze.h"

#include "Neuron.h" //temp for testing

#undef main

int main()
{
	//temp Code start

	Neuron test;							
	test.threshold = 0.2f;
	test.alpha = 0.1f;

	test.type = Neuron::NeuronType::step;
	test.AddInputSignal();
	test.AddInputSignal();

	test.signals[0].InitialiseValues(0, 0.3f);
	test.signals[1].InitialiseValues(0, -0.1f);

	test.dataTemp[0].x1 = 0;
	test.dataTemp[0].x2 = 0;
	test.dataTemp[0].yd = 0;

	test.dataTemp[1].x1 = 0;
	test.dataTemp[1].x2 = 1;
	test.dataTemp[1].yd = 0;

	test.dataTemp[2].x1 = 1;
	test.dataTemp[2].x2 = 0;
	test.dataTemp[2].yd = 0;

	test.dataTemp[3].x1 = 1;
	test.dataTemp[3].x2 = 1;
	test.dataTemp[3].yd = 1;

	test.TrainingAlgorithmTemp();

	//temp code end



	const int winX = 1366, winY = 768, xPos = 25, yPos = 25;
	Application app;
	app.InitialiseApplication("AI", xPos, yPos, winX, winY, SDL_WINDOW_SHOWN);

	app.CleanUp();
	return 0;
}