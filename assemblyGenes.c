#include <stdbool.h>
#include <string.h>
#include "RushNNincludes.h"

bool validAction(char);
void instructionModify(instruction*, pcg32_random_t* rng);

char executeInstructions(instruction* firstInstruction, int* pOffset, int* numFrames)
{
	if(*pOffset >= MAX_INSTRUCTIONS)
		*pOffset = 0;
	instruction* currentInstruction = firstInstruction + *pOffset;
	if((*numFrames) > 0)
	{
		*(numFrames) -= 1;
		return currentInstruction->action;
	}
	(*pOffset)++;
	if(*pOffset >= MAX_INSTRUCTIONS)
		return 81;
		//*pOffset = 0;
	currentInstruction = firstInstruction + *pOffset;
	*numFrames = currentInstruction->repitition;
	*(numFrames) -= 1;
	return currentInstruction->action;
}

void printInstruction(instruction* i)
{
	printf("%c %03d\n", i->action, i->repitition);
}

void fprintInstructionGenome(instruction* base, int cid)
{
	FILE* f;
	char ipath[32];
	snprintf(ipath, sizeof(ipath), "Genomes/i05_128_1_%07d.txt", cid);
	f = fopen(ipath, "w");
	if(f == NULL)
	{
		printf("Warning: Failed to open genome file");
		return;
	}
	instruction* pInstruction;
	for(int i = 0; i < MAX_INSTRUCTIONS; i++)
	{
		pInstruction = base + i;
		fprintf(f, "%c %03d\n", pInstruction->action, pInstruction->repitition);
	}
	fclose(f);
}

bool validAction(char c)
{
	if(c >= 48 && c < 58) return true;
	else if(c >= 65 && c < 72) return true;
	else if(c >= 77 && c < 80) return true;
	else if(c == 35 || c == 82) return true;
	else return false;
}

void InstructionInitialize(instruction* ins, pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	uint32_t m1 = bits & 0x000000FF;
	uint32_t m2 = bits & 0x0000FF00;
	uint32_t m3 = (bits & 0x00FF0000) >> 16;

	ins->repitition = (uint8_t)((m1*m2*m3) >> 24);
	if(ins->repitition < 1)
		ins->repitition = 1;

	int ract = ((bits & 0xFF000000) >> 24) % 21;
	if(ract < 7)
		ins->action = (char)(65 + ract);
	else if(ract == 7)
		ins->action = 35;
	else if(ract == 8)
		ins->action = 82;
	else if(ract < 18)
		ins->action = (char)(40 + ract);
	else
		ins->action = (char)(97 - ract);
}

void InstructionModify(instruction* ins, pcg32_random_t* rng)
{
	uint32_t bits = pcg32_random_r(rng);
	uint32_t highBits = bits >> 24;
	uint32_t lowBits = bits &= 0x0000FFFF;
	uint32_t rep = (uint32_t)(ins->repitition);

	rep *= lowBits;
	rep >>= 15;
	ins->repitition = (uint8_t)rep;
	if(ins->repitition < 1)
		ins->repitition = 1;
	
	char act = (char)highBits;
	if(validAction(act))
		ins->action = act;
}

void InstructionGenomeMutate(instruction* oldBase, instruction* newBase, pcg32_random_t* rng)
{
	instruction* oldCurrent = oldBase;
	instruction* newCurrent = newBase;
	instruction* randomCurrent;
	int state = 0;  // 0 = normal state, 1 = deletion state, 2 = insertion state
	while(newCurrent - newBase < MAX_INSTRUCTIONS)
	{
		uint32_t bits = pcg32_random_r(rng);
		if(oldCurrent - oldBase >= MAX_INSTRUCTIONS)
		{
			InstructionInitialize(newCurrent, rng);
			newCurrent++;
		}
		else if(state == 0)
		{
			*newCurrent = *oldCurrent;
			if(bits % 32 == 0)
				InstructionModify(newCurrent, rng);
			oldCurrent++;
			newCurrent++;
			if(bits & 0x0001FF00 == 0)
				state = 1;
			else if(bits & 0x03FE0000 == 0)
			{
				state = 2;
				bits = pcg32_random_r(rng);
				randomCurrent = oldBase + (bits % MAX_INSTRUCTIONS);
			}
		}
		else if(state == 1)
		{
			oldCurrent++;
			if(bits % 2 == 0)
				state = 0;
		}
		else if(state == 2)
		{
			*newCurrent = *randomCurrent;
			if(bits % 256 == 0)
				InstructionModify(newCurrent, rng);
			newCurrent++;
			randomCurrent++;
			if((bits & 0x80000000) == 0 || randomCurrent - oldBase >= MAX_INSTRUCTIONS)
				state = 0;
			else if((bits & 0x00FF0000) == 0)
				state = 1;
		}
		else
			state = 0;
	}
	memcpy(oldBase, newBase, MAX_INSTRUCTIONS*sizeof(instruction));
}
