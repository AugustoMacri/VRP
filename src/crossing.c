#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "crossing.h"

/*
    -----------------------------------
              ondePointCrossing()
    -----------------------------------

    This function:
    - Will select randomly a point in the cromossome;
    - After it will gona create a new individual with the first half of the first individual and the second half of the second individual;
    - And then, another individual will be created with the first half of the second individual and the second half of the first individual;
    - Both of these individuals will be added to the next population;

    Script:
    - First select two cromossomes from the population;
    - The selection can be done with tournament or Roulette;
    - Then the crossing will be done with one point;
    - The new individual will be added to the nextPop;

*/

void onePointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    printf("REALIZANDO O CRUZAMENTO DE UM PONTO:\n");
    int i, j, cut;
    int son[NUM_VEHICLES][NUM_CLIENTS + 1];

    for(i = 0; i < NUM_VEHICLES; i++){
        for(j = 0; j < NUM_CLIENTS + 1; j++){
            son[i][j] = 0;
        }
    }

    do
    {
        cut = rand() % NUM_CLIENTS + 2;
    } while (cut == NUM_CLIENTS + 1);

    printf("Corte: %d\n", cut);

    // Copy the first half of the first parent to the son
    printf("primeira metade do filho\n");
    for (i = 0; i < cut; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            son[j][i] = parent[0].route[j][i];
            printf("%d ", son[j][i]);
        }
        printf("\n");
    }
    printf("\n");

    // Copy the second half of the second parent to the son
    printf("segunda metade do filho\n");
    for (i = cut; i < NUM_CLIENTS + 1; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            son[j][i] = parent[1].route[j][i];
            printf("%d ", son[j][i]);
        }
        printf("\n");
    }
    printf("\n");

    // Adding the son to the nextPop
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index  = (*index) + 1;

    printf("\n");
}
