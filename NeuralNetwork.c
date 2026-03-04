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

<<<<<<< HEAD
char nn_run(struct gameData gd, float* weights, float* biases, int* numNodes, int* numLayers)
{
	// Make arrays for each layer
	float *layer0, *layer1, *layer2, *layerF;
	
	// Normalize game data to 0-1
	float gameFloats[9] = {gd.a, gd.b, gd.c, gd.d, gd.e, gd.f, gd.g, gd.csharp, gd.RUSH};
=======
char nn_run(struct gameData gd, float* weights, float* biases, int* numNodes, char lastAction)
{
	// Make arrays for each layer
	float *layer0, *layer1, *layer2, *layerF;
	int totalNodes = 32 + nn_topo[0] + nn_topo[1] + nn_topo[2];
	layer0 = (float*)malloc(totalNodes * sizeof(float));
	layer1 = layer0 + 32;
	layer2 = layer0 + 32 + nn_topo[0];
	layerF = layer0 + 32 + nn_topo[0] + nn_topo[1];

	// Translate last action into neural network output
	int lastIndex = -1;
	if(lastAction == 35)
		lastIndex = 7;
	else if(lastAction < 58)
		lastIndex = lastAction - 40;
	else if(lastAction < 72)
		lastIndex = lastAction - 65;
	else if(lastAction == 82)
		lastIndex = 8;
	else if(lastAction > 76)
		lastIndex = 97 - lastAction;
	
	// Normalize game data to 0-1
	float gameFloats[9] = {gd.a, gd.b, gd.c, gd.d, gd.e, gd.f, gd.g, gd.csharp, gd.rush};
>>>>>>> master
	for(int i=0; i<9; i++)
	{
		*(layer0 + i) = RationalActivationF(log10(1+gameFloats[i]));
	}
	for(int i=9; i<21; i++)
	{
		*(layer0 + i) = RationalActivationI(gd.u[i-9]);
	}
<<<<<<< HEAD
=======
	*(layer0+21) = RationalActivationI(gd.stuckframes);
	*(layer0+22) = RationalActivationI(gd.remainingframes);
	for(int i=23; i<32; i++)
	{
		*(layer0 + i) = RationalActivationF(log10(1+gd.gain[i-23]));
	}
>>>>>>> master

	// Calculate layer 1
	for(int i = 0; i < *numNodes; i++)
	{
		float acc = 0;
<<<<<<< HEAD
		for(int j = 0; j < 21; j++)
		{
			acc += (*(layer0 + j)) * (*(weights + 21*i + j));
		}
		*(layer1 + i) = LogisticActivation(acc + *(biases + i));
	}

	// Calculate layer 2
	int bOffset = *numNodes;
	int wOffset = bOffset * 21;
=======
		for(int j = 0; j < 32; j++)
		{	
			acc += (*(layer0 + j)) * (*(weights + 32*i + j));
		}
		*(layer1 + i) = LogisticActivation(acc + *(biases + i));
	}
	
	// Calculate layer 2
	int bOffset = *numNodes;
	int wOffset = bOffset * 32;
>>>>>>> master
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
<<<<<<< HEAD
=======
		if(i == lastAction)
			*(layerF + i) += 0.10;
>>>>>>> master
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
<<<<<<< HEAD
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
=======

	free(layer0);
	
	// Translate neural network max into char
	if(maxIndex < 7)
		return 65 + maxIndex;
	else if(maxIndex == 7)
		return 35;
	else if(maxIndex == 8)
		return 82;
	else if(maxIndex < 18)
		return 40 + maxIndex;
	else
		return 97 - maxIndex;
>>>>>>> master

	return 81;
}
