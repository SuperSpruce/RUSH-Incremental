/* This is RUSH Incremental, created by SuperSpruce.
 * 
 * RUSH Incremental is an incremental game.
 * This means that the goal of the game is to make the numbers bigger and unlock new mechanics (upgrades and notes).
 * You can type a letter to gain some quantity of a note, access upgrades, or RUSH.
 * RUSH is the prestige layer of RUSH incremental, resetting your notes and upgrades in exchange for a boost in all note production.
 * You can buy upgrades that boost various aspects of production with notes and RUSHes.
 * There is not a way to "win" this game in a traditional sense, but getting to C#, the last note, is when you get the last thing that the game offers.
 * 
 * */




//Libraries
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "RushNNincludes.h"

#include <omp.h>


//Function prototypes
double boostCalc(double, int);
double rushCalc(struct gameData);
float fitnessCalc(struct gameData, float);
void writeLogLine(FILE*, struct gameData, unsigned int, char, float);


float runGame(Creature specimen, int maxFrames)
{
	//Declare vars
	struct gameData state;
	state.a = 0;
	state.b = 0;
	state.c = 0;
	state.d = 0;
	state.e = 0;
	state.f = 0;
	state.g = 0;
	state.csharp = 0;
	state.rush = 0;
	for(int i=0; i<12; i++)
	{
		state.u[i] = 0;
	}
	state.stuckframes = 0;
	state.remainingframes = 0;
	for(int i=0; i<9; i++)
	{
		state.gain[i] = 0;
	}
	double boost = 1;
	
	//Prepare for game loop
	boost = boostCalc(state.rush, state.u[9]);
	char action = 'A';
	FILE* logfile;
	char path[32];
	int currentGen = 2*specimen.id / POP_SIZE;
	if(currentGen < 1)
		currentGen = 1;
	snprintf(path, sizeof(path), "Runs/i06_10K_1_gen_%05d.txt", currentGen);
	logfile = fopen(path, "a");
	if(logfile == NULL)
	{
		printf("Failed to open the file!");
		return -2;
	}
	unsigned int frames = 0;

	// Creature instruction state stuff
	int iOffset = 0;
	int repsLeft = specimen.genes[0].repitition;

	// Penalty variables
	char lastAction = 'A';
	float sameFramePenalty = -1.0;
	uint32_t actionRecords = 0;
	int deadInstructions = 0;
	
	//Main game loop
	while(frames < maxFrames)
	{
		state.remainingframes = maxFrames - frames;
		
		// Precalculate gains
		state.gain[0] = floor(boost * (1 + state.u[1]/2.0) * (1+(state.u[3]/4.0)*log10(state.b+1)));
		state.gain[1] = floor(boost * state.u[0] * (1+(state.u[3]/4.0)*log10(state.a+1)));
		state.gain[2] = floor(boost * state.u[2]/2.0);
		state.gain[3] = floor(boost * state.u[4]/4.0);
		state.gain[4] = floor(boost * state.u[5] * (1+state.u[10]*log10((10000+state.a+state.b+state.c+state.d+state.f+state.g+state.csharp)/10000)));
		state.gain[5] = floor(boost * state.u[6]);
		state.gain[6] = floor(boost * state.u[8]/10.0);
		state.gain[7] = floor(boost * state.u[11]/100.0);
		state.gain[8] = floor(rushCalc(state));
		
		lastAction = action;
		// Enter action
		action = executeInstructions(specimen.genes, &iOffset, &repsLeft);
		if(action == 'Q')
			break;
		if(frames > 0 && action != lastAction)
			sameFramePenalty = 0.0;

		// The big switch statement
		switch(action)
		{
			case 'A':
				state.a += state.gain[0];
				actionRecords |= 0x000001;
				if(state.gain[0] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'B':
				state.b += state.gain[1];
				actionRecords |= 0x000002;
				if(state.gain[1] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'C':
				state.c += state.gain[2];
				actionRecords |= 0x000004;
				if(state.gain[2] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'D':
				state.d += state.gain[3];
				actionRecords |= 0x000008;
				if(state.gain[3] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'E':
				state.e += state.gain[4];
				actionRecords |= 0x000010;
				if(state.gain[4] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'F':
				state.f += state.gain[5];
				actionRecords |= 0x000020;
				if(state.gain[5] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'G':
				state.g += state.gain[6];
				actionRecords |= 0x000040;
				if(state.gain[6] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case '#':
				state.csharp += state.gain[7];
				actionRecords |= 0x000080;
				if(state.gain[7] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case 'R':
				//RUSH - This is the prestige layer of this game where you reset your progress for a boost in production.
				state.a = 0;
				state.b = 0;
				state.c = 0;
				state.d = 0;
				state.e = 0;
				state.f = 0;
				state.g = 0;
				state.csharp = 0;
				for(int i=0; i<12; i++)
				{
					state.u[i] = 0;
				}
				//Gain the RUSH
				state.rush += state.gain[8];
				boost = boostCalc(state.rush, state.u[9]);
				actionRecords &= 0x000000;
				actionRecords |= 0x000100;
				if(state.gain[8] > 0)
					state.stuckframes = 0;
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				break;
			case '1':
				if(state.a >= 20*pow(2.0, state.u[0]))
				{
					double aGain = -20*pow(2.0, state.u[0]);
					state.a += aGain;
					state.u[0]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x000200;
				break;
			case '2':
				if(state.b >= 20*pow(3.0, state.u[1]))
				{
					double bGain = -20*pow(3.0, state.u[1]);
					state.b += bGain;
					state.u[1]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x000400;
				break;
			case '3':
				if(state.a >= 50*pow(3.0, state.u[2]) && state.b >= 150*pow(3.0, state.u[2]))
				{
					double aGain = -50*pow(3.0, state.u[2]);
					state.a += aGain;
					double bGain = -150*pow(3.0, state.u[2]);
					state.b += bGain;
					state.u[2]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x000800;
				break;
			case '4':
				if(state.a >= 250*pow(2.0, state.u[3]) && state.b >= 500*pow(2.0, state.u[3]) && state.c >= 25*pow(2.0, state.u[3]))
				{
					double aGain = -250*pow(2.0, state.u[3]);
					state.a += aGain;
					double bGain = -500*pow(2.0, state.u[3]);
					state.b += bGain;
					double cGain = -25*pow(2.0, state.u[3]);
					state.c += cGain;
					state.u[3]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x001000;
				break;
			case '5':
				if(state.c >= 100*pow(5.0, state.u[4]) && state.rush >= 1*pow(5.0, state.u[4]))
				{
					double cGain = -100*pow(5.0, state.u[4]);
					state.c += cGain;
					double rushGain = -1*pow(5.0, state.u[4]);
					state.rush += rushGain;
					boost = boostCalc(state.rush, state.u[9]);
					state.u[4]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x002000;
				break;
			case '6':
				if(state.a >= 3000*pow(2.0, state.u[5]) && state.b >= 7000*pow(2.0, state.u[5]) && state.c >= floor(200*pow(1.8, state.u[5])) && state.d >= floor(100*pow(1.75, state.u[5])))
				{
					double aGain = -3000*pow(2.0, state.u[5]);
					state.a += aGain;
					double bGain = -7000*pow(2.0, state.u[5]);
					state.b += bGain;
					double cGain = -floor(200*pow(1.8, state.u[5]));
					state.c += cGain;
					double dGain = -floor(100*pow(1.75, state.u[5]));
					state.d += dGain;
					state.u[5]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x004000;
				break;
			case '7':
				if(state.b >= 20000*pow(2.0, state.u[6]) && state.e >= 2000*pow(3.0, state.u[6]))
				{
					double bGain = -20000*pow(2.0, state.u[6]);
					state.b += bGain;
					double eGain = -2000*pow(3.0, state.u[6]);
					state.e += eGain;
					state.u[6]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x008000;
				break;
			case '8':
				if(state.b >= 32000*pow(3.0, state.u[7]) && state.d >= 1000*pow(2.0, state.u[7]) && state.f >= 4000*pow(2.0,state.u[7]) && state.rush >=100*pow(2.0,state.u[7]))
				{
					double bGain = -32000*pow(3.0, state.u[7]);
					state.b += bGain;
					double dGain = -1000*pow(2.0, state.u[7]);
					state.d += dGain;
					double fGain = -4000*pow(2.0, state.u[7]);
					state.f += fGain;
					double rushGain = -100*pow(2.0, state.u[7]);
					state.rush += rushGain;
					boost = boostCalc(state.rush, state.u[9]);
					state.u[7]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x010000;
				break;
			case '9':
				if(state.e >= 9000*pow(4.0, state.u[8]) && state.f >= 6000*pow(2.0, state.u[8]))
				{
					double eGain = -9000*pow(4.0, state.u[8]);
					state.e += eGain;
					double fGain = -6000*pow(2.0, state.u[8]);
					state.f += fGain;		
					state.u[8]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x020000;
				break;
			case 'O':
				if(state.a >= 100000*pow(3.0, state.u[9]) && state.c >= 6250*pow(2.0, state.u[9]) && state.e >= 4000*pow(5.0,state.u[9]) && state.g >= floor(250*pow(2.2, state.u[9])) && state.rush >= 250*pow(2.0,state.u[9]))
				{
					double aGain = -100000*pow(3.0, state.u[9]);
					state.a += aGain;
					double cGain = -6250*pow(2.0, state.u[9]);
					state.c += cGain;
					double eGain = -4000*pow(5.0, state.u[9]);
					state.e += eGain;
					double gGain = -floor(250*pow(2.2, state.u[9]));
					state.g += gGain;
					double rushGain = -250*pow(2.0, state.u[9]);
					state.rush += rushGain;
					state.u[9]++;
					boost = boostCalc(state.rush, state.u[9]);
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x040000;
				break;
			case 'N':
				if(state.e >= 18000*pow(4.0, state.u[10]) && state.f >= 10000*pow(2.0, state.u[10]) && state.g >= 500*pow(2.2,state.u[10]) && state.rush >= floor(250*pow(1.8,state.u[10])))
				{
					double eGain = -18000*pow(4.0, state.u[10]);
					state.e += eGain;
					double fGain = -10000*pow(2.0, state.u[10]);
					state.f += fGain;
					double gGain = -500*pow(2.2,state.u[10]);
					state.g += gGain;
					double rushGain = -floor(250*pow(1.8,state.u[10]));
					state.rush += rushGain;
					boost = boostCalc(state.rush, state.u[9]);
					state.u[10]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x080000;
				break;
			case 'M':
				if(state.a >= 10000000*pow(2.0, state.u[11]) && state.b >= 25000000*pow(2.0, state.u[11]) && state.c >= floor(800000*pow(1.5, state.u[11])) && state.d >= floor(200000*pow(1.5, state.u[11])) && state.e >= 50000000*pow(2.0, state.u[11]) && state.f >= floor(1000000*pow(1.5, state.u[11])) && state.g >= floor(50000*pow(1.5, state.u[11])) && state.rush >= floor(10000*pow(1.5, state.u[11])))
				{
					double aGain = -10000000*pow(2.0, state.u[11]);
					state.a += aGain;
					double bGain = -25000000*pow(2.0, state.u[11]);
					state.b += bGain;
					double cGain = -floor(800000*pow(1.5, state.u[11]));
					state.c += cGain;
					double dGain = -floor(200000*pow(1.5, state.u[11]));
					state.d += dGain;
					double eGain = -50000000*pow(2.0, state.u[11]);
					state.e += eGain;
					double fGain = -floor(1000000*pow(1.5, state.u[11]));
					state.f += fGain;
					double gGain = -floor(50000*pow(1.5, state.u[11]));
					state.g += gGain;
					double rushGain = -floor(10000*pow(1.5, state.u[11]));
					state.rush += rushGain;
					boost = boostCalc(state.rush, state.u[9]);
					state.u[11]++;
					state.stuckframes = 0;
				}
				else {
					state.stuckframes++;
					if(lastAction != action)
						deadInstructions++;
				}
				actionRecords |= 0x100000;
				break;
		}
		frames++;
	}
	state.stuckframes = frames;
	writeLogLine(logfile, state, (unsigned int)specimen.id, action, deadInstructions);
	fclose(logfile);
	return fitnessCalc(state, deadInstructions);
}


//Calculates how much RUSH you get
double rushCalc(struct gameData gd)
{
	
	double rGain = log10((gd.a+10)*(gd.b+10)*(gd.c+10)*(gd.d+10)*(gd.e+10)*(gd.f+10)*(gd.g+10)*pow((gd.csharp+10), 3)/1e10);
	return (pow(1+gd.u[7]/4.0, 2) * pow(rGain, 1+log10(1+rGain)));
}

// Calculates the fitness score
float fitnessCalc(struct gameData gd, float penalty)
{
	float f = (float)rushCalc(gd);
	float bonus = 0;
	float steel = 1.0;
	for(int i=0; i<12; i++)
		bonus += (i+1)*sqrt(gd.u[i]);
	if(gd.b > 0)
		steel *= 1.25;
	if(gd.c > 0)
		steel *= 1.25;
	if(gd.d > 0)
		steel *= 1.25;
	if(gd.e > 0)
		steel *= 1.25;
	if(gd.f > 0)
		steel *= 1.25;
	if(gd.g > 0)
		steel *= 1.25;
	if(gd.csharp > 0)
		steel *= 1.25;
	return (gd.rush + f + bonus) * steel * pow(0.99, penalty);
}

//Calculates the boost from RUSH
double boostCalc(double r, int level)
{
	double RUSH = r;
	return (1 + (0.25 + level/8.0) * pow(RUSH, 0.80-0.20*(1-pow(1+RUSH, -0.50))+0.40*(1-pow(1+RUSH, -0.025))));
}

// Writes a line to the logfile
void writeLogLine(FILE* file, struct gameData gd, unsigned int cid, char act, float penalty)
{
	fprintf(file, "%07u,%c,%e,%e,%e,%e,%e,%e.%e,%e,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%02i,%05d,%e,%.03f\n", cid, act, gd.a, gd.b, gd.c, gd.d, gd.e, gd.f, gd.g, gd.csharp, gd.u[0], gd.u[1], gd.u[2], gd.u[3], gd.u[4], gd.u[5], gd.u[6], gd.u[7], gd.u[8], gd.u[9], gd.u[10], gd.u[11], gd.stuckframes, gd.rush, fitnessCalc(gd, penalty)); 
}
