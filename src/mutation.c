#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "mutation.h"

/*
    -----------------------------------
              mutation()
    -----------------------------------

    This function:
    - First we have to determine if will be or not mutation;
    - If the mutation occurs:
        - After it will select randomly a point in the cromossome;
        - Then we will select another randomly point in the cromossome;
        - then we will change the position of the genes that were selected;

*/

void mutation(Individual *nextPop)
{
    int h, i, j, k;
    int mutate;
    int mutateIndividual;

    // Define if it will have or not mutation
    // mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

    mutateIndividual = 0; // Only for testing the mutation

    if (mutateIndividual == 0)
    {
        //printf("mutacao\n");
        //  Here we will select two points on the individual to mutate
        int point1, point2;
        for (i = 0; i < NUM_VEHICLES; i++)
        {

            // Here we will make sure that the points are different
            do
            {
                point1 = rand() % (NUM_CLIENTS + 1);
            } while (nextPop[0].route[i][point1] == 0);

            do
            {
                point2 = rand() % (NUM_CLIENTS + 1);
            } while (nextPop[0].route[i][point2] == 0 || point1 == point2);

            // Here we will mutate the individual (change the position of the genes that were selected)
            int temp = nextPop[0].route[i][point1];
            nextPop[0].route[i][point1] = nextPop[0].route[i][point2];
            nextPop[0].route[i][point2] = temp;
        }
    }
}
