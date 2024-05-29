#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "crossing.h"

/*
    -----------------------------------
              onePointCrossing()
    -----------------------------------

    This function:
    - Will select randomly a point in the cromossome;
    - After it will gona create a new individual with the first half of the first individual and the second half of the second individual;
    - And then, another individual will be created with the first half of the second individual and the second half of the first individual;
    - Both of these individuals will be added to the next population;
    (the 1 point crossing is creating only 1 individual)

    Script:
    - First select two cromossomes from the population;
    - The selection can be done with tournament or Roulette;
    - Then the crossing will be done with one point;
    - The new individual will be added to the nextPop;

*/

int compareFatherSon(Individual *parent, int son[NUM_VEHICLES][NUM_CLIENTS + 1], int vehicleindex)
{
    printf("----------------Função CompareFatherSon-----------------------\n");
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", son[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // int cont = 0;
    for (int j = 1; j < NUM_CLIENTS + 1; j++)
    {
        int val1 = parent[0].route[vehicleindex][j]; // We allways will use the first father as reference
        int found = 0;

        for (int k = 1; k < NUM_CLIENTS + 1; k++)
        {
            int val2 = son[vehicleindex][k];
            if (val1 == val2 && val1 != 0 && val2 != 0)
            {
                found = 1;
                break;
            }
        }

        if (found == 0)
        {
            printf("O valor que não tem na linha %d do filho é o %d \n", vehicleindex, val1);
            return val1;
        }
    }

    return 0; // Return 0 if every client is present
}

void onePointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    printf("---------------------------------------------\n");
    printf("REALIZANDO O CRUZAMENTO DE UM PONTO:\n");
    printf("---------------------------------------------\n");
    int i, j, cut;
    int son1[NUM_VEHICLES][NUM_CLIENTS + 1];
    int son2[NUM_VEHICLES][NUM_CLIENTS + 1];

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = 0;
            son2[i][j] = 0;
        }
    }

    do
    {
        cut = rand() % (NUM_CLIENTS + 1) + 1; // generate a random number between 1 and NUM_CLIENTS + 1
    } while (cut == NUM_CLIENTS + 1);

    printf("Corte: %d\n", cut);

    // Copy the first half of the first parent to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < cut; j++)
        {
            son1[i][j] = parent[0].route[i][j];
            son2[i][j] = parent[1].route[i][j];
        }
    }
    // Copy the second half of the second parent to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = parent[1].route[i][j];
            son2[i][j] = parent[0].route[i][j];
        }
    }

    // The follow verification make sure that each son do not have repeated clients
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son1[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son1[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    printf("Cliente %d Repetido, Verificando qual cliente falta na pai apra substituí-lo\n", val2);
                    int substituto = compareFatherSon(parent, son1, i);
                    printf("SUBSTITUTO: %d\n", substituto);
                    son1[i][k] = substituto;
                }
            }
        }
    }

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son2[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son2[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    printf("Cliente %d Repetido, Verificando qual cliente falta na pai apra substituí-lo\n", val2);
                    int substituto = compareFatherSon(parent, son2, i);
                    printf("SUBSTITUTO: %d\n", substituto);
                    son2[i][k] = substituto;
                }
            }
        }
    }

    // Adding both sons to the nextPop
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son1[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    *index = (*index) + 1;

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son2[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;

    printf("\n");
}

/*
    -----------------------------------
              twoPointCrossing()
    -----------------------------------

    This function:
    - Will select randomly two points in the cromossome;
    - After it will gona create a new individual with the first 1/3 of the first individual and the second 1/3 of the second individual, and again the third 1/3 of the first individual;
    - And then, another individual will be created with the rest of the first individual and the rest of the second individual;
    - Both of these individuals will be added to the next population;

    Script:
    - First select two cromossomes from the population;
    - The selection can be done with tournament or Roulette;
    - Then the crossing will be done with one point;
    - Then both new individual will be added to the nextPop;

*/

void twoPointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    printf("---------------------------------------------\n");
    printf("REALIZANDO O CRUZAMENTO DE DOIS PONTOS:\n");
    printf("---------------------------------------------\n");
    int i, j, cut1, cut2;
    int son1[NUM_VEHICLES][NUM_CLIENTS + 1];
    int son2[NUM_VEHICLES][NUM_CLIENTS + 1];

    // First of all we will initialize both of the sons
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = 0;
            son2[i][j] = 0;
        }
    }

    // Select randomly two points of cut in the cromossome
    do
    {
        cut1 = rand() % (NUM_CLIENTS + 1);
        cut2 = rand() % (NUM_CLIENTS + 1);
    } while (cut1 == NUM_CLIENTS + 1 || cut2 == NUM_CLIENTS + 1 || cut1 == cut2 || cut1 == 0 || cut2 == 0 || cut1 > cut2);

    printf("Corte: %d\n", cut1);
    printf("Corte: %d\n", cut2);

    // Copy the first third of the parents to the sons
    // printf("primeiro 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < cut1; j++)
        {
            son1[i][j] = parent[0].route[i][j];
            son2[i][j] = parent[1].route[i][j];
        }
    }

    // Copy the second third of the parents to the sons
    // printf("segundo 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut1; j < cut2; j++)
        {
            son1[i][j] = parent[1].route[i][j];
            son2[i][j] = parent[0].route[i][j];
        }
    }

    // Copy the third third of the parents to the sons
    // printf("terceiro 1/3 dos filhos\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut2; j < NUM_CLIENTS + 1; j++)
        {
            son1[i][j] = parent[0].route[i][j];
            son2[i][j] = parent[1].route[i][j];
        }
    }

    // Adding the both sons to the nextPop
    printf("FILHO 1: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son1[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;

    printf("FILHO 2: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son2[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;

    printf("\n");
}