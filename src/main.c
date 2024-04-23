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

int main()
{
    printf("Programa em Execucao\n");

    int **nextPop, **parent;
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
    parent = (int **)malloc(sizeof(int *) * 2);

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
        nextPop[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    for(i = 0; i < 2; i++){

        parent[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));

    }

    // Verificando alocacao de memoria
    if (populacaoAtual == NULL || nextPop == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return 0;
    }

    initPop();
    fitness();
    printf("\nTESTANDO O CRUZAMENTO POR ROLETA\n");
    rouletteSelection(parent, populationFitness, populacaoAtual);

    // Liberando memoria alocada
    for (i = 0; i < NUM_CLIENTS + 1; i++){
        free(populacaoAtual[i]);
        free(nextPop[i]);
    }
    for(i = 0; i < 2; i++){
        free(parent[i]);
    }

    free(populacaoAtual);
    free(nextPop);
    free(populationFitness);
    free(parent);


    fclose(file);

    return 0;
}
