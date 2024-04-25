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

double ***distance_clients;
double ***time_clients_end;
int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int ***populacaoAtual;
int *populationFitness;
int **tournamentIndividuals;
int *tournamentFitness;

int main()
{
    printf("Programa em Execucao\n");

    int **nextPop, ***parent;
    int i;
    FILE *file;

    file = fopen("dataVRP.xls", "a+");

    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }

    // Declarando a matriz 3D de populacaoAtual
    populacaoAtual = malloc(POP_SIZE * sizeof(int **));
    for (int i = 0; i < POP_SIZE; i++)
    {
        populacaoAtual[i] = malloc(NUM_VEHICLES * sizeof(int *));
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            populacaoAtual[i][j] = malloc((NUM_CLIENTS + 1) * sizeof(int));
        }
    }

    // Declarando a matriz 3D distanceClients
    distance_clients = malloc(POP_SIZE * sizeof(double **));
    for (int i = 0; i < POP_SIZE; i++)
    {
        distance_clients[i] = (double **)malloc(NUM_VEHICLES * sizeof(double *));
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            distance_clients[i][j] = (double *)malloc((NUM_CLIENTS + 1) * sizeof(double));
        }
    }

    // Declarando a matriz 3D timeClientsEnd
    time_clients_end = malloc(POP_SIZE * sizeof(double **));
    for (int i = 0; i < POP_SIZE; i++)
    {
        time_clients_end[i] = (double **)malloc(NUM_VEHICLES * sizeof(double *));
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            time_clients_end[i][j] = (double *)malloc((NUM_CLIENTS + 1) * sizeof(double));
        }
    }

    // Alocando memória para parent
    parent = malloc(2 * sizeof(int **));
    for (int i = 0; i < 2; i++)
    {
        parent[i] = malloc(POP_SIZE * sizeof(int *));
        for (int j = 0; j < POP_SIZE; j++)
        {
            parent[i][j] = malloc((NUM_CLIENTS + 1) * sizeof(int));
        }
    }

    nextPop = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1)); // proxima populacao
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    tournamentIndividuals = (int **)malloc(sizeof(int *) * QUANTITYSELECTED);
    tournamentFitness = (int *)malloc(sizeof(int) * QUANTITYSELECTED);

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        // populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
        nextPop[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    for (i = 0; i < QUANTITYSELECTED; i++)
    {
        tournamentIndividuals[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verificando alocacao de memoria
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || parent == NULL || tournamentIndividuals == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return 0;
    }

    // Chamando as Funções
    initPop();
    fitness();
    printf("\nTESTANDO O CRUZAMENTO POR ROLETA\n");
    rouletteSelection(parent, populationFitness, populacaoAtual);
    // printf("\nTESTANDO O CRUZAMENTO POR TORNEIO\n");
    // tournamentSelection(tournamentIndividuals, parent, tournamentFitness, populationFitness, populacaoAtual);

    // Liberando memoria alocada

    // Liberando a memória da populacaoAtual
    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            free(populacaoAtual[i][j]);
        }
        free(populacaoAtual[i]);
    }
    free(populacaoAtual);

    // Liberando a memória da distanceClients
    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            free(distance_clients[i][j]);
        }
        free(distance_clients[i]);
    }
    free(distance_clients);

    // Liberando a memória da timeClientsEnd
    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            free(time_clients_end[i][j]);
        }
        free(time_clients_end[i]);
    }
    free(time_clients_end);

    // Liberando memória de parent
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < POP_SIZE; j++)
        {
            free(parent[i][j]);
        }
        free(parent[i]);
    }

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        free(nextPop[i]);
    }
    for (i = 0; i < 2; i++)
    {
        free(parent[i]);
    }

    free(nextPop);
    free(populationFitness);
    free(parent);
    free(*tournamentIndividuals);
    free(tournamentIndividuals);
    free(tournamentFitness);

    fclose(file);

    return 0;
}
