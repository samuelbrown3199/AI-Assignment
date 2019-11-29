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
		chromosomes[i]->chromoNum = i;

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

	RunChromosomeLoop();
}

void Genetic::RunChromosomeLoop()
{
	std::vector<Chromosome*>::iterator cItr;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		//calculate the path that the chromosome is going to follow
		(*cItr)->CalculatePath(currentMaze);
		FitnessFunction(*cItr);
	}

	MateFunction();
}

void Genetic::FitnessFunction(Chromosome* _some)
{
	float tempFit = (float)1 / ((currentMaze->endX - _some->endX) + (currentMaze->endY - _some->endY) + 1);
	_some->fitness = tempFit;

	std::cout << "Fitness for chromosome " << _some->chromoNum << " is " << _some->fitness << std::endl;
}

void Genetic::MateFunction()
{
	float totalFitness = 0;

	std::vector<Chromosome*>::iterator cItr;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		totalFitness += (*cItr)->fitness;
	}
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		(*cItr)->portionOfWheel = (float)(*cItr)->fitness / totalFitness;
		std::cout << "Portion of wheel for chromosome: " << (*cItr)->chromoNum << " is " << (*cItr)->portionOfWheel << std::endl;
	}

	//implement the random wheel with the correct amount of portions for each individual and make it as flexible as possible
}