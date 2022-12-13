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


//Function prototypes
void save(double, double, double, double, double, double, double, double, double, int[], int);
double boostCalc(double, int);


int main()
{
	//Declare vars
	double a = 0;
	double b = 0;
	double c = 0;
	double d = 0;
	double e = 0;
	double f = 0;
	double g = 0;
	double csharp = 0;
	double rush = 0;
	int numUpgrades = 12;
	int u[12];
	for(int i=0; i<numUpgrades; i++)
	{
		u[i] = 0;
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
	
	//Loads the game
	FILE *savegame;
	savegame = fopen("RushIncrementalSave.txt", "r");
	fscanf(savegame, "%lf", &a);
	fscanf(savegame, "%lf", &b);
	fscanf(savegame, "%lf", &c);
	fscanf(savegame, "%lf", &d);
	fscanf(savegame, "%lf", &e);
	fscanf(savegame, "%lf", &f);
	fscanf(savegame, "%lf", &g);
	fscanf(savegame, "%lf", &csharp);
	fscanf(savegame, "%lf", &rush);
	for(int i=0; i<numUpgrades; i++)
	{
		fscanf(savegame, "%d", &u[i]);
	}
	fclose(savegame);
	
	//Prepare for game loop
	boost = boostCalc(rush, u[9]);
	char action[64];
	action[0] = '.';
	int upgradeChoice = -1;
	char areYouSure = '.';
	printf("Welcome to RUSH Incremental!\n\n");
	
	//Main game loop
	while(action[0] != 'q' && action[0] != 'Q')
	{
		//Print info
		printf("\nEnter a letter from A-G to gain some quantity of that note.\n");
		if(u[8] > 0)
			printf("Enter # to gain some quantity of C#.\n");
		printf("Enter u to open the upgrades menu.\n");
		printf("Enter r to RUSH: reset the game for a boost in note production.\n");
		printf("Enter q to quit the game.\n");
		
		printf("A = %.0lf\n", a);
		printf("B = %.0lf\n", b);
		printf("C = %.0lf\n", c);
		printf("D = %.0lf\n", d);
		printf("E = %.0lf\n", e);
		printf("F = %.0lf\n", f);
		printf("G = %.0lf\n", g);
		if(u[8] > 0)
			printf("C# = %.0lf\n", csharp);
		printf("RUSH = %.0lf, boosting all note production by %.2lfx.\n", rush, boost);
		
		//Enter action
		scanf("%s", &action);
		if(action[1] == '\0')
		{
			switch(action[0])
			{
				case 'a':
				case 'A':
					aGain = floor(boost * (1 + u[1]/2.0) * (1+(u[3]/4.0)*log10(b+1)));
					a += aGain;
					printf("%+.0lf A\n\n", aGain);
					break;
				case 'b':
				case 'B':
					bGain = floor(boost * u[0] * (1+(u[3]/4.0)*log10(a+1)));
					b += bGain;
					printf("%+.0lf B\n\n", bGain);
					break;
				case 'c':
				case 'C':
					cGain = floor(boost * u[2]/2.0);
					c += cGain;
					printf("%+.0lf C\n\n", cGain);
					break;
				case 'd':
				case 'D':
					dGain = floor(boost * u[4]/4.0);
					d += dGain;
					printf("%+.0lf D\n\n", dGain);
					break;
				case 'e':
				case 'E':
					eGain = floor(boost * u[5] * (1+u[10]*log10((10000+a+b+c+d+f+g+csharp)/10000)));
					e += eGain;
					printf("%+.0lf E\n\n", eGain);
					break;
				case 'f':
				case 'F':
					fGain = floor(boost * u[6]);
					f += fGain;
					printf("%+.0lf F\n\n", fGain);
					break;
				case 'g':
				case 'G':
					gGain = floor(boost * u[8]/10.0);
					g += gGain;
					printf("%+.0lf G\n\n", gGain);
					break;
				case '#':
					csharpGain = floor(boost * u[11]/100.0);
					csharp += csharpGain;
					printf("%+.0lf C#\n\n", csharpGain);
					break;
				case 'r':
				case 'R':
					//RUSH - This is the prestige layer of this game where you reset your progress for a boost in production.
					//Calculates RUSH gain
					rushGain = log10((a+10)*(b+10)*(c+10)*(d+10)*(e+10)*(f+10)*(g+10)*pow((csharp+10), 3)/1e10);
					rushGain = floor(pow(1+u[7]/4.0, 2) * pow(rushGain, 1+log10(rushGain)));
					//Asks if you actually want to do this and error checks
					while(areYouSure != 'y' && areYouSure != 'Y' && areYouSure != 'N' && areYouSure != 'n')
					{
						printf("\nAre you sure you want to RUSH? You will lose all notes and upgrades to gain %+.0lf RUSH! y/n\n", rushGain);
						scanf(" %c", &areYouSure);
					}
					if(areYouSure == 'n' || areYouSure == 'N')
					{
						areYouSure = '.';
						break;
					}
					areYouSure = '.';
					//Resets everything else
					a = 0;
					b = 0;
					c = 0;
					d = 0;
					e = 0;
					f = 0;
					g = 0;
					csharp = 0;
					for(int i=0; i<numUpgrades; i++)
					{
						u[i] = 0;
					}
					//Gain the RUSH
					rush += rushGain;
					boost = boostCalc(rush, u[9]);
					printf("%+.0lf RUSH!\nNote production is now boosted by %.2lfx.\n\n", rushGain, boost);
					break;
				case 'u':
				case 'U':
					//Opens the upgrades menu.
					do
					{
						//Prints upgrade info
						printf("Enter a number from 1 to %d to choose an upgrade to buy.\nEntering 0 lets you see the current effect of your upgrades.\nAny other number exits the upgrades menu.\n\n", numUpgrades);
						printf("Upgrade 1 (lvl %d): B gain is multiplied by [lvl]. Cost: %.0lfA\n", u[0], 20*pow(2.0, u[0]));
						printf("Upgrade 2 (lvl %d): A gain is multiplied by (1+[lvl]/2). Cost: %.0lfB\n", u[1], 20*pow(3.0, u[1]));
						printf("Upgrade 3 (lvl %d): C gain is multiplied by [lvl]/2. Cost: %.0lfA, %.0lfB\n", u[2], 50*pow(3.0, u[2]), 150*pow(3.0, u[2]));
						printf("Upgrade 4 (lvl %d): A and B gain is multiplied by 1+([lvl]/4*(log10(1+[other note]))). Cost: %.0lfA, %.0lfB, %.0lfC\n", u[3], 250*pow(2.0, u[3]), 500*pow(2.0, u[3]), 25*pow(2.0, u[3]));
						printf("Upgrade 5 (lvl %d): D gain is multiplied by [lvl]/4. Cost: %.0lfC, %.0lf RUSH\n", u[4], 100*pow(5.0, u[4]), 1*pow(5.0, u[4]));
						printf("Upgrade 6 (lvl %d): E gain is multiplied by [lvl]. Cost: %.0lfA, %.0lfB, %.0lfC, %.0lfD\n", u[5], 3000*pow(2.0, u[5]), 7000*pow(2.0, u[5]), floor(200*pow(1.8, u[5])), floor(100*pow(1.75,u[5])));
						if(u[2]>0) printf("Upgrade 7 (lvl %d): F gain is multiplied by [lvl]. Cost: %.0lfB, %.0lfE\n", u[6], 20000*pow(2.0, u[6]), 2000*pow(3.0, u[6]));
						if(u[4]>0) printf("Upgrade 8 (lvl %d): RUSH gain is multiplied by ((1+[lvl]/4)^2). Cost: %.0lfB, %.0lfD, %.0lfF, %.0lf RUSH\n", u[7], 32000*pow(3.0, u[7]), 1000*pow(2.0, u[7]), 4000*pow(2.0,u[7]), 100*pow(2.0,u[7]));
						if(u[5]>0) printf("Upgrade 9 (lvl %d): G gain is multiplied by [lvl]/10. Cost: %.0lfE, %.0lfF\n", u[8], 9000*pow(4.0, u[8]), 6000*pow(2.0, u[8]));
						if(u[6]>0)
						{
							printf("Upgrade 10 (lvl %d): RUSH effect is multiplied by (1+[lvl]/2). Cost: %.0lfA, %.0lfC, %.0lfE, %.0lfG, %.0lf RUSH\n", u[9], 100000*pow(3.0, u[9]), 6250*pow(2.0, u[9]), 4000*pow(5.0, u[9]), floor(250*pow(2.2, u[9])), 250*pow(2.0, u[9]));
							printf("Upgrade 11 (lvl %d): E gain is multiplied by (1+[lvl]*(log10((10000+[All notes but E])/10000))). Cost: %.0lfE, %.0lfF, %.0lfG, %.0lf RUSH\n", u[10], 18000*pow(4.0, u[10]), 10000*pow(2.0, u[10]), 500*pow(2.0, u[10]), floor(250*pow(1.8, u[10])));
						}
						if(u[8]>0) printf("Upgrade 12 (lvl %d): C# gain is multiplied by [lvl]/100. Cost: %.0lfA, %.0lfB, %.0lfC, %.0lfD, %.0lfE, %.0lfF, %.0lfG, %.0lf RUSH\n", u[11], 10000000*pow(2.0, u[11]), 25000000*pow(2.0, u[11]), floor(800000*pow(1.5, u[11])), floor(200000*pow(1.5, u[11])), 50000000*pow(2.0, u[11]), floor(1000000*pow(1.5, u[11])), floor(50000*pow(1.5, u[11])), floor(10000*pow(1.5, u[11])));
						//Performs action based on user input
						scanf("%d", &upgradeChoice);
						switch(upgradeChoice)
						{
							case 0:
								printf("Upgrade 1 (lvl %d): B gain is multiplied by %dx.\n", u[0], u[0]);
								printf("Upgrade 2 (lvl %d): A gain is multiplied by %.1fx.\n", u[1], 1+u[1]/2.0);
								printf("Upgrade 3 (lvl %d): C gain is multiplied by %.1fx.\n", u[2], u[2]/2.0);
								printf("Upgrade 4 (lvl %d): A gain is multiplied by %.2fx, B gain is multiplied by %.2fx.\n", u[3], 1+(u[3]/4.0)*log10(b+1), 1+(u[3]/4.0)*log10(a+1));
								printf("Upgrade 5 (lvl %d): D gain is multiplied by %.2fx.\n", u[4], u[4]/4.0);
								printf("Upgrade 6 (lvl %d): E gain is multiplied by %dx.\n", u[5], u[5]);
								if(u[2]>0) printf("Upgrade 7 (lvl %d): F gain is multiplied by %dx.\n", u[6], u[6]);
								if(u[4]>0) printf("Upgrade 8 (lvl %d): RUSH gain is multiplied by %.2lfx.\n", u[7], pow(1+u[7]/4.0, 2));
								if(u[5]>0) printf("Upgrade 9 (lvl %d): G gain is multiplied by %.1lfx.\n", u[8], u[8]/10.0);
								if(u[6]>0)
								{
									printf("Upgrade 10 (lvl %d): RUSH effect is multiplied by %.1lfx.\n", u[9], 1+u[9]/2.0);
									printf("Upgrade 11 (lvl %d): E gain is multiplied by %.2lfx.\n", u[10], 1+u[10]*log10((10000+a+b+c+d+f+g+csharp)/10000));
								}
								if(u[8]>0) printf("Upgrade 12 (lvl %d): C# gain is multiplied by %.2lfx.\n", u[11], u[11]/100.0);
								break;
							case 1:
								if(a >= 20*pow(2.0, u[0]))
								{
									aGain = -20*pow(2.0, u[0]);
									a += aGain;
									printf("%+.0lf A\n\n", aGain);
									u[0]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 2:
								if(b >= 20*pow(3.0, u[1]))
								{
									bGain = -20*pow(3.0, u[1]);
									b += bGain;
									printf("%+.0lf B\n\n", bGain);
									u[1]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 3:
								if(a >= 50*pow(3.0, u[2]) && b >= 150*pow(3.0, u[2]))
								{
									aGain = -50*pow(3.0, u[2]);
									a += aGain;
									printf("%+.0lf A\n", aGain);
									bGain = -150*pow(3.0, u[2]);
									b += bGain;
									printf("%+.0lf B\n\n", bGain);
									u[2]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 4:
								if(a >= 250*pow(2.0, u[3]) && b >= 500*pow(2.0, u[3]) && c >= 25*pow(2.0, u[3]))
								{
									aGain = -250*pow(2.0, u[3]);
									a += aGain;
									printf("%+.0lf A\n", aGain);
									bGain = -500*pow(2.0, u[3]);
									b += bGain;
									printf("%+.0lf B\n", bGain);
									cGain = -25*pow(2.0, u[3]);
									c += cGain;
									printf("%+.0lf C\n\n", cGain);
									u[3]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 5:
								if(c >= 100*pow(5.0, u[4]) && rush >= 1*pow(5.0, u[4]))
								{
									cGain = -100*pow(5.0, u[4]);
									c += cGain;
									printf("%+.0lf C\n", cGain);
									rushGain = -1*pow(5.0, u[4]);
									rush += rushGain;
									boost = boostCalc(rush, u[9]);
									printf("%+.0lf RUSH\n\n", rushGain);
									u[4]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 6:
								if(a >= 3000*pow(2.0, u[5]) && b >= 7000*pow(2.0, u[5]) && c >= floor(200*pow(1.8, u[5])) && d >= floor(100*pow(1.75, u[5])))
								{
									aGain = -3000*pow(2.0, u[5]);
									a += aGain;
									printf("%+.0lf A\n", aGain);
									bGain = -7000*pow(2.0, u[5]);
									b += bGain;
									printf("%+.0lf B\n", bGain);
									cGain = -floor(200*pow(1.8, u[5]));
									c += cGain;
									printf("%+.0lf C\n", cGain);
									dGain = -floor(100*pow(1.75, u[5]));
									d += dGain;
									printf("%+.0lf D\n\n", dGain);
									u[5]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 7:
								if(b >= 20000*pow(2.0, u[6]) && e >= 2000*pow(3.0, u[6]))
								{
									bGain = -20000*pow(2.0, u[6]);
									b += bGain;
									printf("%+.0lf B\n", bGain);
									eGain = -2000*pow(3.0, u[6]);
									e += eGain;
									printf("%+.0lf E\n\n", eGain);
									u[6]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 8:
								if(b >= 32000*pow(3.0, u[7]) && d >= 1000*pow(2.0, u[7]) && f >= 4000*pow(2.0,u[7]) && rush >=100*pow(2.0,u[7]))
								{
									bGain = -32000*pow(3.0, u[7]);
									b += bGain;
									printf("%+.0lf B\n", bGain);
									dGain = -1000*pow(2.0, u[7]);
									d += dGain;
									printf("%+.0lf D\n", dGain);
									fGain = -4000*pow(2.0, u[7]);
									f += fGain;
									printf("%+.0lf F\n", fGain);
									rushGain = -100*pow(2.0, u[7]);
									rush += rushGain;
									boost = boostCalc(rush, u[9]);
									printf("%+.0lf RUSH\n\n", rushGain);
									u[7]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 9:
								if(e >= 9000*pow(4.0, u[8]) && f >= 6000*pow(2.0, u[8]))
								{
									eGain = -9000*pow(4.0, u[8]);
									e += eGain;
									printf("%+.0lf E\n", eGain);
									fGain = -6000*pow(2.0, u[8]);
									f += fGain;
									printf("%+.0lf F\n\n", fGain);
									u[8]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 10:
								if(a >= 100000*pow(3.0, u[9]) && c >= 6250*pow(2.0, u[9]) && e >= 4000*pow(5.0,u[9]) && g >= floor(250*pow(2.2, u[9])) && rush >= 250*pow(2.0,u[9]))
								{
									aGain = -100000*pow(3.0, u[9]);
									a += aGain;
									printf("%+.0lf A\n", aGain);
									cGain = -6250*pow(2.0, u[9]);
									c += cGain;
									printf("%+.0lf C\n", cGain);
									eGain = -4000*pow(5.0, u[9]);
									e += eGain;
									printf("%+.0lf E\n", eGain);
									gGain = -floor(250*pow(2.2, u[9]));
									g += gGain;
									printf("%+.0lf G\n", gGain);
									rushGain = -250*pow(2.0, u[9]);
									rush += rushGain;
									printf("%+.0lf RUSH\n\n", rushGain);
									u[9]++;
									boost = boostCalc(rush, u[9]);
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 11:
								if(e >= 18000*pow(4.0, u[10]) && f >= 10000*pow(2.0, u[10]) && g >= 500*pow(2.2,u[10]) && rush >= floor(250*pow(1.8,u[10])))
								{
									eGain = -18000*pow(4.0, u[10]);
									e += eGain;
									printf("%+.0lf E\n", eGain);
									fGain = -10000*pow(2.0, u[10]);
									f += fGain;
									printf("%+.0lf F\n", fGain);
									gGain = -500*pow(2.2,u[10]);
									g += gGain;
									printf("%+.0lf G\n", gGain);
									rushGain = -floor(250*pow(1.8,u[10]));
									rush += rushGain;
									boost = boostCalc(rush, u[9]);
									printf("%+.0lf RUSH\n\n", rushGain);
									u[10]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
							case 12:
								if(a >= 10000000*pow(2.0, u[11]) && b >= 25000000*pow(2.0, u[11]) && c>= floor(800000*pow(1.5, u[11])) && d >= floor(200000*pow(1.5, u[11])) && e >= 50000000*pow(2.0, u[11]) && f >= floor(1000000*pow(1.5, u[11])) && g >= floor(50000*pow(1.5, u[11])) && rush >= floor(10000*pow(1.5, u[11])))
								{
									aGain = -10000000*pow(2.0, u[11]);
									a += aGain;
									printf("%+.0lf A\n", aGain);
									bGain = -25000000*pow(2.0, u[11]);
									b += bGain;
									printf("%+.0lf B\n", bGain);
									cGain = -floor(800000*pow(1.5, u[11]));
									c += cGain;
									printf("%+.0lf C\n", cGain);
									dGain = -floor(200000*pow(1.5, u[11]));
									d += dGain;
									printf("%+.0lf D\n", dGain);
									eGain = -50000000*pow(2.0, u[11]);
									e += eGain;
									printf("%+.0lf E\n", eGain);
									fGain = -floor(1000000*pow(1.5, u[11]));
									f += fGain;
									printf("%+.0lf F\n", fGain);
									gGain = -floor(50000*pow(1.5, u[11]));
									g += gGain;
									printf("%+.0lf G\n", gGain);
									rushGain = -floor(10000*pow(1.5, u[11]));
									rush += rushGain;
									boost = boostCalc(rush, u[9]);
									printf("%+.0lf RUSH\n\n", rushGain);
									u[11]++;
								}
								else printf("Not enough resources to buy this upgrade.\n\n");
								break;
						}
					}
					while(upgradeChoice > -1 && upgradeChoice < numUpgrades+1);
					break;
			}
		}
		
		//This makes the game essentially autosave.
		save(a,b,c,d,e,f,g,csharp,rush,u,numUpgrades);
	}
}



//Calculates the boost from RUSH
double boostCalc(double r, int level)
{
	double RUSH = r;
	return (1 + (0.25 + level/8.0) * pow(RUSH, 0.80-0.20*(1-pow(1+RUSH, -0.50))+0.40*(1-pow(1+RUSH, -0.025))));
}


//Saves the game
void save(double a, double b, double c, double d, double e, double f, double g, double csharp, double rush, int u[], int numUpgrades)
{
	FILE *newSave;
	newSave = fopen("RushIncrementalSave.txt", "w");
	
	fprintf(newSave, "%.0lf\n", a);
	fprintf(newSave, "%.0lf\n", b);
	fprintf(newSave, "%.0lf\n", c);
	fprintf(newSave, "%.0lf\n", d);
	fprintf(newSave, "%.0lf\n", e);
	fprintf(newSave, "%.0lf\n", f);
	fprintf(newSave, "%.0lf\n", g);
	fprintf(newSave, "%.0lf\n", csharp);
	fprintf(newSave, "%.0lf\n", rush);
	for(int i=0; i<numUpgrades; i++)
	{
		fprintf(newSave, "%d\n", u[i]);
	}
	fclose(newSave);
}
