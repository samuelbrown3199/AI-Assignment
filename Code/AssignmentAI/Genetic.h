#pragma once
#ifndef GENETIC_H_
#define GENETIC_H_

#include <iostream>
#include <vector>
#include <random>
#include <SDL.h>

#include "Maze.h"

struct Chromosome
{
	std::vector<int> genes;
	float fitness;
	float portionOfWheel;
	float rangeStart = 0;

	int startX, startY;
	int endX, endY;
	int cPosX, cPosY;

	SDL_Rect chrRect;
	int curGene = 0;
	bool pathFinished = false;
	bool setupPathing = false;

	float r, g, b;

	Chromosome() //chromosome default constructor, sets a random colour
	{
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
	}
	Chromosome(Chromosome* clone) //creates a new chromosome using the pass parameter.
	{
		r = clone->r;
		g = clone->g;
		b = clone->b;

		genes = clone->genes;
	}
	Chromosome(Chromosome* parent1, Chromosome* parent2) //creates a new chromosome with two parents
	{
		r = (parent1->r+parent2->r)/2; //work out the average colour of the parents
		g = (parent1->g + parent2->g) / 2;
		b = (parent1->b + parent2->b) / 2;

		std::vector<int>gene1(parent1->genes.begin(), parent1->genes.begin() + parent2->genes.size() / 2); //split the genes into halves
		std::vector<int>gene2(parent2->genes.begin() + parent2->genes.size()/2, parent2->genes.end());

		for (int i = 0; i < gene1.size(); i++) //push the split genes into the gene list
		{
			genes.push_back(gene1[i]);
		}
		for (int i = 0; i < gene2.size(); i++)
		{
			genes.push_back(gene2[i]);
		}
	}

	void NextMove(Maze* maze) //works out the next move based on the genes
	{
		if (!setupPathing) //sets up the pathing the first loop through
		{
			startX = maze->startX;
			startY = maze->startY;

			cPosX = startX;
			cPosY = startY;

			setupPathing = true;
		}

		if (!pathFinished) //checks if the path has finished
		{
			if (genes[curGene] == 0 && genes[curGene + 1] == 0) //move up
			{
				if (cPosY - 1 < 0)
				{
					curGene += 2;
				}
				else
				{
					if (maze->FindTileAtPos(cPosX, cPosY - 1).type == 1) //trying to move into wall, not valid
					{
						curGene += 2;
					}
					else
					{
						cPosY -= 1;
						curGene += 2;
					}
				}
			}
			else if (genes[curGene] == 1 && genes[curGene + 1] == 1) //move down
			{
				if (cPosY + 1 >= maze->row)
				{
					curGene += 2;
				}
				else
				{
					if (maze->FindTileAtPos(cPosX, cPosY + 1).type == 1) //trying to move into wall, not valid
					{
						curGene += 2;
					}
					else
					{
						cPosY += 1;
						curGene += 2;
					}
				}
			}
			else if (genes[curGene] == 0 && genes[curGene + 1] == 1) //move right
			{
				if (cPosX + 1 >= maze->col)
				{
					curGene += 2;
				}
				else
				{
					if (maze->FindTileAtPos(cPosX + 1, cPosY).type == 1) //trying to move into wall, not valid
					{
						curGene += 2;
					}
					else
					{
						cPosX += 1;
						curGene += 2;
					}
				}
			}
			else if (genes[curGene] == 1 && genes[curGene + 1] == 0) //move left
			{
				if (cPosX - 1 < 0)
				{
					curGene += 2;
				}
				else
				{
					if (maze->FindTileAtPos(cPosX - 1, cPosY).type == 1) //trying to move into wall, not valid
					{
						curGene += 2;
					}
					else
					{
						cPosX -= 1;
						curGene += 2;
					}
				}
			}
		}

		if (curGene == genes.size()) //checks if we are at the end of the gene instructions
		{
			endX = cPosX; //sets the end position of the chromosome
			endY = cPosY;

			pathFinished = true; //path is finished
		}
	}

	void RenderChromosome(SDL_Renderer* _renderer) //used to render the chromosome agent to the screen
	{
		chrRect.x = 12 + (50 * cPosX);
		chrRect.y = 12 + (50 * cPosY);
		chrRect.w = 25;
		chrRect.h = 25;

		SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
		SDL_RenderFillRect(_renderer, &chrRect);

	}
};

struct Pair //used to store parent chromosomes for breeding
{
	Chromosome* chrom1;
	Chromosome* chrom2;

	Pair(Chromosome* _pair[2])
	{
		chrom1 = _pair[0];
		chrom2 = _pair[1];
	}
};

class Genetic
{
private:

	Maze* currentMaze;

	int numOfChromosomes = 8;
	int numberOfGenes = 16;
	float crossoverRate = 0.7;
	float mutationRate = 0.001;

	int numOfPairs;
	int numOfOffspring;

	float totalPercent = 0;
	std::vector<Pair*> pairs;

	std::vector<Chromosome*>offspring;

	int mode = 0;

public:

	int generation = 1;
	int currentChromo = 0;
	std::vector<Chromosome*> chromosomes;

	Genetic(Maze* _maze, int chromNum, int geneNum, float crossOverRate, float muteChance, int _mode);
	~Genetic();

	void GenerateInitialChromosomes();
	void FitnessFunction(Chromosome* _some);
	void SetupRouletteWheel();
	void MateFunction();
	void MutateOffspring();

	void GeneticLoop(SDL_Renderer* _renderer);
};

#endif