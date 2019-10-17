#pragma once
#ifndef NEURON_H_
#define NEURON_H_

#include <iostream>
#include <math.h>
#include <vector>

struct InputSignal
{
	float value;
	float weight;

	int index;
	
	void InitialiseValues(float _val, float _wei)
	{
		value = _val;
		weight = _wei;
	}
};

class Neuron
{
public:

	float xValue;
	float threshold;
	std::vector<InputSignal> signals;

	Neuron();
	~Neuron();

	void CalculateOutput();
	void AddInputSignal();

	int StepFunction();
	int SignFunction();
	float SigmoidFunction();
	float LinearFunction();

	void AdjustSignalAtIndex(int i, float val, float wei);
};

#endif

