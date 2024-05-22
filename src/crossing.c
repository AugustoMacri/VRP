#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "crossing.h"

/*
    -----------------------------------
              onePointCrossing()
    -----------------------------------

    This function:
    - Will select randomly a point in the cromossome;
    - After it will gona create a new individual with the first half of the first individual and the second half of the second individual;
    - And then, another individual will be created with the first half of the second individual and the second half of the first individual;
    - Both of these individuals will be added to the next population;
    (the 1 point crossing is creating only 1 individual)

    Script:
    - First select two cromossomes from the population;
    - The selection can be done with tournament or Roulette;
    - Then the crossing will be done with one point;
    - The new individual will be added to the nextPop;

*/

void onePointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    printf("---------------------------------------------\n");
    printf("REALIZANDO O CRUZAMENTO DE UM PONTO:\n");
    printf("---------------------------------------------\n");
    int i, j, cut;
    int son[NUM_VEHICLES][NUM_CLIENTS + 1];

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
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

    *index = (*index) + 1;

    printf("\n");
}

/*
    -----------------------------------
              twoPointCrossing()
    -----------------------------------

    This function:
    - Will select randomly two points in the cromossome;
    - After it will gona create a new individual with the first 1/3 of the first individual and the second 1/3 of the second individual, and again the third 1/3 of the first individual;
    - And then, another individual will be created with the rest of the first individual and the rest of the second individual;
    - Both of these individuals will be added to the next population;

    Script:
    - First select two cromossomes from the population;
    - The selection can be done with tournament or Roulette;
    - Then the crossing will be done with one point;
    - Then both new individual will be added to the nextPop;

*/

void twoPointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    printf("---------------------------------------------\n");
    printf("REALIZANDO O CRUZAMENTO DE DOIS PONTOS:\n");
    printf("---------------------------------------------\n");
    int i, j, cut1, cut2;
    int son1[NUM_VEHICLES][NUM_CLIENTS + 1]; 
    int son2[NUM_VEHICLES][NUM_CLIENTS + 1];

    // First of all we will initialize both of the sons
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = 0;
            son2[i][j] = 0;
        }
    }

    //Select randomly two points of cut in the cromossome
    do
    {
        cut1 = rand() % (NUM_CLIENTS + 1);
        cut2 = rand() % (NUM_CLIENTS + 1);
    } while (cut1 == NUM_CLIENTS + 1 || cut2 == NUM_CLIENTS + 1 || cut1 == cut2 || cut1 == 0 || cut2 == 0 || cut1 > cut2);

    printf("Corte: %d\n", cut1);
    printf("Corte: %d\n", cut2);

    // Copy the first third of the parents to the sons
    //printf("primeiro 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < cut1; j++)
        {
            son1[i][j] = parent[0].route[i][j];
            son2[i][j] = parent[1].route[i][j];
        }
    }

    // Copy the second third of the parents to the sons
    //printf("segundo 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut1; j < cut2; j++)
        {
            son1[i][j] = parent[1].route[i][j];
            son2[i][j] = parent[0].route[i][j];
        }
    }

    // Copy the third third of the parents to the sons
    //printf("terceiro 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut2; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = parent[0].route[i][j];
            son2[i][j] = parent[1].route[i][j];
        }
    }



    // Adding the both sons to the nextPop
    printf("FILHO 1: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son1[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;


    printf("FILHO 2: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son2[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;



    printf("\n");
}
