#include "Neuron.h"



Neuron::Neuron()
{
}


Neuron::~Neuron()
{
}

void Neuron::CalculateOutput()
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

void Neuron::AdjustSignalAtIndex(int i, float val, float wei)
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		if ((*sItr).index == i)
		{
			(*sItr).InitialiseValues(val, wei);
			break;
		}
	}
}