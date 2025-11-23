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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "RushNNincludes.h"


//Function prototypes
double boostCalc(double, int);
void writeLogLine(FILE);


float runGame(int cid, int maxFrames)
{
	//Declare vars
	gameData state = {
	.a = 0;
	.b = 0;
	.c = 0;
	.d = 0;
	.e = 0;
	.f = 0;
	.g = 0;
	.csharp = 0;
	.rush = 0;
	for(int i=0; i<12; i++)
	{
		.state.u[i] = 0;
	}
	double boost = 1;
	double aGain;
	double bGain;
	double cGain;
	double dGain;
	double eGain;
	double fGain;
	double gGain;
	double csharpGain;
	double rushGain;
	};
	
	//Prepare for game loop
	boost = boostCalc(state.rush, state.u[9]);
	char action;
	int upgradeChoice = -1;
	int frames = 0;


	//Main game loop
	while(frames < maxFrames)
	{
	//Enter action
	nn_run(gameData, 0, 0, 0, 3);
		switch(action)
		{
			case 'A':
				aGain = floor(boost * (1 + state.u[1]/2.0) * (1+(state.u[3]/4.0)*log10(state.b+1)));
				state.a += aGain;
				break;
			case 'B':
				bGain = floor(boost * state.u[0] * (1+(state.u[3]/4.0)*log10(state.a+1)));
				state.b += bGain;
				break;
			case 'C':
				cGain = floor(boost * state.u[2]/2.0);
				state.c += cGain;
				break;
			case 'D':
				dGain = floor(boost * state.u[4]/4.0);
				state.d += dGain;
				break;
			case 'E':
				eGain = floor(boost * state.u[5] * (1+state.u[10]*log10((10000+state.a+state.b+state.c+state.d+state.f+state.g+state.csharp)/10000)));
				state.e += eGain;
				break;
			case 'F':
				fGain = floor(boost * state.u[6]);
				state.f += fGain;
				break;
			case 'G':
				gGain = floor(boost * state.u[8]/10.0);
				state.g += gGain;
				break;
			case '#':
				csharpGain = floor(boost * state.u[11]/100.0);
				csharp += csharpGain;
				break;
			case 'R':
				//RUSH - This is the prestige layer of this game where you reset your progress for a boost in production.
				//Calculates RUSH gain
				rushGain = log10((state.a+10)*(state.b+10)*(state.c+10)*(state.d+10)*(state.e+10)*(state.f+10)*(state.g+10)*pow((state.csharp+10), 3)/1e10);
				rushGain = floor(pow(1+state.u[7]/4.0, 2) * pow(rushGain, 1+log10(1+rushGain)));
				}
				//Resets everything else
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
				state.rush += rushGain;
				boost = boostCalc(state.rush, state.u[9]);
				printf("%+.0lf RUSH!\nNote production is now boosted by %.2lfx.\n\n", rushGain, boost);
				break;
				case '1':
					if(state.a >= 20*pow(2.0, state.u[0]))
					{
						aGain = -20*pow(2.0, state.u[0]);
						state.a += aGain;
						state.u[0]++;
					}
					break;
				case '2':
					if(state.b >= 20*pow(3.0, state.u[1]))
					{
						bGain = -20*pow(3.0, state.u[1]);
						state.b += bGain;
						state.u[1]++;
					}
					break;
				case '3':
					if(state.a >= 50*pow(3.0, state.u[2]) && state.b >= 150*pow(3.0, state.u[2]))
					{
						aGain = -50*pow(3.0, state.u[2]);
						state.a += aGain;
						bGain = -150*pow(3.0, state.u[2]);
						state.b += bGain;
						state.u[2]++;
					}
					break;
				case '4':
					if(state.a >= 250*pow(2.0, state.u[3]) && state.b >= 500*pow(2.0, state.u[3]) && state.c >= 25*pow(2.0, state.u[3]))
					{
						aGain = -250*pow(2.0, state.u[3]);
						state.a += aGain;
						bGain = -500*pow(2.0, state.u[3]);
						state.b += bGain;
						cGain = -25*pow(2.0, state.u[3]);
						state.c += cGain;
						state.u[3]++;
					}
					break;
				case '5':
					if(state.c >= 100*pow(5.0, state.u[4]) && state.rush >= 1*pow(5.0, state.u[4]))
					{
						cGain = -100*pow(5.0, state.u[4]);
						state.c += cGain;
						rushGain = -1*pow(5.0, state.u[4]);
						state.rush += rushGain;
						boost = boostCalc(state.rush, state.u[9]);
						state.u[4]++;
					}
					break;
				case '6':
					if(state.a >= 3000*pow(2.0, state.u[5]) && state.b >= 7000*pow(2.0, state.u[5]) && state.c >= floor(200*pow(1.8, state.u[5])) && state.d >= floor(100*pow(1.75, state.u[5])))
					{
						aGain = -3000*pow(2.0, state.u[5]);
						state.a += aGain;
						bGain = -7000*pow(2.0, state.u[5]);
						state.b += bGain;
						cGain = -floor(200*pow(1.8, state.u[5]));
						state.c += cGain;
						dGain = -floor(100*pow(1.75, state.u[5]));
						state.d += dGain;
						state.u[5]++;
					}
					break;
				case '7':
					if(state.b >= 20000*pow(2.0, state.u[6]) && state.e >= 2000*pow(3.0, state.u[6]))
					{
						bGain = -20000*pow(2.0, state.u[6]);
						state.b += bGain;
						eGain = -2000*pow(3.0, state.u[6]);
						state.e += eGain;
						state.u[6]++;
					}
					break;
				case '8':
					if(state.b >= 32000*pow(3.0, state.u[7]) && state.d >= 1000*pow(2.0, state.u[7]) && state.f >= 4000*pow(2.0,state.u[7]) && state.rush >=100*pow(2.0,state.u[7]))
					{
						bGain = -32000*pow(3.0, state.u[7]);
						state.b += bGain;
						dGain = -1000*pow(2.0, state.u[7]);
						state.d += dGain;
						fGain = -4000*pow(2.0, state.u[7]);
						state.f += fGain;
						rushGain = -100*pow(2.0, state.u[7]);
						state.rush += rushGain;
						boost = boostCalc(state.rush, state.u[9]);
						state.u[7]++;
					}
					break;
				case '9':
					if(state.e >= 9000*pow(4.0, state.u[8]) && state.f >= 6000*pow(2.0, state.u[8]))
					{
						eGain = -9000*pow(4.0, state.u[8]);
						state.e += eGain;
						fGain = -6000*pow(2.0, state.u[8]);
						state.f += fGain;		
						state.u[8]++;
					}
					break;
				case 'O':
					if(state.a >= 100000*pow(3.0, state.u[9]) && state.c >= 6250*pow(2.0, state.u[9]) && state.e >= 4000*pow(5.0,state.u[9]) && state.g >= floor(250*pow(2.2, state.u[9])) && state.rush >= 250*pow(2.0,state.u[9]))
					{
						aGain = -100000*pow(3.0, state.u[9]);
						state.a += aGain;
						cGain = -6250*pow(2.0, state.u[9]);
						state.c += cGain;
						eGain = -4000*pow(5.0, state.u[9]);
						state.e += eGain;
						gGain = -floor(250*pow(2.2, state.u[9]));
						state.g += gGain;
						rushGain = -250*pow(2.0, state.u[9]);
						state.rush += rushGain;
						state.u[9]++;
						boost = boostCalc(state.rush, state.u[9]);
					}
					break;
				case 'N':
					if(state.e >= 18000*pow(4.0, state.u[10]) && state.f >= 10000*pow(2.0, state.u[10]) && state.g >= 500*pow(2.2,state.u[10]) && state.rush >= floor(250*pow(1.8,state.u[10])))
					{
						eGain = -18000*pow(4.0, state.u[10]);
						state.e += eGain;
						fGain = -10000*pow(2.0, state.u[10]);
						state.f += fGain;
						gGain = -500*pow(2.2,state.u[10]);
						state.g += gGain;
						rushGain = -floor(250*pow(1.8,state.u[10]));
						state.rush += rushGain;
						boost = boostCalc(state.rush, state.u[9]);
						state.u[10]++;
					}
					break;
				case 'M':
					if(state.a >= 10000000*pow(2.0, state.u[11]) && state.b >= 25000000*pow(2.0, state.u[11]) && state.c >= floor(800000*pow(1.5, state.u[11])) && state.d >= floor(200000*pow(1.5, state.u[11])) && state.e >= 50000000*pow(2.0, state.u[11]) && state.f >= floor(1000000*pow(1.5, state.u[11])) && state.g >= floor(50000*pow(1.5, state.u[11])) && state.rush >= floor(10000*pow(1.5, state.u[11])))
					{
						aGain = -10000000*pow(2.0, state.u[11]);
						state.a += aGain;
						bGain = -25000000*pow(2.0, state.u[11]);
						state.b += bGain;
						cGain = -floor(800000*pow(1.5, state.u[11]));
						state.c += cGain;
						dGain = -floor(200000*pow(1.5, state.u[11]));
						state.d += dGain;
						eGain = -50000000*pow(2.0, state.u[11]);
						state.e += eGain;
						fGain = -floor(1000000*pow(1.5, state.u[11]));
						state.f += fGain;
						gGain = -floor(50000*pow(1.5, state.u[11]));
						state.g += gGain;
						rushGain = -floor(10000*pow(1.5, state.u[11]));
						state.rush += rushGain;
						boost = boostCalc(state.rush, state.u[9]);
						state.u[11]++;
					}
					break;
			}
		}
		
	}
}



//Calculates the boost from RUSH
double boostCalc(double r, int level)
{
	double RUSH = r;
	return (1 + (0.25 + level/8.0) * pow(RUSH, 0.80-0.20*(1-pow(1+RUSH, -0.50))+0.40*(1-pow(1+RUSH, -0.025))));
}
