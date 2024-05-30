#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "print.h"
#include "initialization.h"

/*
This part of the code is for printing the representations of our chromossome.
So, the idea is to print the cromossome in the following way:

Imagine that there is 2 vehicles (lines) and 4 clients (columns). The first vehicle has 2 clients (A and D) and the second vehicle has 2 clients (B and C).
1001
0110

Whith that in mind, first we need to print a matrix with the clients and vehicles that are in the main.h.
So we need to create a matrix with the dimensions of the vehicles and clients.
Then, we need to print the matrix with the chromossome.

*/

/*
    -----------------------------------
              showPopulation()
    -----------------------------------

    This function:
    - Prints the population and its fitness;
    - Show the best individual (higher fitness);
    - Show the average fitness;
    - Save all the data in the file "dataVRP.xls".

*/
void showPopulation(int val)
{
    int h, i, j, k;

    printf("\n------------------------------------------------\n");

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", population[val].route[i][j]);
        }
        printf("\n");
    }

    printf("------------------------------------------------\n");

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 1; j < NUM_CLIENTS + 1; j++)
        {
            for (k = 1; k < NUM_CLIENTS + 1; k++)
            {
                if (population[val].route[i][j] == k)
                {
                    printf("1 ");
                }
                else
                {
                    printf("0 ");
                }
            }
            printf("\n");
        }
        printf("\n");
    }
}
