#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "proj4.h"

//This struct will be used to give arguments to the thread 
typedef struct 
{
	//pointer to the input and ouput grid 
	grid *input;
    	grid *output;
    	
    	//sum to find diagonally
    	unsigned long s;
    	
    	//starting row for thread
    	unsigned int startRow;
    	
    	//ending row for thread 
    	unsigned int endRow;
    	
} 	ThreadData;

//Function to initialize the grid from a file
void initializeGrid(grid *g, char *fileName) 
{
	//open the file 
    	FILE *file = fopen(fileName, "r");
    	if (file == NULL) 
    	{
        	perror("Error opening file");
        	exit(1);
    	}

	//will be used to check what is the size of grid 
    	unsigned int n = 0;
    	
    	char ch;
    	
    	while ((ch = fgetc(file)) != '\n' && ch != EOF) 
    	{
        	n++;
    	}
    	
    	//will be used to go back to the start of the file 
    	rewind(file);

	//will be allocating memory to the grid 
    	g->n = n;
    	
    	g->p = malloc(n * sizeof(unsigned char *));
    	
    	if (g->p == NULL) 
    	{
        	perror("Memory allocation failed");
        	exit(1);
    	}

	//allocate memory to the row 
    	for (unsigned int i = 0; i < n; i++) 
    	{
        	g->p[i] = malloc(n * sizeof(unsigned char));
        	if (g->p[i] == NULL) 
        	{
            		perror("Memory allocation failed");
            		exit(1);
        	}
    	}

	//read from file 
    	for (unsigned int i = 0; i < n; i++) 
    	{
        	for (unsigned int j = 0; j < n; j++) 
        	{
            		if (fscanf(file, "%1hhu", &g->p[i][j]) != 1) 
            		{
                		perror("Error reading grid data");
                		exit(1);
            		}
        	}
    	}

	//close the file 
    	fclose(file);
}

//Function will compute diagonal sums for a segment of the grid. The function will take the pointers to the grids as input, the targetted sum number, and other information related to the grid. Will not return anything
void computeDiagonals(grid *input, grid *output, unsigned long s, unsigned int startRow, unsigned int endRow) 
{
	//the size of grid 
    	unsigned int n = input->n;
    	
    	//stores the sum of diagonals 
    	unsigned long sum;

	//will go through the starting numbers 
    	for (unsigned int i = startRow; i < endRow; i++) 
    	{
    		//will go right 
        	for (unsigned int j = 0; j < n; j++) 
        	{
            		sum = 0;
            		for (unsigned int k = 0; i + k < n && j + k < n; k++) 
            		{
                		sum += input->p[i + k][j + k];
                		
                		//will check if the diagonal sum equals the targetted sum 
                		if (sum == s) 
                		{
                    			for (unsigned int m = 0; m <= k; m++) 
                    			{
                        			output->p[i + m][j + m] = input->p[i + m][j + m];
                    			}
                		}
            		}

			
            		sum = 0;
            		
            		//will go in the opposite direction 
            		for (unsigned int k = 0; i + k < n && j >= k; k++) 
            		{
                		sum += input->p[i + k][j - k];
                		if (sum == s) 
                		{
                    			for (unsigned int m = 0; m <= k; m++) 
                    			{
                        			output->p[i + m][j - m] = input->p[i + m][j - m];
                    			}
                		}
            		}
        	}
    	}
}

//This function will be the thread function to help compute the diagonals. Will take a pointer to void. Will not return anything 
void *threadWorker(void *arg) 
{
	ThreadData *data = (ThreadData *)arg;
	
	//will compute the diagonal sums 
    	computeDiagonals(data->input, data->output, data->s, data->startRow, data->endRow);
    	
    	return NULL;
}

//Function will compute all diagonal sums
void diagonalSums(grid *input, unsigned long s, grid *output, int t) 
{
	//stores the size of the grid 
	unsigned int n = input->n;

	//will set up the output grid 
    	output->n = n;
    	
    	output->p = malloc(n * sizeof(unsigned char *));
    	
    	if (output->p == NULL) 
    	{
        	perror("Memory allocation failed");
        	
        	exit(1);
    	}

    	for (unsigned int i = 0; i < n; i++) 
    	{
        	output->p[i] = calloc(n, sizeof(unsigned char));
        	if (output->p[i] == NULL) 
        	{
            		perror("Memory allocation failed");
            		exit(1);
        	}
    	}

	//will run this if the thread is only 1 
    	if (t == 1) 
    	{
        	computeDiagonals(input, output, s, 0, n);
        	return;
    	}

	//this part below is for multithreading 
    	pthread_t threads[t];
    	ThreadData threadData[t];
    	unsigned int rowsPerThread = n / t;
    	unsigned int extraRows = n % t;

    	unsigned int startRow = 0;
    	for (int i = 0; i < t; i++) 
    	{
    	
    		//checks how many rows the thread will need to work on 
        	unsigned int endRow = startRow + rowsPerThread + (i < extraRows ? 1 : 0);

		//thread data 
        	threadData[i].input = input;
        	
        	threadData[i].output = output;
        	
        	threadData[i].s = s;
        	
        	threadData[i].startRow = startRow;
        	
        	threadData[i].endRow = endRow;
	
		//will create the thread 
        	pthread_create(&threads[i], NULL, threadWorker, &threadData[i]);
        	
        	startRow = endRow;
    	}	

	//will make the main thread wait for the others to finish 
    	for (int i = 0; i < t; i++) 
    	{
        	pthread_join(threads[i], NULL);
    	}
}

//Function to write the grid to a file
void writeGrid(grid *g, char *fileName) 
{
	//open an output file 
	FILE *file = fopen(fileName, "w");
	
    	if (file == NULL) 
    	{
        	perror("Error opening file for writing");
        	
        	exit(1);
    	}

	//will write to the output grid 
    	for (unsigned int i = 0; i < g->n; i++) 
    	{
        	for (unsigned int j = 0; j < g->n; j++) 
        	{
            		fprintf(file, "%d", g->p[i][j]);
        	}
        	
        	fprintf(file, "\n");
    	}

	//closes the file 
    	fclose(file);
}

//Function to free allocated memory for a grid
void freeGrid(grid *g) 
{
	//free memory for the rows 
    	if (g->p) 
    	{
        	for (unsigned int i = 0; i < g->n; i++) 
        	{
            		free(g->p[i]);
        	}
        	
        	//free the pointers for rows 
        	free(g->p);
        	
        	//make the pointer null
        	g->p = NULL;
    	}
}

