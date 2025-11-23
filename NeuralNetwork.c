#include "RushNNincludes.h"

float RationalActivationF(float);
float RationalActivationI(int);
float LogisticActivation(float);

float RationalActivationF(float x)
{
	return 1 - 1 / (0.1*x+1);
}

float RationalActivationI(int x)
{
	return 1 - 1 / (0.1*x+1);
}

float LogisticActivation(float x)
{
	return 1 / (1 + exp(-1*x));
}

char nn_run(struct gameData gd, float* weights, float* biases, int* numNodes, int* numLayers)
{
	// Make arrays for each layer
	float *layer0, *layer1, *layer2, *layerF;
	
	// Normalize game data to 0-1
	float gameFloats[9] = {gd.a, gd.b, gd.c, gd.d, gd.e, gd.f, gd.g, gd.csharp, gd.RUSH};
	for(int i=0; i<9; i++)
	{
		*(layer0 + i) = RationalActivationF(log10(1+gameFloats[i]));
	}
	for(int i=9; i<21; i++)
	{
		*(layer0 + i) = RationalActivationI(gd.u[i-9]);
	}

	// Calculate layer 1
	for(int i = 0; i < *numNodes; i++)
	{
		float acc = 0;
		for(int j = 0; j < 21; j++)
		{
			acc += (*(layer0 + j)) * (*(weights + 21*i + j));
		}
		*(layer1 + i) = LogisticActivation(acc + *(biases + i));
	}

	// Calculate layer 2
	int bOffset = *numNodes;
	int wOffset = bOffset * 21;
	int lastNumNodes = *numNodes;
	for(int i = 0; i < *(numNodes+1); i++)
	{
		float acc = 0;
		for(int j = 0; j < lastNumNodes; j++)
		{
			acc += (*(layer1 + j)) * (*(weights + wOffset + lastNumNodes*i + j));
		}
		*(layer2 + i) = LogisticActivation(acc + *(biases + bOffset + i));
	}
	
	// Calculate final layer
	lastNumNodes = *(numNodes+1);
	bOffset += lastNumNodes;
	wOffset += lastNumNodes * (*numNodes);
	for(int i = 0; i < *(numNodes+2); i++)
	{
		float acc = 0;
		for(int j = 0; j < lastNumNodes; j++)
		{
			acc += (*(layer2 + j)) * (*(weights + wOffset + lastNumNodes*i + j));
		}
		*(layerF + i) = (acc + *(biases + bOffset + i));
	}
	
	// Pick character for final selection
	int maxIndex = -1;
	float maxNum = -1e30;
	for(int i = 0; i < *(numNodes+2); i++)
	{
		if(*(layerF + i) > maxNum)
		{
			maxNum = *(layerF + i);
			maxIndex = i;
		}
	}
	if(maxIndex < 8)
		return 65 + maxIndex;
	else if(maxIndex == 8)
		return 35;
	else if(maxIndex == 9)
		return 82;
	else if(maxIndex < 19)
		return 39 + maxIndex;
	else
		return 98 - maxIndex;

	return 81;
}
