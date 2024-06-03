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
    printf("\n------------------------------------------------\n");
    printf("\tTESTANDO A ELITISMO\n");
    printf("------------------------------------------------\n");

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

    // Testando
    for (h = 0; h < val; h++)
    {
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                printf("%d ", nextPop[h].route[i][j]);
            }
            printf("\n");
        }

        printf("Com Fitness: %d\n", nextPop[h].fitness);
        printf("\n");
    }
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
    printf("\n------------------------------------------------\n");
    printf("TESTANDO O SELECAO POR ROLETA\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", population[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    int i, j, k, l;
    int sumFitness = 0;
    int numSort, fitnessAcumulated;

    // First of all we will sum the fitness of all population
    for (i = 0; i < POP_SIZE; i++)
    {
        sumFitness += populationFitness[i];
        // printf("%d ", populationFitness[i]);
        // printf("%d ", sumFitness); está fazendo a soma de dois fitness corretamente como deveria
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
        // printf("%.2f ", sumInverse);
    }

    double *probabilidade = (double *)malloc(POP_SIZE * sizeof(double));
    // double *prob2 = (double*)malloc(POP_SIZE * sizeof(double));
    for (i = 0; i < POP_SIZE; i++)
    {
        probabilidade[i] = inverseFitness[i] / sumInverse;
        // prob2[i] = (double)populationFitness[i] / sumFitness;
        printf("A probabilidade de escolher o individuo %d: %.2f \n", i + 1, probabilidade[i]);
        // printf("A probabilidade de escolher o individuo %d: %.2f \n", i + 1, prob2[i]);
    }

    //free(inverseFitness);

    printf("SUMFITNESS");
    printf("%.2d ", sumFitness);
    printf("SUMINVERSE");
    printf("%.2f ", sumInverse);

    // Here we will select two parents
    for (i = 0; i < 2; i++)
    {
        // Spin the wheel
        numSort = rand() % sumFitness; // Gera um intervalo de 0 a sumFitness
        fitnessAcumulated = 0;

        // Select the first parent
        for (j = 0; j < POP_SIZE; j++)
        {
            fitnessAcumulated += populationFitness[j];
            if (fitnessAcumulated > numSort)
            {
                // Save the cromossome of the first parent
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

    for (i = 0; i < 2; i++)
    {
        printf("pai numero %d:\n", i + 1);
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (l = 0; l < NUM_CLIENTS + 1; l++)
            {
                printf("%d ", parent[i].route[j][l]);
            }
            printf("\n");
        }
        printf("O fitness do pai %d: %d \n", i + 1, parent[i].fitness);
        printf("\n");
    }
    printf("\n");
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

    printf("------------------------------------------------\n");
    printf("\tREALIZANDO SELECAO POR TORNEIO\n");
    printf("------------------------------------------------\n");

    printf("\n");

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

        int winnerIndex = 0;
        int maxFitness = __INT_MAX__;

        for (j = 0; j < QUANTITYSELECTEDTOURNAMENT; j++)
        {
            // printf("realizando torneio para o individuo %d\n", j + 1);
            if (population[individual[j]].fitness < maxFitness && parentIndex) // Verify if the individual is the best(lower fitness), if it is, it will be the winner
            {
                winnerIndex = individual[j];
                maxFitness = population[individual[j]].fitness;
            }
        }
        printf("O individuo que saiu vitorioso no torneio %d foi o individuo %d\n", i, winnerIndex);
        printf("\n");

        // the winners will be parents for the crossing
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
    }

    printf("\nTournament results:\n");
    for (int i = 0; i < 2; i++)
    {
        printf("Parent %d:\n", i + 1);
        printf("Fitness: %d\n", parent[i].fitness);
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