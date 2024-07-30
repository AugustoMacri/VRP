#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "population.h"
#include "initialization.h"
#include "fitness.h"
#include "selection.h"
#include "crossing.h"
#include "mutation.h"
#include "print.h"

/*
    -----------------------------------
              updatePop()
    -----------------------------------

    This function:
    - In this function we will update the population;
    - so the population[i] will be updated to nextpop[i];
*/
void updatePop(Individual *population, Individual *nextPop)
{
    int h, i, j;

    for (h = 0; h < POP_SIZE; h++)
    {
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                population[h].route[i][j] = nextPop[h].route[i][j];
            }
        }
    }
}

void compareSonSubPop(Individual *nextPop, Individual *subPop, int index)
{

    int replaced = 0;
    int highestFitnessIndex = 0;
    int individualVerified = 0;

    // First we have to pick the individual of the subpopulation selected with the lowest fitness
    for (int i = 1; i < SUBPOP_SIZE; i++)
    {
        switch (index)
        {
        case 0:
            if (subPop[i].fitnessDistance > subPop[highestFitnessIndex].fitnessDistance)
            {
                highestFitnessIndex = i;
            }
            break;

        case 1:
            if (subPop[i].fitnessTime > subPop[highestFitnessIndex].fitnessTime)
            {
                highestFitnessIndex = i;
            }
            break;

        case 2:
            if (subPop[i].fitnessFuel > subPop[highestFitnessIndex].fitnessFuel)
            {
                highestFitnessIndex = i;
            }
            break;

        case 3:
            if (subPop[i].fitness > subPop[highestFitnessIndex].fitness)
            {
                highestFitnessIndex = i;
            }
            break;

        default:
            return;
        }
    }

    switch (index)
    {
    case 0:

        if (nextPop[0].fitnessDistance < subPop[highestFitnessIndex].fitnessDistance)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPDISTANCE\n");
            printf("Fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessDistance);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = nextPop[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = nextPop[0].id;
            subPop[highestFitnessIndex].fitnessDistance = nextPop[0].fitnessDistance;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessDistance);

            replaced = 1;
        }

        break;

    case 1:

        if (nextPop[0].fitnessTime < subPop[highestFitnessIndex].fitnessTime)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPTIME\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessTime);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = nextPop[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = nextPop[0].id;
            subPop[highestFitnessIndex].fitnessTime = nextPop[0].fitnessTime;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessTime);

            replaced = 1;
        }

        break;

    case 2:

        if (nextPop[0].fitnessFuel < subPop[highestFitnessIndex].fitnessFuel)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPFuel\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessFuel);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = nextPop[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = nextPop[0].id;
            subPop[highestFitnessIndex].fitnessFuel = nextPop[0].fitnessFuel;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessFuel);

            replaced = 1;
        }

        break;

    case 3:

        if (nextPop[0].fitness < subPop[highestFitnessIndex].fitness)
        {
            printf("\nSUBSTITUINDO O FILHO NA SubpopPonderacao\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitness);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = nextPop[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = nextPop[0].id;
            subPop[highestFitnessIndex].fitness = nextPop[0].fitness;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitness);

            replaced = 1;
        }

        break;

    default:
        break;
    }
}

/*
    -----------------------------------
              evolvePop()
    -----------------------------------

    This function:
    - The population will be evolved until the limit of generations is reached or the optimal solution is found;
*/

void evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, Individual *subpop1, Individual *subpop2, int startIndex)
{

    // int startIndex = 1;
    int *index = &startIndex;

    // Selection between the subpopulations and tournament
    subPopSelection(tournamentIndividuals, parent, tournamentFitness, subpop1, subpop2);

    // Crossing between the parents
    switch (CROSSINGTYPE)
    {
    case 1:
        onePointCrossing(index, parent, nextPop);
        break;

    case 2:
        twoPointCrossing(index, parent, nextPop);
        break;
    }
}
