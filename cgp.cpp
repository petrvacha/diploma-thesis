/*
 * Author:	Petr Vacha
 * Projekt:	Diploma thesis
 * Program:	CGP 
 * Email:	xvacha03@stud.fit.vutbr.cz
 * Datum:	2012
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POPULATION_SIZE 5

#define PARAM_M 8
#define PARAM_N 1

#define FUNCTIONS 6				// pocet funkci
#define L_BACK PARAM_M			// l-back
#define BLOCK_IN 2 				// pocet vstupu
#define BLOCK_SIZE (BLOCK_IN+1) 	// delka jednoho bloku v chromozumo
#define BLOCKS_SIZE (BLOCK_SIZE*PARAM_M*PARAM_N) 	//delka vsech bloku v chromozomu

#define BLOCK_INDICES (PARAM_M*PARAM_N) 			//pocet bloku
#define DATAFILE "parita4.txt"

typedef int *chromozome;             
chromozome population[POPULATION_SIZE]; 
  

int param_in = 0;
int param_out = 0;

int chromozomeLength = 0;
int maxfitness  = 0; //max. hodnota fitness



int **datainput = NULL;
int **dataoutput = NULL;

/*
 * Alokuje pole pro populaci.
 */
void allocPopulation()
{	
	chromozomeLength = BLOCKS_SIZE + param_out;

	for (int i = 0 ; i < POPULATION_SIZE; i++) {
		population[i] = new int [chromozomeLength];
	}	
}

/*
 * Uvolni pole pro populaci.
 */
void freePopulation()
{	
	for (int i = 0 ; i < POPULATION_SIZE; i++) {
		delete [] population[i];
	}	
	
}

/*
 * Alokuje datova pole pro trenovaci data.
 */
void allocData(int input_x, int output_x, int arrayLenght)
{
	datainput = new int *[input_x];
	
	for (int i = 0 ; i < input_x ; i++) {
		datainput[i] = new int[arrayLenght];
	}
	
	dataoutput = new int *[output_x];
	
	for (int i = 0 ; i < output_x ; i++) {
		dataoutput[i] = new int[arrayLenght];
	}
	
}

/*
 * Uvolni alokovana datova pole s trenovacimi daty.
 */
void freeData()
{
	for( int i = 0 ; i < param_in ; i++ )
	delete [] datainput[i];
	delete [] datainput;
	
	for( int i = 0 ; i < param_out ; i++ )
	delete [] dataoutput[i];
	delete [] dataoutput;	
}

/*
 * Nacteni vstupnich dat.
 */
void readData()
{
	int c;
	int afterInput = 0;
	int lenght;
	
	FILE *data = fopen(DATAFILE, "r");
	if (data) {
		c = fgetc(data);
		
		while (c != EOF && c != '\n') {
			if (afterInput == 0 && (c == '0' || c == '1')) {
				param_in++;
			}
			
			if (c == ':') {
				afterInput = 1;
			}
			
			if (afterInput == 1 && (c == '0' || c == '1')) {
				param_out++;	
			}
			c = fgetc(data);
		}
		
		lenght = pow(2, param_in);
		allocData(param_in, param_out, lenght);
	
		fseek (data ,0 , SEEK_SET);
		
		int inputArray, inputIndex, outputArray, outputIndex;
		inputArray = inputIndex = outputArray = outputIndex = afterInput = 0;

		while ((c = fgetc(data)) != EOF) {
			if (afterInput == 0 && (c == '0' || c == '1')) {
				datainput[inputArray++][inputIndex] =  c - 48;
			}

			if (c == ':') {
				afterInput = 1;
				outputArray = 0;
			}

			if (afterInput == 1 && (c == '0' || c == '1')) {
				dataoutput[outputArray++][outputIndex] = c - 48;	
			}

			if (c == '\n') {
				inputIndex++;
				outputIndex++;
				afterInput = 0;
				inputArray = 0;
			}
		}	
		
	/*	for(int i =0;i<lenght;i++){ // test
			for(int x=0;x<param_in;x++)
				printf("%d",datainput[x][i]);
			for(int y=0;y<param_out;y++)
				printf("%d",dataoutput[y][i]);
			printf("\n");
		}	
	*/
		fclose (data);

	} else {
		fprintf(stderr,"ERROR: Nelze otevrit soubor se vstupnimi daty.\n");
	}
	
}

/*
 * Ohodnoceni populace.
 */
inline int fitness()
{

 return 0;
}

/*
 * Mutace jedince.
 */
inline void mutace()
{

}

/*
 * Vygenerovani nahodnou populaci.
 */
void generationRandomPopulation()
{
	int min = -1;
	int max = -1;
	int blockIndex = -1;
	int randomNumber;
	int tmpCount;

	for (int p=0; p<1; p++) {
		for (int i=0; i<chromozomeLength; i++) {
			if (i < BLOCKS_SIZE) {
				if (i % BLOCK_SIZE < 2) {
				
					if (i >= BLOCK_SIZE*PARAM_N) { //neni v prvnim sloupci
					 
						blockIndex = i/BLOCK_SIZE + param_in + PARAM_N - 1;
						
						max = blockIndex - (PARAM_N - (blockIndex % PARAM_N));
						min = blockIndex - PARAM_N*L_BACK;
						
						if(min<=param_in) {
							population[p][i] = rand() % max;
							
						} else {
							tmpCount = max + min + param_in;
							randomNumber = rand() % tmpCount;
							if (randomNumber >= param_in) {
								population[p][i] = randomNumber - param_in + min -1;
								
							} else {
								population[p][i] = randomNumber;
							}
						}
						
					} else {
						population[p][i] = rand() % param_in;
					}
					
				} else {
					population[p][i] = rand() % FUNCTIONS;
				}
			//	if (i % 3 != 2) //DEBUG
			//		printf("i=%d. block=%d max=%d min=%d \t\tdrat=%d\n", i, blockIndex, max, min, population[p][i]);
			}
		}
	}
}

/*
 * Hlavni funkce.
 */
int main(int argc, char* argv[])
{
	srand ( time(NULL) );
	readData();
	allocPopulation();
	generationRandomPopulation();


	for (int p=0; p<POPULATION_SIZE; p++) {
		for (int i=0; i<chromozomeLength; i++) {
			if(i % 3 == 0)
				printf(")(");
			printf("%d",population[p][i]);
		}
		printf("\n");break;
	}
	freePopulation();
	freeData();
	return 0;
}
