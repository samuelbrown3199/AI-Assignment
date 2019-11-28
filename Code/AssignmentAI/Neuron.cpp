#include "Neuron.h"



Neuron::Neuron(int _index, float _alpha, float _threshold)
{
	neuronIndex = _index;
	alpha = _alpha;
	threshold = _threshold;
}


Neuron::~Neuron()
{
}

void Neuron::CalculateOutput()
{
	GetSignalCount();

	if (signalCount > 1)
	{
		std::vector<InputSignal>::iterator sItr;
		for (sItr = signals.begin(); sItr != signals.end(); sItr++)
		{
			xValue += (*sItr).value*(*sItr).weight;
		}

		yValue = SigmoidFunction(2.71828);
	}
}

void Neuron::AddInputSignal(float _weight)
{
	InputSignal newSig;
	newSig.index = signals.size();
	newSig.weight = _weight;

	signals.push_back(newSig);
}

void Neuron::AddPreviousNeuron(int input, Neuron* prevNeur)
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		(*sItr).previousNeuron = prevNeur;
		(*sItr).value = prevNeur->yValue;
	}
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

void Neuron::AdjustSignalWeightAtIndex(int i, float val)
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		if ((*sItr).index == i)
		{
			(*sItr).weight = val;
			break;
		}
	}
}

float Neuron::GetSignalWeight(int i)
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		if ((*sItr).index == i)
		{
			return (*sItr).weight;
			break;
		}
	}
}

void Neuron::GetSignalCount()
{
	std::vector<InputSignal>::iterator sItr;
	for (sItr = signals.begin(); sItr != signals.end(); sItr++)
	{
		signalCount++;
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

float Neuron::SigmoidFunction(float e)
{
	return 1 / (1 + pow(e, -(xValue - threshold)));
}

float Neuron::LinearFunction()
{
	return xValue - threshold;
}

void Neuron::TrainingAlgorithm()
{
	signals[0].weight = 0.3;
	signals[1].weight = -0.1;
	alpha = 0.1;

	int iter = 20;
	for (int p = 0; p < iter; p++)
	{
		for (int i = 0; i < 4; i++)
		{
			signals[0].value = data[i]->input[0];
			signals[1].value = data[i]->input[1];
			CalculateOutput();
			yValue = SigmoidFunction(2.71828);

			float error = data[i]->desiredY - yValue;
			signals[0].weight += alpha * (signals[0].value*error);
			signals[1].weight += alpha * (signals[1].value*error);

			std::cout << signals[0].weight << "    " << signals[1].weight << std::endl;
		}
	}
}