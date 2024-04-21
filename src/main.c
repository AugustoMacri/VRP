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
int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int **populacaoAtual;

int main()
{
    printf("Programa em Execucao\n");

    int **nextPop, i;
    FILE *file;

    file = fopen("dataVRP.xls", "a+");

    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    else
    {
        fclose(file);
    }

    populacaoAtual = (int **)malloc(sizeof(int *) * NUM_CLIENTS); // populacao atual
    nextPop = (int **)malloc(sizeof(int *) * NUM_CLIENTS);        // proxima populacao

    for (i = 0; i < NUM_CLIENTS; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * NUM_CLIENTS);
        nextPop[i] = (int *)malloc(sizeof(int) * NUM_CLIENTS);
    }

    // Verificando alocacao de memoria
    if (populacaoAtual == NULL || nextPop == NULL)
    {
        printf("Falha ao alocar memoria!\n");
        return 0;
    }

    initPop();
    fitness();

    // Liberando memoria alocada
    free(*populacaoAtual);
    free(populacaoAtual);
    free(*nextPop);
    free(nextPop);

    return 0;
}
