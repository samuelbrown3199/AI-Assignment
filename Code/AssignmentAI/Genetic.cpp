#include "Genetic.h"



Genetic::Genetic(Maze* _maze, int chromNum, int geneNum, float crossOverRate, float muteChance, int _mode) //constructor for the genetic algorithm
{
	currentMaze = _maze; //keeps a pointer to the maze data

	while(chromNum % 4 != 0) //makes sure the chromosome count is a multiple of 4
	{
		chromNum++;
	}
	while (geneNum % 2 != 0) //makes sure the gene count is a multiple of 2
	{
		geneNum++;
	}
	if (crossOverRate > 1) //makes sure the crossover rate is valid
	{
		crossOverRate = 1;
	}
	else if (crossOverRate < 0)
	{
		crossOverRate = 0;
	}
	if (muteChance > 1) //makes sure the mutation rate is valid
	{
		muteChance = 1;
	}
	else if (muteChance < 0)
	{
		muteChance = 0;
	}

	numOfChromosomes = chromNum; //stores values for later use
	numberOfGenes = geneNum;
	numOfPairs = numOfChromosomes / 4;
	numOfOffspring = numOfChromosomes;

	crossoverRate = crossOverRate;
	mutationRate = muteChance;

	mode = _mode;

	GenerateInitialChromosomes(); //generates the start set of chromosomes
}


Genetic::~Genetic()
{
}

void Genetic::GenerateInitialChromosomes() //used to generate the starting chromosomes
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, 1.0f); //used to generate an equal distribution of 1s and 0s

	for (int i = 0; i < numOfChromosomes; i++) //loops through the amount of chromosomes
	{
		chromosomes.push_back(new Chromosome()); //pushes back a new chromosome
		std::cout << "Chromosome " << i << ": ";

		for (int o = 0; o < numberOfGenes; o++) //loops through the genes of the chromosome
		{
			double r = dist(generator); //generates a random floating point number between 0-1
			if (r <= 0.5) //it the number is less than 0.5 then the gene will be a 0
			{
				chromosomes[i]->genes.push_back(0);
			}
			else //else its a 1
			{
				chromosomes[i]->genes.push_back(1);
			}
			std::cout << chromosomes[i]->genes[o]; //print out each gene of each chromosome
		}
		std::cout << std::endl;
	}
}

void Genetic::FitnessFunction(Chromosome* _some) //ued to work out the fitness of the chromosome
{
	float tempFit = (float)1 / ((currentMaze->endX - _some->endX) + (currentMaze->endY - _some->endY) + 1); //works out the fitness based on the direct distance from the finished point to the end point
	_some->fitness = tempFit; //stores it into the chromosome
	
	if (mode == 1) //if we're using mode 1 we attempt to punish chromosomes with walls blocking their path to the end goal
	{
		int xDif = currentMaze->endX - _some->endX;
		int yDif = currentMaze->endY - _some->endY;

		if (yDif > 0)
		{
			if (currentMaze->FindTileAtPos(_some->endX + 1, _some->endY).type == 1)
			{
				_some->fitness -= 0.1;
			}
			if (currentMaze->FindTileAtPos(_some->endX - 1, _some->endY).type == 1)
			{
				_some->fitness -= 0.1;
			}
		}
		if (xDif > 0)
		{
			if (currentMaze->FindTileAtPos(_some->endX, _some->endY + 1).type == 1)
			{
				_some->fitness -= 0.1;
			}
			if (currentMaze->FindTileAtPos(_some->endX, _some->endY - 1).type == 1)
			{
				_some->fitness -= 0.1;
			}
		}
		if (_some->fitness <= 0)
		{
			_some->fitness = 0.01;
		}
	}
}

void Genetic::SetupRouletteWheel() //used to setup the roulette wheel for choosing genetic pairs
{
	float totalFitness = 0; //works out the total fitness

	std::vector<Chromosome*>::iterator cItr;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		totalFitness += (*cItr)->fitness; //loops through the chromosomes and adds their total fitness
	}
	
	totalPercent = 0;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
	{
		(*cItr)->portionOfWheel = (float)(*cItr)->fitness / totalFitness;
		totalPercent += (float)(*cItr)->portionOfWheel; //works out the total percent to use for working out the proportions of the wheel
	}

	float currentPercent = 0;
	for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++) //works out the ranges in which chromosomes can be picked for breeding
	{
		(*cItr)->rangeStart = currentPercent;
		currentPercent += (*cItr)->portionOfWheel;
	}

	MateFunction(); //starts the breeding function
}

void Genetic::MateFunction() //used to generate the offspring
{
	std::default_random_engine generator;
	std::cout << "Total percent: " << totalPercent << std::endl;
	std::uniform_real_distribution<double> dist(0.0f, totalPercent);

	for (int i = 0; i < numOfPairs; i++) //loops through the pair count
	{
		Chromosome* tempPair[2]; //creates a temperary pair

		for (int o = 0; o < 2; o++) //used to pick 2 random chromosomes
		{
			float r = dist(generator); //gnerations a random number
			
			std::vector<Chromosome*>::iterator cItr;
			for (cItr = chromosomes.begin(); cItr != chromosomes.end(); cItr++)
			{
				if (r >= (*cItr)->rangeStart && r < ((*cItr)->portionOfWheel + (*cItr)->rangeStart)) //works out where the r value puts into what chromosome range
				{
					tempPair[o] = *cItr;
				}
			}
		}

		pairs.push_back(new Pair(tempPair)); //pushes the pairs back for use later
	}

	std::uniform_real_distribution<double> dist2(0.0f, 1);

	for (int curPair = 0; curPair < numOfPairs; curPair++) //loops through the pairs
	{
		float crossover = dist2(generator); //determines where the genes will be crossed over

		if (crossover <= crossoverRate)
		{
			//perform crossover for new offspring
			for (int i = 0; i < numOfOffspring; i++)
			{
				if (i % 2 == 0)
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom1, pairs[curPair]->chrom2)); //calls the cross over chromosome constructor
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
					offspring.push_back(new Chromosome(pairs[curPair]->chrom1)); //calls the clone chromosome constructor
				}
				else
				{
					offspring.push_back(new Chromosome(pairs[curPair]->chrom2));
				}
			}
		}
	}

	MutateOffspring(); //starts the mutation function
}

void Genetic::MutateOffspring() //used to mutate some of the genes in each offspring
{
	std::default_random_engine generator;
	std::uniform_real_distribution<double> dist(0.0f, 1);

	for (int i = 0; i < numOfChromosomes; i++) //loops through chromosomes
	{
		for (int o = 0; o < numberOfGenes; o++) //loops through genes
		{
			float mutate = dist(generator); //used to determine whether mutation will happen
			if (mutate <= mutationRate)
			{
				if (offspring[i]->genes[o] == 1) //switches a 0 to 1 and 1 to 0
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

	pairs.clear(); //clears the memory ot the pairs
	chromosomes.clear(); //clears the old chromosomes

	generation++; //increments generation count
	std::cout << "Starting generation: " << generation << std::endl;

	for (int i = 0; i < numOfChromosomes; i++)
	{
		chromosomes.push_back(offspring[i]); //pushes offspring into chromosome list
		offspring[i] = nullptr; //nulls the offspring

		std::cout << "Chromosome " << i << ": ";

		for (int o = 0; o < numberOfGenes; o++)
		{
			std::cout << chromosomes[i]->genes[o];
		}
		std::cout << std::endl;
	}
	
	offspring.clear(); //clears the offspring list
	currentChromo = 0; //sets to zero
}

void Genetic::GeneticLoop(SDL_Renderer* _renderer) //used in each frame in the main loop
{
	if (currentChromo < numOfChromosomes) //loops through chromosomes
	{
		if (chromosomes[currentChromo] != nullptr) //if its not a nullptr
		{
			if (!chromosomes[currentChromo]->pathFinished) //if its path isnt finished
			{
				chromosomes[currentChromo]->NextMove(currentMaze); //do the chromosomes next move
			}
			else //if it is finished
			{
				FitnessFunction((chromosomes[currentChromo])); //do the fitness function for the chromosome
				currentChromo++; //increment to the next chromosome

				std::cout << "Currently running chromosome: " << currentChromo-1 << std::endl;
			}
		}
	}
	else //all the chromosomes are finished
	{
		SetupRouletteWheel(); //setup the roulette wheel
	}
}