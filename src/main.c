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
    nextPop = (Individual *)malloc(sizeof(Individual) * (POP_SIZE));
    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    tournamentIndividuals = (Individual *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verifying the memory allocation
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL)
    {
        printf("Fail locating memory!\n");
        return 0;
    }

    // Initializating the population
    initPop(population);

    for (rouds = 0; solutionFound == 0; rouds++)
    {
        solutionFound = evolvePop(rouds, populationFitness, population, nextPop, tournamentFitness, tournamentIndividuals, solutionFound);

        cont++;

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

    // printado a rota do melhor indivíduo
    /*
    printf("Printando o melhor indivíduo\n");
    for(int m=0; m<NUM_VEHICLES; m++){
        for(int n=0; n<NUM_CLIENTS+1; n++){
            printf("%d ", population[0].route[m][n]);
        }
        printf("\n");
    }
    */

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
        // fprintf(file, "O segundo fitness foi de: %d\n", secondfitness);
        fprintf(file, "A melhor fitness eh: %d\n", bestFitness);
        fprintf(file, "A media dos fitness com taxa de Elitismo %.2f eh: %d\n", ELITISMRATE, media_val);
        fprintf(file, "O desvio Padrao eh: %.5f\n", dp);
        fprintf(file, "Solution Found = %d\n", solutionFound);

        printf("\n");
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
    free(distance_clients);
    free(time_clients_end);
    // free(clients);

    return 0;
}
