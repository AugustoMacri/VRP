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

double distance_clients[NUM_VEHICLES][NUM_CLIENTS + 1];
double time_clients_end[NUM_VEHICLES][NUM_CLIENTS + 1];
int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int **populacaoAtual;
int *populationFitness;
int *tournamentFitness;
Individual *population, *parent;
Individual *tournamentIndividuals;
Individual *nextPop;

int main()
{
    printf("Programa em Execucao\n");

    int **nextPop;// **parent;
    int i;
    FILE *file;

    file = fopen("dataVRP.xls", "a+");

    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    
    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1)); // populacao atual
    nextPop = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1));        // proxima populacao
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    population = (Individual *)malloc(sizeof(Individual) * POP_SIZE);
    parent = (Individual *)malloc(sizeof(Individual) * 2);
    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    nextPop = (int *)malloc(sizeof(Individual) * (POP_SIZE));

    tournamentIndividuals = (int *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    

    // Verificando alocacao de memoria
    if (populacaoAtual == NULL || nextPop == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return 0;
    }

    // Inicializando a populacao
    initPop();
    // Calculando o fitness
    fitness();
    // Selecionando os individuos
    rouletteSelection(parent, populationFitness, population);
    //tournamentSelection(tournamentIndividuals, parent, tournamentFitness, populationFitness, population);
    // Cruzamento
    onePointCrossing(parent, nextPop);
    // Mutacao
    mutation(nextPop);

    // Liberando memoria alocada
    for (i = 0; i < NUM_CLIENTS + 1; i++){
        free(populacaoAtual[i]);
    }

    free(populacaoAtual);
    free(populationFitness);
    free(population);
    free(parent);
    free(tournamentFitness);
    free(tournamentIndividuals);
    free(nextPop);


    fclose(file);

    return 0;
}
