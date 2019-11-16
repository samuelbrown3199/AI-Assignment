#include "Network.h"



Network::Network()
{
	network[0] = new Neuron(1, 0.1, 0.1);
	network[0]->AddInputSignal(1);

	network[1] = new Neuron(2, 0.1, 0.1);
	network[1]->AddInputSignal(1);

	network[2] = new Neuron(3, 0.1, 0.8);
	network[2]->AddInputSignal(0.5);
	network[2]->AddPreviousNeuron(1, network[0]);
	network[2]->AddInputSignal(0.4);
	network[2]->AddPreviousNeuron(2, network[1]);

	network[3] = new Neuron(4, 0.1, -0.1);
	network[3]->AddInputSignal(0.9);
	network[3]->AddPreviousNeuron(1, network[0]);
	network[3]->AddInputSignal(1.0);
	network[3]->AddPreviousNeuron(2, network[1]);

	network[4] = new Neuron(5, 0.1, 0.3);
	network[4]->AddInputSignal(-1.2);
	network[4]->AddPreviousNeuron(1, network[2]);
	network[4]->AddInputSignal(1.1);
	network[4]->AddPreviousNeuron(2, network[3]);

	//TrainingAlgorithm();
	HardCode();
}


Network::~Network()
{
}

void Network::TrainingAlgorithm()
{
	int nIter = 10000;
	int curItr = 1;

	data[0] = new TrainingData(1, 1, 0);
	data[1] = new TrainingData(0, 1, 1);
	data[2] = new TrainingData(1, 0, 1);
	data[3] = new TrainingData(0, 0, 0);

	while (curItr <= (nIter - 4))
	{
		std::cout << "Iteration " << curItr << std::endl;

		for (int i = 0; i < 4; i++)
		{
			int epocSumError = 0;

			network[0]->yValue = data[i]->input[0];
			network[1]->yValue = data[i]->input[1];

			network[2]->CalculateOutput();
			network[3]->CalculateOutput();
			network[4]->CalculateOutput();

			network[4]->error = data[i]->desiredY - network[4]->yValue;
			network[4]->delta = (network[4]->yValue*(1 - network[4]->yValue))*network[4]->error;
			float w1 = network[4]->GetSignalWeight(0);
			float w2 = network[4]->GetSignalWeight(1);
			network[4]->AdjustSignalWeightAtIndex(0, w1 + ((network[4]->alpha*network[2]->yValue)*network[4]->delta));
			network[4]->AdjustSignalWeightAtIndex(1, w2 + ((network[4]->alpha*network[3]->yValue)*network[4]->delta));

			network[2]->error = network[4]->GetSignalWeight(0) - network[4]->error;
			network[2]->delta = (network[2]->yValue*(1 - network[2]->yValue))*(network[4]->delta * network[4]->GetSignalWeight(0));
			network[2]->AdjustSignalWeightAtIndex(0, network[2]->GetSignalWeight(0) + (network[2]->alpha*data[i]->input[0] * network[2]->delta));
			network[2]->AdjustSignalWeightAtIndex(1, network[2]->GetSignalWeight(1) + (network[2]->alpha*data[i]->input[1] * network[2]->delta));

			network[3]->error = network[4]->GetSignalWeight(1) - network[4]->error;
			network[3]->delta = (network[3]->yValue*(1 - network[3]->yValue))*(network[4]->delta * network[4]->GetSignalWeight(1));
			network[3]->AdjustSignalWeightAtIndex(0, network[3]->GetSignalWeight(0) + (network[3]->alpha*data[i]->input[0] * network[3]->delta));
			network[3]->AdjustSignalWeightAtIndex(1, network[3]->GetSignalWeight(1) + (network[3]->alpha*data[i]->input[1] * network[3]->delta));

			network[2]->CalculateOutput();
			network[3]->CalculateOutput();
			network[4]->CalculateOutput();

			float tx3 = network[2]->xValue;
			float ty3 = network[2]->yValue;
			float tx4 = network[3]->xValue;
			float ty4 = network[3]->yValue;
			float tx5 = network[4]->xValue;
			float ty5 = network[4]->yValue;
			float te = data[i]->desiredY - network[4]->yValue;

			epocSumError = epocSumError + (te*te);
		}

		std::cout << "y3 " << network[2]->yValue << std::endl;
		std::cout << "y4 " << network[3]->yValue << std::endl;
		std::cout << "y5 " << network[4]->yValue << std::endl;

		curItr++;
	}
}

void Network::HardCode()
{
	int nIter = 10000;
	int curItr = 1;

	data[0] = new TrainingData(1, 1, 0);
	data[1] = new TrainingData(0, 1, 1);
	data[2] = new TrainingData(1, 0, 1);
	data[3] = new TrainingData(0, 0, 0);

	float w[5][5];
	w[1][3] = 0.5f;
	w[1][4] = 0.9f;
	w[3][5] = -1.2f;
	w[2][3] = 0.4f;
	w[2][4] = 1.0f;
	w[4][5] = 1.1f;

	float theta[5];
	theta[3] = 0.8f;
	theta[4] = -0.1f;
	theta[5] = 0.3f;

	float alpha = 0.1f;

	float xVal[5];
	float yVal[5];

	float e[5];
	float delta[5];

	float tx[5];
	float ty[5];
	float te;

	while (curItr <= (nIter - 4))
	{
		std::cout << "Iteration " << curItr << std::endl;
		float epocSumError = 0;

		for (int i = 0; i < 4; i++)
		{
			xVal[3] = (data[i]->input[0] * w[1][3]) + (data[i]->input[2]*w[2][3]);
			yVal[3] = 1 / (1 + pow(2.71828, -(xVal[3] - theta[3])));

			xVal[4] = (data[i]->input[0] * w[1][4]) + (data[i]->input[2] * w[2][4]);
			yVal[4] = 1/(1 + pow(2.71828, -(xVal[4] - theta[4])));

			xVal[5] = (yVal[3] * w[3][5]) + (yVal[4] * w[4][5]);
			yVal[5] = 1 / (1 + pow(2.71828, -(xVal[4] - theta[5])));

			e[5] = data[i]->desiredY - yVal[5];
			delta[5] = yVal[5] * (1 - yVal[5])*e[5];
			float curW1 = w[3][5];
			float curW2 = w[4][5];
			w[3][5] = w[3][5] + (alpha*(yVal[3] * delta[5]));
			w[4][5] = w[4][5] + (alpha*(yVal[4] * delta[5]));
			theta[5] = theta[5] + alpha * (-1)*delta[5];

			e[3] = curW1 * e[5];
			delta[3] = yVal[3] * (1 - (yVal[3]*delta[5]*curW1));
			w[1][3] = w[1][3] + (alpha*(data[i]->input[0] * delta[3]));
			w[2][3] = w[2][3] + (alpha*(data[i]->input[1] * delta[3]));
			theta[3] = theta[3] + alpha * (-1)*delta[3];

			e[4] = curW2 * e[4];
			delta[4] = yVal[4] * (1 - (yVal[4] * delta[5] * curW2));
			w[1][4] = w[1][4] + (alpha*(data[i]->input[0] * delta[4]));
			w[2][4] = w[2][4] + (alpha*(data[i]->input[1] * delta[4]));
			theta[4] = theta[4] + alpha * (-1)*delta[4];

			tx[3] = (data[i]->input[0] * w[1][3]) + (data[i]->input[1] * w[2][3]);
			ty[3] = 1 / (1 + pow(2.71828, -(tx[3] - theta[3])));
			tx[4] = (data[i]->input[0] * w[1][4]) + (data[i]->input[1] * w[2][4]);
			ty[4] = 1 / (1 + pow(2.71828, -(tx[4] - theta[4])));
			tx[5] = (ty[3] * w[3][5]) + (ty[4] * w[4][5]);
			ty[5] = 1 / (1 + pow(2.71828, -(tx[5] - theta[5])));
			te = data[i]->desiredY - ty[5];

			std::cout << "y3 " << yVal[3] << std::endl;
			std::cout << "y4 " << yVal[4] << std::endl;
			std::cout << "y5 " << yVal[5] << std::endl;
		}

		epocSumError = epocSumError + (te*te);

		if (epocSumError < 0.001)
		{
			std::cout << "Trained" << std::endl;
			break;
		}

		curItr++;
	}
}