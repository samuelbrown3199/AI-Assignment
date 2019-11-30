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

	float r,g,b;

	Chromosome()
	{
		r = rand() % 255;
		g = rand() % 255;
		b = rand() % 255;
	}
	Chromosome(std::vector<int> parent1, std::vector<int> parent2)
	{
		for (int i = 0; i < parent1.size(); i++)
		{
			genes.push_back(parent1[i]);
		}
		for (int i = 0; i < parent2.size(); i++)
		{
			genes.push_back(parent2[i]);
		}
	}
	Chromosome(Chromosome* clone)
	{
		genes = clone->genes;
	}

	void NextMove(Maze* maze)
	{
		if (!setupPathing)
		{
			startX = maze->startX;
			startY = maze->startY;

			cPosX = startX;
			cPosY = startY;

			setupPathing = true;
		}

		if (!pathFinished)
		{
			if (genes[curGene] == 0 && genes[curGene + 1] == 0) //move up
			{
				if (cPosY - 1 < 0)
				{
					curGene += 2;
				}
				else
				{
					if (maze->FindTileAtPos(cPosY - 1, cPosX).type == 1)
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
					if (maze->FindTileAtPos(cPosY + 1, cPosX).type == 1)
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
					if (maze->FindTileAtPos(cPosY, cPosX + 1).type == 1)
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
					if (maze->FindTileAtPos(cPosY, cPosX - 1).type == 1)
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

		if(curGene == genes.size())
		{
			endX = cPosX;
			endY = cPosY;

			pathFinished = true;
		}
	}

	void RenderChromosome(SDL_Renderer* _renderer)
	{
		chrRect.x = 12 + (50 * cPosX);
		chrRect.y = 12 + (50 * cPosY);
		chrRect.w = 25;
		chrRect.h = 25;

		SDL_SetRenderDrawColor(_renderer, r, g, b, 255);
		SDL_RenderFillRect(_renderer, &chrRect);

	}
};

class Genetic
{
private:

	Maze* currentMaze;

	const int numOfChromosomes = 8;
	int numberOfGenes = 16;
	float crossoverRate = 0.7;
	float mutationRate = 0.001;

	float totalPercent = 0;
	Chromosome pairs[4];

	Chromosome* offspring[8];

public:

	int generation = 1;
	int currentChromo = 0;
	std::vector<Chromosome*> chromosomes;

	Genetic(Maze* _maze);
	~Genetic();

	void GenerateInitialChromosomes();
	void RunChromosomeLoop();
	void FitnessFunction(Chromosome* _some);
	void SetupRouletteWheel();
	void MateFunction();
	void MutateOffspring();

	void GeneticLoop(SDL_Renderer* _renderer);
};

#endif