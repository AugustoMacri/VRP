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
        - After it will select randomly a point in the cromossome;
        - Then we will select another randomly point in the cromossome;
        - then we will change the position of the genes that were selected;

*/

void mutation(int *index, Individual *nextPop)
{
    printf("\n------------------------------------------------\n");
    printf("\tTESTANDO A MUTACAO (dois pontos aleatorios\n");
    printf("------------------------------------------------\n");
    int h, i, j, k;
    int mutate;
    int mutateIndividual;

    // Define if it will have or not mutation
    // mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

    mutateIndividual = 0; // Only for testing the mutation

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
        // Here we will select two points on the individual to mutate
        int point1, point2;
        for(i = 0; i < NUM_VEHICLES; i++){

            // Here we will make sure that the points are different
            do{
                point1 = rand() % (NUM_CLIENTS + 1);
            }while (nextPop[(*index) - 1].route[i][point1] == 0);

            do{
                point2 = rand() % (NUM_CLIENTS + 1);
            }while (nextPop[(*index) - 1].route[i][point2] == 0 || point1 == point2);

            printf("No veiculo %d: Ponto 1: %d Ponto 2: %d\n", i, point1, point2);


            // Here we will mutate the individual (change the position of the genes that were selected)
            int temp = nextPop[(*index) - 1].route[i][point1];
            nextPop[(*index) - 1].route[i][point1] = nextPop[(*index) - 1].route[i][point2];
            nextPop[(*index) - 1].route[i][point2] = temp;
        }

        printf("\nOCORRENDO MUTAÇÃO\n");
        for(i = 0; i < NUM_VEHICLES; i++){
            for(j = 0; j < NUM_CLIENTS + 1; j++){
                printf("%d ", nextPop[(*index) - 1].route[i][j]);
            }
            printf("\n");
        }
    }
}

/*
    -----------------------------------
              mutation2()
    -----------------------------------

    - This type of mutation not only affect negatively the individual fitness, but also affect the clients that are visited, so it will not be a good mutation.


    This function:
    - First we have to determine if will be or not mutation;
    - If the mutation occurs:
        - After it will select randomly a point in the cromossome that will be mutated;
        - Then the individual will be mutated;


    Script:
    - A gente precisa fazer a parte do indice, para editar a bosta do individuo de acordo com o indice


*/
void mutation2(int *index, Individual *nextPop)
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