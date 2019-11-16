#pragma once
#ifndef NEURON_H_
#define NEURON_H_

#include <iostream>
#include <vector>
#include <math.h>

class Neuron;

struct InputSignal
{
	int index;
	float value;
	float weight;

	Neuron* previousNeuron = nullptr;

	void InitialiseValues(float _val, float _wei)
	{
		value = _val;
		weight = _wei;
	}
	void AdjustValue(float _val)
	{
		value = _val;
		std::cout << value << std::endl;
	}
	void AdjustWeight(float _wei)
	{
		weight = _wei;
	}
};

struct TrainingData
{
	float input[2];
	float desiredY;

	TrainingData(float x1, float x2, float _desiredY)
	{
		input[0] = x1;
		input[1] = x2;

		desiredY = _desiredY;
	}
};

class Neuron
{
public:

	int neuronIndex;
	int signalCount = 0;

	std::vector<InputSignal> signals;
	float xValue;
	float yValue;
	float threshold;
	float alpha = 0.1f;

	float delta = 0;
	float error = 0;

	TrainingData* data[4]{ nullptr };

	Neuron(int _index, float _alpha, float _threshold);
	~Neuron();

	void CalculateOutput();
	void AddInputSignal(float _weight);
	void AddPreviousNeuron(int input, Neuron* prevNeur);
	void AdjustSignalAtIndex(int i, float val);
	void AdjustSignalWeightAtIndex(int i, float val);
	float GetSignalWeight(int i);
	void GetSignalCount();

	int SignFunction();
	int StepFunction();
	float SigmoidFunction(float e);
	float LinearFunction();

	void TrainingAlgorithm();
};

#endif // !NEURON_H_