#include "Genetic.h"



Genetic::Genetic(Maze* _maze, int chromNum, int geneNum, float crossOverRate, float muteChance)
{
	currentMaze = _maze;

	while(chromNum % 4 != 0)
	{
		chromNum++;
	}
	while (geneNum % 2 != 0)
	{
		geneNum++;
	}

	numOfChromosomes = chromNum;
	numberOfGenes = geneNum;
	numOfPairs = numOfChromosomes / 4;
	numOfOffspring = numOfChromosomes;

	crossoverRate = crossOverRate;
	mutationRate = muteChance;

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
}

void Genetic::FitnessFunction(Chromosome* _some) //needs a change to punish for being near a wall
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
	for (int i = 0; i < numOfPairs; i++)
	{
		Chromosome* tempPair[2];

		for (int o = 0; o < 2; o++)
		{
			float r = dist(generator);

			for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
			{
				if (r >= (*cItr)->rangeStart && r < ((*cItr)->portionOfWheel + (*cItr)->rangeStart))
				{
					tempPair[o] = *cItr;
				}
			}
		}

		pairs.push_back(new Pair(tempPair));
	}

	std::uniform_real_distribution<double> dist2(0.0f, 1);

	for (int curPair = 0; curPair < numOfPairs; curPair++)
	{
		float crossover = dist2(generator);

		if (crossover <= crossoverRate)
		{
			//perform crossover for new offspring
			for (int i = 0; i < numOfOffspring; i++)
			{
				if (i % 2 == 0)
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom1, pairs[curPair]->chrom2));
				}
				else
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom2, pairs[curPair]->chrom1));
				}
			}
		}
		else
		{
			//copy parents into new offspring
			for (int i = 0; i < numOfOffspring; i++)
			{
				if (i % 2 == 0)
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom1));
				}
				else
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom2));
				}
			}
		}
	}

	MutateOffspring();
}

void Genetic::MutateOffspring()
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, 1);

	for (int i = 0; i < numOfChromosomes; i++)
	{
		for (int o = 0; o < numberOfGenes; o++)
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

	pairs.clear();
	chromosomes.clear();

	generation++;
	std::cout << "Starting generation: " << generation << std::endl;

	for (int i = 0; i < numOfChromosomes; i++)
	{
		chromosomes.push_back(offspring[i]);
		offspring[i] = nullptr;

		std::cout << "Chromosome " << i << ": ";

		for (int o = 0; o < numberOfGenes; o++)
		{
			std::cout << chromosomes[i]->genes[o];
		}
		std::cout << std::endl;
	}
	
	offspring.clear();
	currentChromo = 0;
}

void Genetic::GeneticLoop(SDL_Renderer* _renderer)
{
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

				std::cout << "Currently running chromosome: " << currentChromo << std::endl;
			}
		}
	}
	else
	{
		SetupRouletteWheel();
	}
}