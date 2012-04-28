/*
 * Author:	Petr Vacha
 * Projekt:	Diploma thesis
 * Program:	CGP 
 * Email:	xvacha03@stud.fit.vutbr.cz
 * Datum:	2012
 */

#include <stdio.h>
#include <math.h>

#define POPULATION_SIZE 5
#define L_BACK
#define PARAM_M 40
#define PARAM_N 1

#define BLOCK_IN 2
#define DATAFILE "parita4.txt"

typedef int *chromozome;             
chromozome *population[POPULATION_SIZE]; 
  

int param_in = 0;
int param_out = 0;

int chromozomeLenght = 0;
int maxfitness  = 0; //max. hodnota fitness



int **datainput = NULL;
int **dataoutput = NULL;

/*
 * Alokuje pole pro populace.
 */
void allocPopulation()
{	
	chromozomeLenght = PARAM_M*PARAM_N*(BLOCK_IN+1) + param_out;

	for (int i = 0 ; i < POPULATION_SIZE; i++) {
		population[i] = new chromozome [chromozomeLenght];
	}	
}

/*
 * Uvolni pole pro populace.
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
 * Nacteni vstupnich dat
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
		
		for(int i =0;i<lenght;i++){ // test
			for(int x=0;x<param_in;x++)
				printf("%d",datainput[x][i]);
			for(int y=0;y<param_out;y++)
				printf("%d",dataoutput[y][i]);
			printf("\n");
		}	
		
		fclose (data);

	} else {
		fprintf(stderr,"ERROR: Nelze otevrit soubor se vstupnimi daty.\n");
	}
	
}

/*
 * Ohodnoceni populace
 */
inline int fitness()
{

 return 0;
}

/*
 * Mutace na populaci
 */
inline void mutace()
{

}


/*
 * Hlavni funkce
 */
int main(int argc, char* argv[])
{
	
	readData();
	allocPopulation();
	

	freePopulation();
	freeData();
	return 0;
}
