/*
 * Author:	Petr Vacha
 * Projekt:	Diploma thesis
 * Program:	CGP 
 * Email:	xvacha03@stud.fit.vutbr.cz
 * Datum:	2012
 */
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define POPULATION_SIZE 5
#define RUNS 100

#define GENERATIONS 10000000

#define PARAM_M 3
#define PARAM_N 1

#define MUTATIONS 1
#define FUNCTIONS 6				// pocet funkci
#define L_BACK PARAM_M			// l-back
#define BLOCK_IN 2 				// pocet vstupu
#define BLOCK_SIZE (BLOCK_IN+1) 	// delka jednoho bloku v chromozomu
#define BLOCKS_SIZE (BLOCK_SIZE*PARAM_M*PARAM_N) 	//delka vsech bloku v chromozomu

#define BLOCK_INDICES (PARAM_M*PARAM_N) 			//pocet bloku
#define DATAFILE "parita4.txt"

#define COPY_CHROMOZOME(from,to) (chromozome) memcpy(to, from, (chromozomeLength)*sizeof(int));

typedef int *chromozome;             
chromozome population[POPULATION_SIZE]; 

int *tmpPopulation; // pro přepočet fitness
  

int param_in = 0;
int param_out = 0;

int chromozomeLength = 0;
int maxfitness  = 0; //max. hodnota fitness

int arrayLength; // pocet radku v trenovacich datech

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
	
	tmpPopulation = new int [param_in + BLOCK_INDICES + param_out];
}

/*
 * Uvolni pole pro populaci.
 */
void freePopulation()
{	
	for (int i = 0 ; i < POPULATION_SIZE; i++) {
		delete [] population[i];
	}

	delete [] tmpPopulation;
}

/*
 * Alokuje datova pole pro trenovaci data.
 */
void allocData(int input_x, int output_x)
{
	datainput = new int *[input_x];
	
	for (int i = 0 ; i < input_x ; i++) {
		datainput[i] = new int[arrayLength];
	}
	
	dataoutput = new int *[output_x];
	
	for (int i = 0 ; i < output_x ; i++) {
		dataoutput[i] = new int[arrayLength];
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
		
		arrayLength = pow(2, param_in);
		allocData(param_in, param_out);
	
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
 * Generator cisel pro bloky.
 */
inline int generator(int gene)
{
	int min = -1;
	int max = -1;
	int blockIndex = -1;
	int randomNumber;
	int tmpCount;
	
	int result = -1;

	if (gene < BLOCKS_SIZE) {
		if (gene % BLOCK_SIZE < 2) {
		
			if (gene >= BLOCK_SIZE*PARAM_N) { //neni v prvnim sloupci
			 
				blockIndex = gene/BLOCK_SIZE + param_in + PARAM_N - 1;
				
				max = blockIndex - (PARAM_N - (blockIndex % PARAM_N));
				min = blockIndex - PARAM_N*L_BACK;
				//if(gene == 4) printf("%d\n", min, max);
				if(min<=param_in) {
					result = rand() % (max+1);
					
				} else {
					tmpCount = max - min + param_in+1;
					randomNumber = rand() % tmpCount;
					if (randomNumber >= param_in) {
						result = randomNumber - param_in + min;
						
					} else {
						result = randomNumber;
					}
				}
				
			} else {
				result = rand() % param_in;
			}
			
		} else {
			//result = 2;
			result = rand() % FUNCTIONS;
		}
	} else { // vystup
		result = rand() % (BLOCK_INDICES + param_in);
	}
	
	return result;
}

/*
 * Mutace kandidatniho reseni.
 */
inline void mutace(int populationIndex)
{
	int gene;
	
	for (int i=0; i < MUTATIONS; i++) {
		gene = rand() % chromozomeLength;
		population[populationIndex][gene] = generator(gene);
	}
}

/*
 * Vygenerovani nahodnou populaci.
 */
void generationRandomPopulation()
{
	for (int p=0; p < POPULATION_SIZE; p++) {
		for (int i=0; i < chromozomeLength; i++) {
			population[p][i] = generator(i);
		}
	}
}


/*
 * Ohodnoceni populace.
 */
inline int fitness(int populationIndex)
{
	int fitness = 0;
	int fail;
	int block;
	int x;
	int in1, in2;

	
	
	for (int rowIndex= 0; rowIndex <arrayLength; rowIndex++) {

		block = param_in;
		
		for (int i=0; i < param_in; i++) {
			tmpPopulation[i] = datainput[i][rowIndex];
			//printf("%d ",tmpPopulation[i]);
		}
		//printf("\n");
		
		for (int i=0; i < chromozomeLength; i++) {//printf("%d, ",BLOCK_INDICES);
			if (i < BLOCKS_SIZE) {
				x = i % BLOCK_SIZE;
				if (x == 0) {
					in1 = i;//printf("in1 %d, ",tmpPopulation[population[populationIndex][in1]]);
				}
		
				if (x == 1) {
					in2 = i;//printf("in2 %d, ",tmpPopulation[population[populationIndex][in2]]);
				}

				if (x == 2) { // funkce
					//printf("f %d, ",population[populationIndex][i]);
					switch(population[populationIndex][i]) {
						  case 0: tmpPopulation[block] = tmpPopulation[population[populationIndex][in1]]; 
						  //printf("in1 ");
						  break;       		//in1

						  case 1: tmpPopulation[block] = tmpPopulation[population[populationIndex][in1]] & tmpPopulation[population[populationIndex][in2]];
						 // printf("and ");
						  break; 		//and
						  case 2: tmpPopulation[block] = tmpPopulation[population[populationIndex][in1]] ^ tmpPopulation[population[populationIndex][in2]];
						//  printf("xor ");
						  break; 		//xor

						  case 3: tmpPopulation[block] = (tmpPopulation[population[populationIndex][in1]]==1)?0:1; 	
						 // printf("not in1 ");		  				      
						  break;  			//not in1
						  case 4: tmpPopulation[block] = (tmpPopulation[population[populationIndex][in2]]==1)?0:1; 	
						//  printf("not in2 ");	
						  break;  			//not in2

						  case 5: tmpPopulation[block] = tmpPopulation[population[populationIndex][in1]] | tmpPopulation[population[populationIndex][in2]]; 
						//  printf("or ");
						  break; 		//or
						  case 6: tmpPopulation[block] = tmpPopulation[population[populationIndex][in1]] & ((tmpPopulation[population[populationIndex][in2]]==1)?0:1);
						  break;  // in1 AND NOT in2
						  case 7: tmpPopulation[block] = ((tmpPopulation[population[populationIndex][in1]] & tmpPopulation[population[populationIndex][in2]])==1)?0:1;
						  break;	//nand
						  case 8: tmpPopulation[block] = ((tmpPopulation[population[populationIndex][in1]] | tmpPopulation[population[populationIndex][in2]])==1)?0:1;
						  break;	//nor
					}
					//printf("vysledek: %d, block %d  \t\t\t", tmpPopulation[block], block);
					//for (int c=0; c<BLOCK_INDICES+param_in+param_out; c++) {
					//	printf("%d", tmpPopulation[c]);
					//}
					//printf("\n");
					block++;
				}
			
			} else {
				tmpPopulation[block] = tmpPopulation[population[populationIndex][i]];
			}
		
		}
	/*	printf("0123|456|V\n");
		for (int b=0; b<BLOCK_INDICES+param_in+param_out; b++) {
			if (b == param_in || b == BLOCK_INDICES+param_in) {
				printf ("|");
			}
			printf("%d", tmpPopulation[b]);
		}
		printf("\n");
		*/		
	
		fail = 0;
		
		//printf("vystup: ");
		for (int d=0, i=(param_in + BLOCK_INDICES); d < param_out; i++, d++) {
		//	printf("<%d%d>", dataoutput[d][rowIndex],tmpPopulation[i]);
			if (dataoutput[d][rowIndex] != tmpPopulation[i]) {
				fail = 1;
			}
		}
	/*		
		printf(" dataoutput:");
		for (int d=0, i=(param_in + BLOCK_INDICES); d < param_out; i++, d++) {
			printf("%d", dataoutput[d][rowIndex]);
		}		
		printf("\n");
		*/
		if (fail == 0) {
			fitness++;
		}
	}
 	return fitness;
}


/*
 * Tisk chromozomu.
 */
void printChromozome(int populationIndex)
{
	
	for (int i=0; i<chromozomeLength; i++) {
		if(i !=0 && i % 3 == 0)
			printf(")(");
		if (i == 0)
			printf("(");
			
		printf("%d",population[populationIndex][i]);

		if (i == chromozomeLength-1)
			printf(")");
	}
		
	printf("\n");
	
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
	
	int bestCandidate;
	int bestFitness;
	
	int neutral;
	int fitnessValue;

	int run = 0;
	int generation = 0;


	bestCandidate = -1;
	bestFitness = -1;
	while (generation < GENERATIONS) {
		neutral = 0;
		for (int p = 0; p<POPULATION_SIZE; p++) {
			fitnessValue = fitness(p);
			if (fitnessValue > bestFitness) {
				bestFitness = fitnessValue;
				bestCandidate = p;
			}
			
			if (fitnessValue == bestFitness && p != bestCandidate && !neutral) {
				bestFitness = fitnessValue;
				bestCandidate = p;
				neutral = 1;
			}
		}
		
		for (int p = 0; p<POPULATION_SIZE; p++) {		
			if (p == bestCandidate) {
				continue;
			}
			COPY_CHROMOZOME(population[bestCandidate], population[p]);
			mutace(p);
			
		}

		generation++;
	}

		
	freePopulation();
	freeData();
	return 0;
}
