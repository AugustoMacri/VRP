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

    for (int i = 0; i < SUBPOP_SIZE && replaced == 0; i++)
    {
        switch (index)
        {
        case 0:

            if (nextPop[0].fitnessDistance < subPop[i].fitnessDistance)
            {
                printf("\nfitness do individuo subst: %d\n", subPop[i].fitnessDistance);
                printf("SUBSTITUINDO O FILHO NA SUBPOPDISTANCE\n");
                for (int j = 0; j < NUM_VEHICLES; j++)
                {
                    for (int k = 0; k < NUM_CLIENTS + 1; k++)
                    {
                        subPop[i].route[j][k] = nextPop[0].route[j][k];
                    }
                }

                subPop[i].id = nextPop[0].id;
                subPop[i].fitnessDistance = nextPop[0].fitnessDistance;

                replaced = 1;
            }

            break;

        case 1:

            if (nextPop[0].fitnessTime < subPop[i].fitnessTime)
            {
                printf("\nfitness do individuo subst: %d\n", subPop[i].fitnessTime);
                printf("SUBSTITUINDO O FILHO NA SUBPOPTIME\n");
                for (int j = 0; j < NUM_VEHICLES; j++)
                {
                    for (int k = 0; k < NUM_CLIENTS + 1; k++)
                    {
                        subPop[i].route[j][k] = nextPop[0].route[j][k];
                    }
                }

                subPop[i].id = nextPop[0].id;
                subPop[i].fitnessTime = nextPop[0].fitnessTime;

                replaced = 1;
            }

            break;

        case 2:

            if (nextPop[0].fitnessFuel < subPop[i].fitnessFuel)
            {
                printf("\nfitness do individuo subst: %d\n", subPop[i].fitnessFuel);
                printf("SUBSTITUINDO O FILHO NA SUBPOPFuel\n");
                for (int j = 0; j < NUM_VEHICLES; j++)
                {
                    for (int k = 0; k < NUM_CLIENTS + 1; k++)
                    {
                        subPop[i].route[j][k] = nextPop[0].route[j][k];
                    }
                }

                subPop[i].id = nextPop[0].id;
                subPop[i].fitnessFuel = nextPop[0].fitnessFuel;

                replaced = 1;
            }

            break;

        default:
            break;
        }
    }
}

/*
    -----------------------------------
              evolvePop()
    -----------------------------------

    This function:
    - The population will be evolved until the limit of generations is reached or the optimal solution is found;
*/
/*
int evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, int solutionFound)
{

    int startIndex = 0;
    int *index = &startIndex;
    int i = 0;

    // Evaluate the initial population
    solutionFound = fitness(population, populationFitness, solutionFound);

    elitism(index, nextPop, population);
    do
    {
        // Selection
        switch (SELECTION)
        {
        case 1:
            rouletteSelection(parent, populationFitness, population);
            break;

        case 2:
            tournamentSelection(tournamentIndividuals, parent, tournamentFitness, populationFitness, population);
            break;
        }
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

        // Mutation
        mutation(index, nextPop);

    } while (startIndex < POP_SIZE);

    // Update the population with next generation
    updatePop(population, nextPop);

    // Evaluate the new population
    solutionFound = fitness(population, populationFitness, solutionFound);

    return solutionFound;
}

*/