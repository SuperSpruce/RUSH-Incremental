#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "pcg_random.h"

extern int nn_topo[];
#define MAX_INSTRUCTIONS 128
#define GENE_SIZE 2571
#define WEIGHT_SIZE 2490
#define POP_SIZE 900 

typedef struct
{
	char action;
	uint8_t repitition;
} instruction;

typedef struct
{
	int id;
	float fitness;
	instruction* genes;
} Creature;

struct gameData
{
	double a;
	double b;
	double c;
	double d;
	double e;
	double f;
	double g;
	double csharp;
	double rush;
	int u[12];
	int stuckframes;
	int remainingframes;
	double gain[9];
};

float runGame(Creature, int);

char executeInstructions(instruction*, int*, int*);
void printInstruction(instruction*);
void fprintInstructionGenome(instruction*, int);
void InstructionInitialize(instruction*, pcg32_random_t*);
void InstructionGenomeMutate(instruction*, instruction*, pcg32_random_t*);

//char nn_run(struct gameData, float*, float*, int*, char);
