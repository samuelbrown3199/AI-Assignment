#pragma once
#ifndef NETWORK_H_
#define NETWORk_H_

#include <iostream>
#include <math.h>
#include "Neuron.h"

class Network
{
public:

	Neuron* network[5]{ nullptr };
	TrainingData* data[4]{ nullptr };

	Network();
	~Network();

	void TrainingAlgorithm();
	void HardCode();
};

#endif