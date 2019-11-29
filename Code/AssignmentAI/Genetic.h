#pragma once
#ifndef GENETIC_H_
#define GENETIC_H_

#include <iostream>
#include <vector>
#include <random>

#include "Maze.h"

struct Chromosome
{
	int chromoNum;
	std::vector<int> genes;
	float fitness;
	float portionOfWheel;

	int startX, startY;
	int endX, endY;
	int cPosX, cPosY;

	void CalculatePath(Maze* maze)
	{
		startX = maze->startX;
		startY = maze->startY;

		cPosX = startX;
		cPosY = startY;

		int curGene = 0;

		while (curGene < genes.size())
		{
			if (genes[curGene] == 0 && genes[curGene + 1] == 0) //move up
			{
				if (cPosY - 1 < 0)
				{
					curGene += 2;
					continue;
				}
				else
				{
					if (maze->FindTileAtPos(cPosY-1, cPosX).type == 1)
					{
						curGene += 2;
						continue;
					}
					else
					{
						cPosY -= 1;
					}
				}
			}
			if (genes[curGene] == 1 && genes[curGene + 1] == 1) //move down
			{
				if (cPosY + 1 > maze->row)
				{
					curGene += 2;
					continue;
				}
				else
				{
					if (maze->FindTileAtPos(cPosY + 1, cPosX).type == 1)
					{
						curGene += 2;
						continue;
					}
					else
					{
						cPosY += 1;
					}
				}
			}
			if (genes[curGene] == 0 && genes[curGene + 1] == 1) //move right
			{
				if (cPosX + 1 > maze->col)
				{
					curGene += 2;
					continue;
				}
				else
				{
					if (maze->FindTileAtPos(cPosY, cPosX+1).type == 1)
					{
						curGene += 2;
						continue;
					}
					else
					{
						cPosX += 1;
					}
				}
			}
			if (genes[curGene] == 1 && genes[curGene + 1] == 0) //move left
			{
				if (cPosX - 1 < 0)
				{
					curGene += 2;
					continue;
				}
				else
				{
					if (maze->FindTileAtPos(cPosY, cPosX - 1).type == 1)
					{
						curGene += 2;
						continue;
					}
					else
					{
						cPosX -= 1;
					}
				}
			}
			curGene +=  2;
		}

		endX = cPosX;
		endY = cPosY;

		std::cout << "End position for chromosome " << chromoNum << " is X: " << endX << " and Y: " << endY << std::endl;
	}
};

class Genetic
{
private:

	Maze* currentMaze;

	const int numOfChromosomes = 8;
	int numberOfGenes = 16;

	std::vector<Chromosome*> chromosomes;

	Chromosome pair1[2];
	Chromosome pair2[2];

public:
	Genetic(Maze* _maze);
	~Genetic();

	void GenerateInitialChromosomes();
	void RunChromosomeLoop();
	void FitnessFunction(Chromosome* _some);
	void MateFunction();
};

#endif