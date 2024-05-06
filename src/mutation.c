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

void mutation(Individual *nextPop)
{
    int h, i, j, k;
    int mutate;
    int mutateIndividual;

    // Define if it will have or not mutation
    //mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

    mutateIndividual = 0;

    printf("mutateIndividual: %d\n", mutateIndividual);

    printf("Individuo antes da mutacao: \n");
    for(i = 0; i < NUM_VEHICLES; i++)
    {
        for(j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", nextPop[0].route[i][j]);
        }
        printf("\n");
    }

    if (mutateIndividual == 0)
    {

        printf("OCORRENDO MUTAÇÃO\n");
        mutate = rand() % (NUM_CLIENTS) + 1; // Choose a gene to mutate (except the first one)

        printf("O gene a ser mutado: %d\n", mutate);

        for (i = 0; i < NUM_VEHICLES; i++)
        {
            // change the gene selected with the gene of the past
            int temp = nextPop[0].route[i][mutate];
            nextPop[0].route[i][mutate] = nextPop[0].route[i][mutate - 1];
            nextPop[0].route[i][mutate - 1] = temp;
        }

        printf("\nIndividuo depois da mutacao: \n");
        for(i = 0; i < NUM_VEHICLES; i++){
            for(j = 0; j < NUM_CLIENTS + 1; j++){
                printf("%d ", nextPop[0].route[i][j]);
            }
            printf("\n");
        }
    }
}