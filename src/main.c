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

// double distance_clients[NUM_VEHICLES][NUM_CLIENTS + 1];
// double time_clients_end[NUM_VEHICLES][NUM_CLIENTS + 1];
int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int **populacaoAtual;
int *populationFitness;
int *tournamentFitness;
Individual *population, *parent;
Individual *tournamentIndividuals;
Individual *nextPop;
Storage *distance_clients, *time_clients_end;
Client clients[NUM_CLIENTS + 1];

int main()
{
    printf("Programa em Execucao\n");

    // Calcular o tempo de execucao do programa
    double time_spent = 0.0;
    clock_t begin = clock();

    int solutionFound = 0;
    int i, j;
    FILE *file;

    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1)); // populacao atual
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    population = (Individual *)malloc(sizeof(Individual) * POP_SIZE);
    parent = (Individual *)malloc(sizeof(Individual) * 2);
    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    nextPop = (Individual *)malloc(sizeof(Individual) * (POP_SIZE));

    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);

    //clients = (Client *)malloc(sizeof(Client) * NUM_CLIENTS + 1);

    tournamentIndividuals = (Individual *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verificando alocacao de memoria
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return 0;
    }

    srand(time(NULL));

    // Initializating the population
    initPop(population);

    for (i = 0; solutionFound == 0; i++)
    {
        solutionFound = evolvePop(i, populationFitness, population, nextPop, tournamentFitness, tournamentIndividuals, solutionFound);
        printf("Round %d\n", i + 1);

        break;
    }


    // Calcular o tempo de execucao do programa
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
        fprintf(file, "Eliticity Rate: %f\n", ELITISMRATE);
        fprintf(file, "Rounds: %d\n", ROUNDS);
        fprintf(file, "Time: %f\n", time_spent);

        printf("\n");
        printf("Population Size: %d\n", POP_SIZE);
        printf("Number of Vehicles: %d\n", NUM_VEHICLES);
        printf("Number of Clients: %d\n", NUM_CLIENTS);
        printf("Vehicle Capacity: %d\n", VEHICLES_CAPACITY);
        if (SELECTION == 1)
            printf("Selection Type: Roulette\n");
        printf("Crossover Points: %d points\n", CROSSINGTYPE);
        printf("Mutation Rate: %f\n", MUTATIONRATE);
        printf("Eliticity Rate: %f\n", ELITISMRATE);
        printf("Rounds: %d\n", ROUNDS);
        printf("Time: %f\n", time_spent);

        fclose(file);
    }

    // printf("Teste número 2\n");
    //  Inicializando a populacao
    // initPop(population);
    //  Calculando o fitness
    // fitness(population, populationFitness, solutionFound);
    //  Selecionando os individuos
    // rouletteSelection(parent, populationFitness, population);
    // tournamentSelection(tournamentIndividuals, parent, tournamentFitness, populationFitness, population);
    //  Cruzamento
    // onePointCrossing(parent, nextPop);
    //  Mutacao
    // mutation(nextPop);

    // Liberando memoria alocada
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
    //free(clients);

    return 0;
}
