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
    - Elitism();
    - rouletteSelection();
    - tournamentSelection();

*/

/*
    -----------------------------------
               Elitism()
    -----------------------------------

    - The first selection is the elitism;
    - It will be used to select the best individuals in the population;
    - After it is selected, we will add them to the next population;

*/

void elitism(int *index, Individual *nextPop, Individual *population)
{
    int h, i, j;
    int val = POP_SIZE * (ELITISMRATE * 100) / 100; // This variable will determine how many individuals will be selected to continue to the next population
    int bestIndex[val];

    for (h = 0; h < val; h++)
    {
        int bestIndividual = -1;
        for (i = 0; i < POP_SIZE; i++)
        {
            int chosen = 0;
            for (j = 0; j < h; j++)
            {
                if (bestIndex[j] == i)
                {
                    chosen = 1;
                    break;
                }
            }
            if (!chosen && (bestIndividual == -1 || population[i].fitness < population[bestIndividual].fitness))
            {
                bestIndividual = i;
            }
        }

        bestIndex[h] = bestIndividual;
    }

    // Then, we need to copy the best individual to te next population
    for (h = 0; h < val; h++)
    {
        int bestiIndividual = bestIndex[h];
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                nextPop[h].route[i][j] = population[bestiIndividual].route[i][j];
            }
        }
        nextPop[h].fitness = population[bestiIndividual].fitness;
    }

    *index = val; // Identify the position where future individuals will be inserted
}

/*
    -----------------------------------
            rouletteSelection()
    -----------------------------------

    - The second selection will be Roulette selection;
    - In this selection we will select two individuals from the population;
    - Individuals will be chosen according to their fitness, the higher it is, the more likely they are to be chosen.

*/

void rouletteSelection(Individual *parent, int *populationFitness, Individual *population)
{
    int i, j, k, l;
    int sumFitness = 0;
    int numSort, fitnessAcumulated;

    // First of all we will sum the fitness of all population
    for (i = 0; i < POP_SIZE; i++)
    {
        sumFitness += populationFitness[i];
    }

    /*
        -Then we need to calculate the probability of each individual to be chosen
        -Lowest fitness, higher the probability to be chosen as a parent
        -So, instead of using the actual value of fitness, we will use the negative fitness
    */
    double *inverseFitness = (double *)malloc(POP_SIZE * sizeof(double));
    double sumInverse = 0;
    for (int i = 0; i < POP_SIZE; i++)
    {
        inverseFitness[i] = 1.0 / populationFitness[i];
        sumInverse += inverseFitness[i];
    }

    double *probabilidade = (double *)malloc(POP_SIZE * sizeof(double));
    for (i = 0; i < POP_SIZE; i++)
    {
        probabilidade[i] = inverseFitness[i] / sumInverse;
    }

    // Here we will select two parents
    for (i = 0; i < 2; i++)
    {
        numSort = ((double)rand() / RAND_MAX) * sumInverse;
        double probAcumulated = 0;

        for (j = 0; j < POP_SIZE; j++)
        {
            probAcumulated += probabilidade[j];
            if (probAcumulated > numSort)
            {
                for (k = 0; k < NUM_VEHICLES; k++)
                {
                    for (l = 0; l < NUM_CLIENTS + 1; l++)
                    {
                        parent[i].route[k][l] = population[j].route[k][l];
                    }
                }
                break;
            }
        }
        parent[i].fitness = population[j].fitness;
    }

    free(inverseFitness);
    free(probabilidade);
}

/*
    -----------------------------------
            tournamentSelection()
    -----------------------------------

    - The third selection will be Tournament selection;
    - In this selection we will select two individuals from the population, they will be chosen randomly;
    - After they are chosen, the best (greater fitness) will be selected.

    - Problem: In this selection, the best individual can be selected more than once;

*/

void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population)
{
    int h, i, j, k;
    int individual[QUANTITYSELECTEDTOURNAMENT];

    for (i = 0; i < 2; i++)
    {
        int parentIndex = -1;

        // Selection of the individual that will be part of the tournament
        for (j = 0; j < QUANTITYSELECTEDTOURNAMENT; j++)
        {
            do
            {
                individual[j] = rand() % POP_SIZE;

                if (parentIndex == individual[j])
                {
                    individual[j] = rand() % POP_SIZE;
                }

                // Verify if it is different from the other individual
                for (k = 0; k < j; k++)
                {
                    if (individual[j] == individual[k])
                    {
                        break;
                    }
                }

                if (i == 0)
                {
                    parentIndex = individual[j];
                }

            } while (k < j);
        }
        printf("\n");

        // Finding the winner
        int winnerIndex = 0;
        int maxFitness = __INT_MAX__;

        for (j = 0; j < QUANTITYSELECTEDTOURNAMENT; j++)
        {
            if (population[individual[j]].fitness < maxFitness) // Verify if the individual is the best(lower fitness), if it is, it will be the winner
            {
                winnerIndex = individual[j];
                maxFitness = population[individual[j]].fitness;
            }
        }

        tournamentFitness[i] = maxFitness;
        tournamentIndividuals[i].fitness = maxFitness;
        for (k = 0; k < NUM_VEHICLES; k++)
        {
            for (h = 0; h < NUM_CLIENTS + 1; h++)
            {
                parent[i].route[k][h] = population[winnerIndex].route[k][h];
                parent[i].fitness = population[winnerIndex].fitness;
            }
        }
        parent[i].id = population[winnerIndex].id;
    }

    printf("\nTournament results:\n");
    for (int i = 0; i < 2; i++)
    {
        printf("Parent %d:\n", i + 1);
        printf("Fitness: %d\n", parent[i].fitness);
        printf("Fitness: %d\n", parent[i].id);
        printf("Route:\n");
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", parent[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

void tournamentSelectionEquals(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop, int index, int *previousWinner, int fitnessType)
{
    int h, j, k;

    int individual[SUBPOP_SIZE];

    int parentIndex = -1;

    // Selection of the individual that will be part of the tournament
    for (j = 0; j < SUBPOP_SIZE; j++)
    {
        do
        {
            individual[j] = rand() % SUBPOP_SIZE;

            if (parentIndex == individual[j])
            {
                individual[j] = rand() % SUBPOP_SIZE;
            }

            // Verify if it is different from the other individual
            for (k = 0; k < j; k++)
            {
                if (individual[j] == individual[k])
                {
                    break;
                }
            }

            if (j == 0)
            {
                parentIndex = individual[j];
            }

        } while (k < j);
    }

    int winnerIndex = 0;
    int minFitness = __INT_MAX__;

    for (j = 0; j < SUBPOP_SIZE; j++)
    {
        /*
        printf("individual[j]: %d ", individual[j]);
        printf("\n");
        for (int l = 0; l < NUM_VEHICLES; l++)
        {
            for (int m = 0; m < NUM_CLIENTS + 1; m++)
            {
                printf("%d ", subpop[individual[j]].route[l][m]);
            }
            printf("\n");
        }

        printf("\n");
        */

        int fitnessVal;

        switch (fitnessType)
        {
        case 0:
            // printf("A subpopulacao eh subpopDistance, entao o fitness eh fitnessDistance\n");
            fitnessVal = subpop[individual[j]].fitnessDistance;
            break;

        case 1:
            // printf("A subpopulacao eh subPopTime, entao o fitness eh fitnessTime\n");
            fitnessVal = subpop[individual[j]].fitnessTime;
            break;

        case 2:
            // printf("A subpopulacao eh subPopFuel, entao o fitness eh fitnessFuel\n");
            fitnessVal = subpop[individual[j]].fitnessFuel;
            break;

        default:
            break;
        }

        if (fitnessVal < minFitness && subpop[individual[j]].id != *previousWinner) // Verify if the individual is the best(lower fitness), if it is, it will be the winner
        {
            winnerIndex = individual[j];
            minFitness = fitnessVal;
        }
    }

    *previousWinner = subpop[winnerIndex].id;

    printf("\n Individuo vencedor do torneio: %d\n", winnerIndex);

    tournamentFitness[index] = minFitness;
    tournamentIndividuals[index].fitness = minFitness;
    for (k = 0; k < NUM_VEHICLES; k++)
    {
        for (h = 0; h < NUM_CLIENTS + 1; h++)
        {
            parent[index].route[k][h] = subpop[winnerIndex].route[k][h];
        }
    }

    switch (fitnessType)
    {
    case 0:
        // printf("fitnessDistance do pai\n");
        parent[index].fitnessDistance = minFitness;
        break;

    case 1:
        // printf("fitnessTime do pai\n");
        parent[index].fitnessTime = minFitness;
        break;

    case 2:
        // printf("fitnessFuel do pai\n");
        parent[index].fitnessFuel = minFitness;
        break;

    default:
        break;
    }

    printf("\n");

    printf("Tournament results:\n");
    printf("Parent %d:\n", index + 1);
    printf("Fitness: %d\n", minFitness);
    printf("Route:\n");
    for (int j = 0; j < NUM_VEHICLES; j++)
    {
        for (int k = 0; k < NUM_CLIENTS + 1; k++)
        {
            printf("%d ", parent[index].route[j][k]);
        }
        printf("\n");
    }
    printf("\n");
}

/*
    -----------------------------------
            subPopSelection()
    -----------------------------------

    - This one will select the subpopulation;
    - It will select randomly between all the subpopulation that we have;
    - After they are chosen, it will use the tournament selection to pick the best individual.

*/

void subPopSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop1, Individual *subpop2)
{
    int index = rand() % NUM_SUBPOP;
    int index2 = rand() % NUM_SUBPOP;

    // int index = 1;
    // int index2 = 1;

    int *previousWinner = -1; // keep track from the last chosen (when the same subpop is chosen)

    Individual *subpopulations[NUM_SUBPOP] = {subPopDistance, subPopTime, subPopFuel};

    subpop1 = subpopulations[index];
    subpop2 = subpopulations[index2];

    const char *subpopNames[NUM_SUBPOP] = {"subPopDistance", "subPopTime", "subPopFuel"};
    int fitnessType1 = index;
    int fitnessType2 = index2;

    for (int i = 0; i < 2; i++)
    {
        switch (i)
        {
        case 0:
            printf("Selected subpopulation 1: %s\n", subpopNames[index]);
            tournamentSelectionEquals(tournamentIndividuals, parent, tournamentFitness, subpop1, i, &previousWinner, fitnessType1);
            break;

        case 1:
            printf("Selected subpopulation 2: %s\n", subpopNames[index2]);
            tournamentSelectionEquals(tournamentIndividuals, parent, tournamentFitness, subpop2, i, &previousWinner, fitnessType2);
            break;

        default:
            break;
        }
    }
}
