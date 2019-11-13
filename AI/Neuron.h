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
	void AdjustValue(float _val)
	{
		value = _val;
	}
	void AdjustWeight(float _wei)
	{
		weight = _wei;
	}
};

struct TrainingData
{
	float x1, x2, yd;
};

class Neuron
{
private:


public:

	enum NeuronType
	{
		step,
		sign,
		sigmoid,
		linear
	};

	float xValue;
	float yValue;
	float threshold;
	float alpha = 0.1f;

	std::vector<InputSignal> signals;
	NeuronType type;
	TrainingData dataTemp[4];

	Neuron();
	~Neuron();

	void CalculateXValue();
	void AddInputSignal();
	void AddInputSignal(float _val, float _wei);

	int StepFunction();
	int SignFunction();
	float SigmoidFunction();
	float LinearFunction();

	void AdjustSignalAtIndex(int i, float val);
	void TrainWeights();

	void TrainingAlgorithmTemp();
};

#endif

