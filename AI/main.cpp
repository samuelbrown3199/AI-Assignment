#include <iostream>
#include "Neuron.h"

int main()
{
	Neuron test;
	test.threshold = -0.7;

	test.AddInputSignal();
	test.AddInputSignal();
	test.AddInputSignal();

	test.AdjustSignalAtIndex(0, 0.1, 0.7);
	test.AdjustSignalAtIndex(1, 0.5, 0.2);
	test.AdjustSignalAtIndex(2, 0.8, 0.6);

	test.CalculateOutput();

	std::cout << test.xValue << std::endl;
	std::cout << test.SigmoidFunction() << std::endl;

	return 0;
}