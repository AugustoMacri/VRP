#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "mutation.h"

/*
    -----------------------------------
              mutation()
    -----------------------------------

    This function:
    - First we have to determine if will be or not mutation;
    - If the mutation occurs:
        - After it will select randomly a point in the cromossome that will be mutated;
        - Then the individual will be mutated;

    Script:
    - A gente precisa fazer a parte do indice, para editar a bosta do individuo de acordo com o indice

*/

void mutation(int *index, Individual *nextPop)
{
    printf("\n------------------------------------------------\n");
    printf("\tTESTANDO A MUTACAO\n");
    printf("------------------------------------------------\n");
    int h, i, j, k;
    int mutate;
    int mutateIndividual;

    // Define if it will have or not mutation
    // mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

    mutateIndividual = 0;

    printf("mutateIndividual: %d\n", mutateIndividual);

    printf("Individuo antes da mutacao: \n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", nextPop[(*index) - 1].route[i][j]);
        }
        printf("\n");
    }

    if (mutateIndividual == 0)
    {

        printf("\nOCORRENDO MUTAÇÃO\n");
        // here we will put a condition to cut untill de capacity of the vehicle
        mutate = rand() % (VEHICLES_CAPACITY + 1) + 2; // Choose a gene to mutate (except the first two)
        // that way, the mutation will happend more efficiently

        printf("O gene a ser mutado: %d\n", mutate);

        for (i = 0; i < NUM_VEHICLES; i++)
        {
            // change the gene selected with the gene of the past
            int temp = nextPop[(*index) - 1].route[i][mutate];
            nextPop[(*index) - 1].route[i][mutate] = nextPop[(*index) - 1].route[i][mutate - 1];
            nextPop[(*index) - 1].route[i][mutate - 1] = temp;
        }

        printf("\nIndividuo depois da mutacao: \n");
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                printf("%d ", nextPop[(*index) - 1].route[i][j]);
            }
            printf("\n");
        }
    }
}