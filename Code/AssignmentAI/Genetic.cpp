#include "Genetic.h"



Genetic::Genetic(Maze* _maze)
{
	currentMaze = _maze;
	GenerateInitialChromosomes();
}


Genetic::~Genetic()
{
}

void Genetic::GenerateInitialChromosomes()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, 1.0f);

	for (int i = 0; i < numOfChromosomes; i++)
	{
		chromosomes.push_back(new Chromosome());
		std::cout << "Chromosome " << i << ": ";

		for (int o = 0; o < numberOfGenes; o++)
		{
			double r = dist(generator);
			if (r <= 0.5)
			{
				chromosomes[i]->genes.push_back(0);
			}
			else
			{
				chromosomes[i]->genes.push_back(1);
			}
			std::cout << chromosomes[i]->genes[o];
		}
		std::cout << std::endl;
	}

	//RunChromosomeLoop();
}

void Genetic::RunChromosomeLoop()
{
	std::vector<Chromosome*>::iterator cItr;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		//calculate the path that the chromosome is going to follow
		(*cItr)->NextMove(currentMaze);
		FitnessFunction(*cItr);
	}

	SetupRouletteWheel();
}

void Genetic::FitnessFunction(Chromosome* _some)
{
	float tempFit = (float)1 / ((currentMaze->endX - _some->endX) + (currentMaze->endY - _some->endY) + 1);
	_some->fitness = tempFit;
}

void Genetic::SetupRouletteWheel()
{
	float totalFitness = 0;

	std::vector<Chromosome*>::iterator cItr;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		totalFitness += (*cItr)->fitness;
	}
	
	totalPercent = 0;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		(*cItr)->portionOfWheel = (float)(*cItr)->fitness / totalFitness;
		totalPercent += (float)(*cItr)->portionOfWheel;
	}
	//implement the random wheel with the correct amount of portions for each individual and make it as flexible as possible

	float currentPercent = 0;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		(*cItr)->rangeStart = currentPercent;
		currentPercent += (*cItr)->portionOfWheel;
	}

	MateFunction();
}

void Genetic::MateFunction()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, totalPercent);

	std::vector<Chromosome*>::iterator cItr;
	for (int i = 0; i < 4; i++)
	{
		float r = dist(generator);

		for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
		{
			if (r >= (*cItr)->rangeStart && r < ((*cItr)->portionOfWheel + (*cItr)->rangeStart))
			{
				pairs[i] = **cItr;
			}
		}
	}

	std::vector<int>gene1(pairs[0].genes.begin(), pairs[0].genes.begin() + 8);
	std::vector<int>gene2(pairs[0].genes.begin() + 8, pairs[0].genes.end());
	
	std::vector<int>gene3(pairs[1].genes.begin(), pairs[1].genes.begin() + 8);
	std::vector<int>gene4(pairs[1].genes.begin() + 8, pairs[1].genes.end());

	std::vector<int>gene5(pairs[2].genes.begin(), pairs[2].genes.begin() + 8);
	std::vector<int>gene6(pairs[2].genes.begin() + 8, pairs[2].genes.end());

	std::vector<int>gene7(pairs[3].genes.begin(), pairs[3].genes.begin() + 8);
	std::vector<int>gene8(pairs[3].genes.begin() + 8, pairs[3].genes.end());

	std::uniform_real_distribution<double> dist2(0.0f, 1);

	if (offspring[0] != nullptr)
	{
		for (int i = 0; i < 8; i++)
		{
			delete offspring[i];
		}
	}

	float crossover1 = dist2(generator);
	float crossover2 = dist2(generator);
	if (crossover1 <= crossoverRate)
	{
		//perform crossover for new offspring
		offspring[0] = (new Chromosome(gene1, gene4));
		offspring[1] = (new Chromosome(gene3, gene2));
		offspring[2] = (new Chromosome(gene1, gene4));
		offspring[3] = (new Chromosome(gene3, gene2));
	}
	else
	{
		//copy parents into new offspring
		offspring[0] = (new Chromosome(&pairs[0]));
		offspring[1] = (new Chromosome(&pairs[1]));
		offspring[2] = (new Chromosome(&pairs[0]));
		offspring[3] = (new Chromosome(&pairs[1]));
	}

	if (crossover2 <= crossoverRate)
	{
		//perform crossover for new offspring
		offspring[4] = (new Chromosome(gene5, gene8));
		offspring[5] = (new Chromosome(gene7, gene6));
		offspring[6] = (new Chromosome(gene5, gene8));
		offspring[7] = (new Chromosome(gene7, gene6));
	}
	else
	{
		offspring[4] = (new Chromosome(&pairs[2]));
		offspring[5] = (new Chromosome(&pairs[3]));
		offspring[6] = (new Chromosome(&pairs[2]));
		offspring[7] = (new Chromosome(&pairs[3]));
	}

	MutateOffspring();
}

void Genetic::MutateOffspring()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, 1);

	for (int i = 0; i < 8; i++)
	{
		for (int o = 0; o < 16; o++)
		{
			float mutate = dist(generator);
			if (mutate <= mutationRate)
			{
				if (offspring[i]->genes[o] == 1)
				{
					offspring[i]->genes[o] = 0;
				}
				else
				{
					offspring[i]->genes[o] = 1;
				}
			}
		}
	}

	chromosomes.clear();

	for (int i = 0; i < 8; i++)
	{
		chromosomes.push_back(offspring[i]);
	}

	currentChromo = 0;
	generation++;
	std::cout << "Starting generation: " << generation << std::endl;
}

void Genetic::GeneticLoop(SDL_Renderer* _renderer)
{
	/*if (chromosomes.size() > 0)
	{
		std::vector<Chromosome*>::iterator cItr;
		for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
		{
			if (!(*cItr)->pathFinished)
			{
				(*cItr)->NextMove(currentMaze);
			}
			else
			{
				FitnessFunction((*cItr));
				if (cItr == chromosomes.end())
				{
					SetupRouletteWheel(); //doesnt call this function so no evolution happens
				}
			}
		}
	}*/
	if (currentChromo < numOfChromosomes)
	{
		if (chromosomes[currentChromo] != nullptr)
		{
			if (!chromosomes[currentChromo]->pathFinished)
			{
				chromosomes[currentChromo]->NextMove(currentMaze);
			}
			else
			{
				FitnessFunction((chromosomes[currentChromo]));
				currentChromo++;
			}
		}
	}
	else
	{
		SetupRouletteWheel();
	}
}