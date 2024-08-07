#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"
#include "fitness.h"
#include "selection.h"
#include "crossing.h"
#include "mutation.h"
#include "initialization.h"
#include "print.h"

int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int **populacaoAtual;
int *populationFitness;
int *tournamentFitness;

Individual *population, *parent;
Individual *tournamentIndividuals;
Individual *nextPop;
Individual *newSon;

Individual *subPopDistance;
Individual *subPopTime;
Individual *subPopFuel;
Individual *subPopCapacity;
Individual *subPopWeighting;

Individual *nextSubPopDistance;
Individual *nextSubPopTime;
Individual *nextSubPopFuel;
Individual *nextSubPopWeighting;

Individual *subpop1;
Individual *subpop2;

Storage *distance_clients, *time_clients_end;
Client clients[NUM_CLIENTS + 1];

int firstfitness = -1;
int secondfitness = -1;
int cont = 0;

int main()
{
    printf("Executing\n");
    srand(time(NULL));

    // Calculating the time spent executing
    double time_spent = 0.0;
    clock_t begin = clock();

    int solutionFound = 0;
    int i, rouds, idTrack;
    FILE *file;

    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1));
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);

    population = (Individual *)malloc(sizeof(Individual) * POP_SIZE);
    parent = (Individual *)malloc(sizeof(Individual) * 2);
    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    tournamentIndividuals = (Individual *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));
    nextPop = (Individual *)malloc(sizeof(Individual) * (1));
    newSon = (Individual *)malloc(sizeof(Individual) * (1));

    subPopDistance = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopTime = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopFuel = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopCapacity = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopWeighting = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    nextSubPopDistance = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopTime = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopFuel = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopWeighting = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    subpop1 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subpop2 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verifying the memory allocation
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL || subPopDistance == NULL || subPopTime == NULL ||
        subPopFuel == NULL || subPopCapacity == NULL || subPopWeighting == NULL || nextSubPopDistance == NULL || nextSubPopTime == NULL || nextSubPopFuel == NULL || nextSubPopWeighting == NULL)
    {
        printf("Fail locating memory!\n");
        return 0;
    }

    //---------------------------------------------------------------------------------------------------------------------
    // Initializating the population
    initPop(population);

    // Distributing the population in subpops
    distributeSubpopulation(population);

    // Calculando o fitness de cada subpopulação
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        fitnessDistance(subPopDistance, i);

        fitnessTime(subPopTime, i);

        fitnessFuel(subPopFuel, i);

        fitness(subPopWeighting, i);
    }

    printf("numero de individuos de ELITE %d\n", ELITISM_SIZE_POP);
    printf("numero de individuos da prox poplacao %d\n", SUBPOP_SIZE);

    file = fopen("output/dataVRP.xls", "a+");

    idTrack = 1;
    for (int i = 0; i < ROUNDS; i++)
    {
        evolvePop(rouds, populationFitness, population, newSon, tournamentFitness, tournamentIndividuals, subpop1, subpop2, i, &idTrack);
    }

    /*/ printando next pop
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", nextSubPopDistance[i].route[i][j]);
            }
        }
    }
    /*/

    // Printaremos todos os individuos de todas as subpopulações ao final, assim conseguiremos ver quais mudaram e quais nao
    // file = fopen("output/dataVRP.xls", "a+");
    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    else
    {
        fprintf(file, "|==================================Subpopulacaoo ao final DISTANCIA==================================|\n");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopDistance[i].id, subPopDistance[i].fitnessDistance);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopDistance[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Time==================================|\n ");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopTime[i].id, subPopTime[i].fitnessTime);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopDistance[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Fuel==================================|\n ");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopFuel[i].id, subPopFuel[i].fitnessFuel);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopFuel[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Weighting==================================|\n");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopWeighting[i].id, subPopWeighting[i].fitness);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopWeighting[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }
    }

    // Releasing memory
    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        free(populacaoAtual[i]);
    }

    free(populacaoAtual);
    free(populationFitness);
    free(population);
    free(parent);
    free(tournamentFitness);
    free(tournamentIndividuals);
    free(nextPop);
    free(newSon);

    free(subPopDistance);
    free(subPopTime);
    free(subPopFuel);
    free(subPopCapacity);
    free(subPopWeighting);

    free(nextSubPopDistance);
    free(nextSubPopTime);
    free(nextSubPopFuel);
    free(nextSubPopWeighting);

    free(distance_clients);
    free(time_clients_end);

    return 0;
}
