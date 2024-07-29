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
Individual *subPopDistance;
Individual *subPopTime;
Individual *subPopFuel;
Individual *subPopCapacity;
Individual *subPopWeighting;
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
    int i, rouds;
    FILE *file;

    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1));
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);

    population = (Individual *)malloc(sizeof(Individual) * POP_SIZE);
    parent = (Individual *)malloc(sizeof(Individual) * 2);
    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    tournamentIndividuals = (Individual *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));
    nextPop = (Individual *)malloc(sizeof(Individual) * (1));
    subPopDistance = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopTime = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopFuel = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopCapacity = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopWeighting = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subpop1 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subpop2 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verifying the memory allocation
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL || subPopDistance == NULL || subPopTime == NULL || subPopFuel == NULL || subPopCapacity == NULL || subPopWeighting == NULL)
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

    // subPopSelection(tournamentIndividuals, parent, tournamentFitness, subpop1, subpop2);

    // onePointCrossing(1, parent, nextPop);

    for (int i = 1; i < ROUNDS + 1; i++)
    {
        evolvePop(rouds, populationFitness, population, nextPop, tournamentFitness, tournamentIndividuals, subpop1, subpop2, i);

        if (i == 3)
        {
            break;
        }
    }

    // Printaremos todos os individuos de todas as subpopulações ao final, assim conseguiremos ver quais mudaram e quais nao

    file = fopen("output/dataVRP.xls", "a+");
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

    //---------------------------------------------------------------------------------------------------------------------

    /*
    for (rouds = 0; solutionFound == 0; rouds++)
    {
        solutionFound = evolvePop(rouds, populationFitness, population, nextPop, tournamentFitness, tournamentIndividuals, solutionFound);

        cont++;

        if (cont == 1)
        {
            firstfitness = populationFitness[0];
            // printf("Primeiro indivíduo %d \n", firstfitness);
        }

        if ((cont % 100) == 0)
        {
            printf("%d \n", populationFitness[0]);
        }

        if (rouds == ROUNDS)
        {
            break;
        }
    }

    int val = 0;
    int bestFitness = __INT_MAX__;

    for (i = 0; i < POP_SIZE; i++)
    {
        val += populationFitness[i];
        if (populationFitness[i] < bestFitness)
        {
            bestFitness = populationFitness[i];
        }
    }
    int media_val = val / POP_SIZE;

    // Desvio padrão
    double v = 0;
    double variancia = 0;
    double dp = 0;
    for (int k = 0; k < POP_SIZE; k++)
    {
        double desvio = 0;
        desvio = populationFitness[k] - media_val;
        v += pow(desvio, 2);
    }
    variancia = v / POP_SIZE;
    dp = sqrt(variancia);

    // time spent executing in seconds
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    file = fopen("output/dataVRP.xls", "a+");
    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    else
    {
        fprintf(file, "\n");
        fprintf(file, "Population Size: %d\n", POP_SIZE);
        fprintf(file, "Number of Vehicles: %d\n", NUM_VEHICLES);
        fprintf(file, "Number of Clients: %d\n", NUM_CLIENTS);
        fprintf(file, "Vehicle Capacity: %d\n", VEHICLES_CAPACITY);
        if (SELECTION == 1)
            fprintf(file, "Selection Type: Roulette\n");
        fprintf(file, "Crossover Points: %d points\n", CROSSINGTYPE);
        fprintf(file, "Mutation Rate: %f\n", MUTATIONRATE);
        fprintf(file, "Elitism Rate: %f\n", ELITISMRATE);
        fprintf(file, "Rounds: %d\n", rouds);
        fprintf(file, "Time: %f\n", time_spent);
        fprintf(file, "O primeiro fitness foi de: %d\n", firstfitness);
        fprintf(file, "A melhor fitness eh: %d\n", bestFitness);
        fprintf(file, "A media dos fitness com taxa de Elitismo %.2f eh: %d\n", ELITISMRATE, media_val);
        fprintf(file, "O desvio Padrao eh: %.5f\n", dp);
        fprintf(file, "Solution Found = %d\n", solutionFound);

        printf("\n");
    }
    */

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
    free(subPopDistance);
    free(subPopTime);
    free(subPopFuel);
    free(subPopCapacity);
    free(subPopWeighting);
    free(distance_clients);
    free(time_clients_end);

    return 0;
}
