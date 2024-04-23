#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "selection.h"

/*
    -----------------------------------
               Selection
    -----------------------------------

    - In this part of the code we will create the selection of the population;
    - We will use Roulette selection (just like in Stanley's implementation) and Tournament selection;
    - So, we're gona have one function for each of these selection;
    - rouletteSelection();
    - tournamentSelection();

*/

/*
    -----------------------------------
            rouletteSelection()
    -----------------------------------

    - The first selection will be Roulette selection;
    - In this selection we will select two individuals from the population;
    - Individuals will be chosen according to their fitness, the higher it is, the more likely they are to be chosen.

*/

void rouletteSelection(int **parent, int *populationFitness, int **populacaoAtual)
{
    int i, j, k;
    int sumFitness = 0;
    int numSort, fitnessAcumulated, cont;

    // First of all we will sum the fitness of all population
    for (i = 0; i < POP_SIZE; i++)
    {
        sumFitness += populationFitness[i];
    }

    for (i = 0; i < 2; i++)
    {
        cont = 0;

        // Spining the Wheel
        numSort = rand() % sumFitness;

        // Selection one of the parents
        for (j = 0; (j < POP_SIZE && fitnessAcumulated <= numSort); j++)
        {
            fitnessAcumulated += populationFitness[j];
            if (fitnessAcumulated > numSort)
            {
                for (k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    parent[i][k] = populacaoAtual[j][k];
                }
            }
        }

        // Verify if they are both the same
        if (i > 0)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                if (parent[0][j] == parent[1][j])
                {
                    cont++;
                }
            }
        }

        if (cont == NUM_CLIENTS + 1)
        {
            i--;
        }
    }
}