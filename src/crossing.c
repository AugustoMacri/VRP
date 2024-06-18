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

void indicaFaltantes(Individual *parent, int son[NUM_VEHICLES][NUM_CLIENTS + 1], int contRepetidos, int *vetorDeFaltantes)
{
    int index = 0;

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 1; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = parent[0].route[i][j];
            int found = 0;

            for (int l = 0; l < NUM_VEHICLES; l++)
            {
                for (int k = 1; k < NUM_CLIENTS + 1; k++)
                {
                    int val2 = son[l][k];
                    if (val1 == val2)
                    {
                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                    break;
                }
            }

            if (!found && val1 != 0)
            {
                vetorDeFaltantes[index] = val1;
                index++;
                if (index >= contRepetidos)
                {
                    break;
                }
            }
        }
        if (index >= contRepetidos)
        {
            break;
        }
    }
}

void indicaFaltantes2(Individual *parent, int son[NUM_VEHICLES][NUM_CLIENTS + 1], int contRepetidos, int *vetorDeFaltantes, int dadChosen)
{
    int index = 0;

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 1; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = parent[dadChosen].route[i][j];
            int found = 0;

            for (int l = 0; l < NUM_VEHICLES; l++)
            {
                for (int k = 1; k < NUM_CLIENTS + 1; k++)
                {
                    int val2 = son[l][k];
                    if (val1 == val2)
                    {
                        found = 1;
                        break;
                    }
                }
                if (found)
                {
                    break;
                }
            }

            if (!found && val1 != 0)
            {
                vetorDeFaltantes[index] = val1;
                index++;
                if (index >= contRepetidos)
                {
                    break;
                }
            }
        }
        if (index >= contRepetidos)
        {
            break;
        }
    }
}

// This function will compare the son with the father, that way we can make sure that every individual will have every client
int compareFatherSon(Individual *parent, int son[NUM_VEHICLES][NUM_CLIENTS + 1], int vehicleindex, int dadChosen)
{

    // int cont = 0;
    for (int j = 1; j < NUM_CLIENTS + 1; j++)
    {
        int val1 = parent[dadChosen].route[vehicleindex][j];
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
            return val1;
        }
    }

    return 0;
}

void onePointCrossing(int *index, Individual *parent, Individual *nextPop)
{
    int i, j, cut;
    int son[NUM_VEHICLES][NUM_CLIENTS + 1];

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            son[i][j] = 0;
        }
    }

    do
    {
        cut = rand() % VEHICLES_CAPACITY + 1; // generate a random number between 1 and NUM_CLIENTS + 1 -> Perhaps we can do this with VEHICLES_CAPACITY
    } while (cut == VEHICLES_CAPACITY + 1);

    // Copy the first half of the first parent to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < cut; j++)
        {
            son[i][j] = parent[0].route[i][j];
        }
    }
    // Copy the second half of the second parent to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut; j < NUM_CLIENTS + 1; j++)
        {
            son[i][j] = parent[1].route[i][j];
        }
    }

    /*/ Version 1 -> Here we choose  randomly between two fathers
    //-------------------------------------------------------------------------------------------------------------------------------
    int vetDadsChosen[NUM_VEHICLES];
    // The follow verification make sure that each son do not have repeated clients
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        // First we need to generate a random value that's gona determine wich dad will be the reference
        int dadChosen;
        do
        {
            dadChosen = rand() % 2; // chese a number between 0 and 1
        } while (dadChosen == vetDadsChosen[i - 1] && i != 0);

        vetDadsChosen[i] = dadChosen;

        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    int substituto = compareFatherSon(parent, son, i, dadChosen);
                    son[i][k] = substituto;
                }
            }
        }
    }
    //-------------------------------------------------------------------------------------------------------------------------------*/

    // Version2
    //-------------------------------------------------------------------------------------------------------------------------------
    int contRepetidos = 0;
    // Contando quantos repetidos tem
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    contRepetidos++;
                }
            }
        }
    }

    int *vetorDeFaltantes = malloc(contRepetidos * sizeof(int));
    if (vetorDeFaltantes == NULL)
    {
        return;
    }

    indicaFaltantes(parent, son, contRepetidos, vetorDeFaltantes);

    // The follow verification make sure that each son do not have repeated clients
    int indexFaltantes = 0;
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    son[i][k] = vetorDeFaltantes[indexFaltantes];
                    indexFaltantes++;
                }
            }
        }
    }

    free(vetorDeFaltantes);
    //-------------------------------------------------------------------------------------------------------------------------------*/

    /*/ Version3
    //-------------------------------------------------------------------------------------------------------------------------------

    int dadChosen;
    dadChosen = rand() % 2;

    int contRepetidos = 0;
    // Contando quantos repetidos tem
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    contRepetidos++;
                }
            }
        }
    }

    int *vetorDeFaltantes = malloc(contRepetidos * sizeof(int));
    if (vetorDeFaltantes == NULL)
    {
        return;
    }

    indicaFaltantes2(parent, son, contRepetidos, vetorDeFaltantes, dadChosen);

    // The follow verification make sure that each son do not have repeated clients
    int indexFaltantes = 0;
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            int val1 = son[i][j];
            for (int k = j; k < NUM_CLIENTS + 1; k++)
            {
                int val2 = son[i][k];
                if (k != j && val1 == val2 && val1 != 0 && val2 != 0)
                {
                    son[i][k] = vetorDeFaltantes[indexFaltantes];
                    indexFaltantes++;
                }
            }
        }
    }

    free(vetorDeFaltantes);
    //-------------------------------------------------------------------------------------------------------------------------------*/

    // Adding the son to the nextPop
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son[i][j];
        }
    }

    *index = (*index) + 1;
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
    int son[NUM_VEHICLES][NUM_CLIENTS + 1];

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            son[i][j] = 0;
        }
    }

    do
    {
        cut1 = rand() % (NUM_CLIENTS + 1);
        cut2 = rand() % (NUM_CLIENTS + 1);
    } while (cut1 == NUM_CLIENTS + 1 || cut2 == NUM_CLIENTS + 1 || cut1 == cut2 || cut1 == 0 || cut2 == 0 || cut1 > cut2);

    printf("Corte: %d\n", cut1);
    printf("Corte: %d\n", cut2);

    // Copy the first third of the parents to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < cut1; j++)
        {
            son[i][j] = parent[0].route[i][j];
        }
    }

    // Copy the second third of the parents to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut1; j < cut2; j++)
        {
            son[i][j] = parent[1].route[i][j];
        }
    }

    // Copy the third third of the parents to the son
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = cut2; j < NUM_CLIENTS + 1; j++)
        {
            son[i][j] = parent[0].route[i][j];
        }
    }

    // Adding the son to the nextPop
    printf("FILHO 1: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            nextPop[*index].route[i][j] = son[i][j];
            printf("%d ", nextPop[*index].route[i][j]);
        }
        printf("\n");
    }

    *index = (*index) + 1;
}