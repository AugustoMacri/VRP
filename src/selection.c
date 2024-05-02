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

void rouletteSelection(Individual *parent, int *populationFitness, Individual *population)
{
    printf("\nTESTANDO O CRUZAMENTO POR ROLETA\n");

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
    }
    int i, j, k, l;
    int sumFitness = 0;
    int numSort, fitnessAcumulated, cont;

    // First of all we will sum the fitness of all population
    for (i = 0; i < POP_SIZE; i++)
    {
        sumFitness += populationFitness[i];
        // printf("%d ", populationFitness[i]);
        // printf("%d ", sumFitness); está fazendo a soma de dois fitness corretamente como deveria
    }

    // Calcular as probabilidades de seleção para cada indivíduo
    double *probabilidade = (double *)malloc(POP_SIZE * sizeof(double));
    for (i = 0; i < POP_SIZE; i++)
    {
        probabilidade[i] = (double)populationFitness[i] / sumFitness;
        printf("A probabilidade de escolher o individuo %d: %.2f \n", i + 1, probabilidade[i]);
    }

    // Seleção dois pais para o cruzamento
    for (i = 0; i < 2; i++)
    {
        // Girar a roleta
        numSort = rand() % sumFitness; // Gera um intervalo de 0 a sumFitness
        fitnessAcumulated = 0;

        // Seleção de um dos pais
        for (j = 0; j < POP_SIZE; j++)
        {
            fitnessAcumulated += populationFitness[j];
            if (fitnessAcumulated > numSort)
            {
                // Armazenar o cromossomo do pai selecionado
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
        printf("\n");
    }
    printf("\n");
}

/*
    -----------------------------------
            tournamentSelection()
    -----------------------------------

    - The second selection will be Tournament selection;
    - In this selection we will select two individuals from the population;
    - Individuals will be chosen randomly in a population;
    - After they are chosen, the best (greater fitness) will be selected.

*/

void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population)
{
    int h, i, j, k;
    int cont = 0;
    int individual[QUANTITYSELECTEDTOURNAMENT];

    printf("REALIZANDO SELECAO POR TORNEIO\n");

    printf("\n");

    // Loop para cada pai a ser selecionado
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < QUANTITYSELECTEDTOURNAMENT; j++)
        {
            int k; // Declaração de k aqui
            do
            {
                individual[j] = rand() % POP_SIZE;
                k = 0; // Inicializa k
                while (k < j && individual[j] != individual[k])
                { // Verifica se é uma repetição
                    k++;
                }
            } while (k < j); // Repete enquanto houver repetições
            // printf("Individuo selecionado: %d\n", individual[j]);
        }

        // Realizar o torneio para selecionar um indivíduo
        int winnerIndex = 0;
        int maxFitness = 0;
        for (j = 0; j < QUANTITYSELECTEDTOURNAMENT; j++)
        {
            printf("realizando torneio para o individuo %d\n", j + 1);
            if (populationFitness[individual[j]] > maxFitness)
            {
                winnerIndex = individual[j];
                maxFitness = populationFitness[individual[j]];
            }
        }

        // Armazenar o indivíduo vencedor
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