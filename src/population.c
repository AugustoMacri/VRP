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

/*
    -----------------------------------
              evolvePop()
    -----------------------------------

    This function:
    - The population will be evolved until the limit of generations is reached or the optimal solution is found;
*/

int evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, int solutionFound)
{

    printf("\n************************************************************************\n");
    printf("\t\t\tEvoluindo a Populacao\n");

    int startIndex = 0;
    int *index = &startIndex;
    int i = 0;

    // Evaluate the initial population
    solutionFound = fitness(population, populationFitness, solutionFound);

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