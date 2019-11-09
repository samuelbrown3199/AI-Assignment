#include "Neuron.h"



Neuron::Neuron()
{
}


Neuron::~Neuron()
{
}

void Neuron::CalculateXValue()
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		xValue += (*sItr).value*(*sItr).weight;
	}
}

void Neuron::AddInputSignal()
{
	InputSignal newSig;

	newSig.index = signals.size();
	signals.push_back(newSig);
}

void Neuron::AddInputSignal(float _val, float _wei)
{
	InputSignal newSig;
	newSig.InitialiseValues( _val, _wei);

	newSig.index = signals.size();
	signals.push_back(newSig);
}

int Neuron::StepFunction()
{
	if (xValue >= threshold)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int Neuron::SignFunction()
{
	if (xValue >= threshold)
	{
		return 1;
	}
	else
	{
		return -1;
	}
}

float Neuron::SigmoidFunction()
{
	float e = 2.71828;
	return 1 / (1 + pow(e, -(xValue - threshold)));
}

float Neuron::LinearFunction()
{
	return xValue - threshold;
}

void Neuron::AdjustSignalAtIndex(int i, float val)
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		if ((*sItr).index == i)
		{
			(*sItr).value = val;
			break;
		}
	}
}

void Neuron::TrainWeights() //not quite working, refer to lab 4 on brightspace
{
	for (int i = 0; i < 4; i++)
	{
		AdjustSignalAtIndex(0, dataTemp[i].x1);
		AdjustSignalAtIndex(0, dataTemp[i].x2);

		CalculateXValue();

		switch (type)
		{
		case step:
			yValue = StepFunction();
			break;
		case sign:
			yValue = SignFunction();
			break;
		case sigmoid:
			yValue = SigmoidFunction();
			break;
		case linear:
			yValue = LinearFunction();
			break;
		default:
			std::cout << "Type of neuron not defined, this neuron wont work" << std::endl;
		}

		float error = dataTemp[i].yd - yValue;
		signals[0].AdjustWeight(signals[0].weight + alpha * (dataTemp[i].x1*error));
		signals[1].AdjustWeight(signals[1].weight + alpha * (dataTemp[i].x2*error));

		std::cout << "Adjusting weights by " << signals[0].weight + alpha * (dataTemp[i].x1*error) << " and " << signals[1].weight + alpha * (dataTemp[i].x2*error) << std::endl;
		std::cout << "Weights are " << signals[0].weight << " and  " << signals[1].weight << std::endl;
	}
}