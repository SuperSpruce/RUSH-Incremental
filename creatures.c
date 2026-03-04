#include <stdlib.h>
#include <string.h>
#include "RushNNincludes.h"

#define NUM_GENS 22221 
int nn_topo[] = {30, 30, 21};

void deathAndReproduction(Creature*, instruction*, pcg32_random_t*, int);
void mutate(Creature*, pcg32_random_t*);
void creatureMergeSortWrapper(Creature*);
Creature* creatureMergeSort(Creature*, Creature*, int, int);
Creature* creatureFinalMerge(Creature*, Creature*, Creature*, int, int, int);

int main()
{
	pcg32_random_t srng;
	pcg32_random_t* prng = &srng;

	Creature *population = (Creature*)malloc(2 * POP_SIZE * sizeof(Creature));
	printf("Creature size: %d bytes, %d creatures per generation\n", sizeof(Creature), POP_SIZE);
	uint32_t memorySize = (POP_SIZE+1) * MAX_INSTRUCTIONS * sizeof(instruction);
	printf("Memory size: %d bytes, %d per creature\n", memorySize, memorySize / (POP_SIZE+1));
	instruction* genePool = (instruction*)malloc(memorySize);

	float bestOfAllTime = 1.0;

	for(int g=0; g<NUM_GENS; g++)
	{
		if(g == 0)
		{
			for(int i=0; i<POP_SIZE; i++)
			{
				population[i].id = i+1;
				population[i].genes = genePool + i*MAX_INSTRUCTIONS;
				for(int j=0; j<MAX_INSTRUCTIONS; j++)
				{
					InstructionInitialize(genePool + i*MAX_INSTRUCTIONS + j, prng);
				}
			}
		}
		else if(g > 0)
		{
			deathAndReproduction(population, genePool, prng, g);
		}
		for(int i=0; i<POP_SIZE; i++)
		{
			// Run the game!
			if((2*population[i].id) / POP_SIZE > g || g==0)
				population[i].fitness = runGame(population[i], 10000);
			if(population[i].fitness > bestOfAllTime * 1.0)
				printf("Creature %03d: %.2f\n", population[i].id, population[i].fitness);
		}
		creatureMergeSortWrapper(population);
		if(population[POP_SIZE-1].fitness > bestOfAllTime)
		{
			bestOfAllTime = population[POP_SIZE-1].fitness;
			fprintInstructionGenome(population[POP_SIZE-1].genes, population[POP_SIZE-1].id);
		}
		printf("Generation %d's top 3 creatures: %03d, %03d, %03d\n", g+1,  population[POP_SIZE-1].id, population[POP_SIZE-2].id, population[POP_SIZE-3].id);
	}
	free(genePool);
	free(population);
	return 0;
}




void deathAndReproduction(Creature *C, instruction *pool, pcg32_random_t *r, int gen)
{
	instruction* tempInstructionData = pool + POP_SIZE * MAX_INSTRUCTIONS;
	// Creatures already sorted by fitness
	for(int i=0; i<POP_SIZE/2; i++)
	{
		int offset1 = (C[POP_SIZE-i-1].genes - pool);
		int offset2 = (C[i].genes - pool);
		if(pcg32_random_f(r) < ((float)(i-5))/((float)(POP_SIZE)))
		{	// Unexpected death
			memcpy(pool + offset1, pool + offset2, MAX_INSTRUCTIONS * sizeof(instruction));
			InstructionGenomeMutate(C[POP_SIZE-i-1].genes, tempInstructionData, r);
			//mutate(&C[POP_SIZE-i-1], r);
			C[POP_SIZE-i-1].id = 1 + i + (POP_SIZE/2)*(gen+1);
		}
		else
		{	// Expected death
			memcpy(pool + offset2, pool + offset1, MAX_INSTRUCTIONS * sizeof(instruction));
			InstructionGenomeMutate(C[i].genes, tempInstructionData, r);
			//mutate(&C[i], r);
			C[i].id = 1 + i + (POP_SIZE/2)*(gen+1);
		}
	}
}


void meiosis(float *child, float *parent1, float *parent2)
{
	
}


void mutate(Creature *C, pcg32_random_t *r)
{
	/*for(int i=0; i<GENE_SIZE; i++)
	{
		if(pcg32_random_r(r) % 256 == 0)
		{
			*(C->genes + i) += pcg32_random_fm03(r);
			if(*(C->genes + i) > 1)
				*(C->genes + i) = 1;
			else if(*(C->genes + i) < -1);
				*(C->genes + i) = -1;
		}
	}*/
}


void creatureMergeSortWrapper(Creature *C)
{
	memcpy(C+POP_SIZE, C, POP_SIZE*sizeof(Creature));
	C = creatureMergeSort(C, C+POP_SIZE, 0, POP_SIZE);
}

Creature* creatureMergeSort(Creature *p1, Creature* p2, int begin, int end)
{
	if(end - begin <= 1)
		return p1+begin;
	// Recursively split the arrays
	int middle = (begin + end) / 2;
	Creature *left = creatureMergeSort(p2, p1, begin, middle);
	Creature *right = creatureMergeSort(p2, p1, middle, end);
	// Merge everything together
	return creatureFinalMerge(p1, left, right, begin, middle, end);
}

Creature* creatureFinalMerge(Creature *R, Creature *A, Creature *B, int begin, int middle, int end)
{
	int size = end-begin;
	int aSize = middle-begin;
	int bSize = end-middle;
	int aCount = 0;
	int bCount = 0;
	for(int i=0; i<size; i++)
	{
		if(aCount >= aSize)
		{
			*(R+begin+i) = *(B+bCount);
			bCount++;
		}
		else if(bCount >= bSize || (B+bCount)->fitness >= (A+aCount)->fitness)
		{
			*(R+begin+i) = *(A+aCount);
			aCount++;
		}
		else
		{
			*(R+begin+i) = *(B+bCount);
			bCount++;
		}
	}
	return R+begin;
}
