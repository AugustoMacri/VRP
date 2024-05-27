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
    printf("\tTESTANDO A MUTACAO (dois pontos aleatorios)\n");
    printf("------------------------------------------------\n");
    int h, i, j, k;
    int mutate;
    int mutateIndividual;
    int ind;

    for (ind = (*index) - 2; ind < *index; ind++)
    {
        printf("Calculando se vai ter mutação nessa merda (adaptado para os dois filhos gerados)");
        // Define if it will have or not mutation
        // mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

        mutateIndividual = 0; // Only for testing the mutation

        printf("mutateIndividual: %d\n", mutateIndividual);

        printf("Individuo antes da mutacao: \n");
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                printf("%d ", nextPop[ind].route[i][j]);
            }
            printf("\n");
        }

        if (mutateIndividual == 0)
        {
            // Here we will select two points on the individual to mutate
            int point1, point2;
            for (i = 0; i < NUM_VEHICLES; i++)
            {

                // Here we will make sure that the points are different
                do
                {
                    point1 = rand() % (NUM_CLIENTS + 1);
                } while (nextPop[ind].route[i][point1] == 0);

                do
                {
                    point2 = rand() % (NUM_CLIENTS + 1);
                } while (nextPop[ind].route[i][point2] == 0 || point1 == point2);

                printf("No veiculo %d: Ponto 1: %d Ponto 2: %d\n", i, point1, point2);

                // Here we will mutate the individual (change the position of the genes that were selected)
                int temp = nextPop[ind].route[i][point1];
                nextPop[ind].route[i][point1] = nextPop[ind].route[i][point2];
                nextPop[ind].route[i][point2] = temp;
            }

            printf("\nOCORRENDO MUTAÇÃO\n");
            for (i = 0; i < NUM_VEHICLES; i++)
            {
                for (j = 0; j < NUM_CLIENTS + 1; j++)
                {
                    printf("%d ", nextPop[ind].route[i][j]);
                }
                printf("\n");
            }
        }
    }
}

/*
    -----------------------------------
              mutation2()
    -----------------------------------

    This function:
    - First we have to determine if will be or not mutation;
    - If the mutation occurs:
        - After it will select randomly a point in the cromossome;
        - then we will change the position of the genes that that are in the same column that were selected;

    - Considerações:
        - Seguinte, as outras mutações até agora, por mudarem o individuo de posição na linha, torna possivel
        que, quando ocorrer a seleção dos individuos e o cruzamento, pegue a metade de um individuo que tenha o cliente 1
        por exemplo, e a outra metade de outro individu que tenha também o cliente um, por exemplo.
        - Isso ocorre principalmente quando aumentamos a taxa de elitismo
        0 1 2 3 5 0 0 0
        0 6 6 4 0 0 0 0  (aqui o 6 se repetiu exatamente por esse motivo)

*/

void mutation2(int *index, Individual *nextPop)
{
    printf("\n------------------------------------------------\n");
    printf("\tTESTANDO A MUTACAO (Um ponto perpendicular)\n");
    printf("------------------------------------------------\n");
    int h, i, j, k;
    int mutate;
    int mutateIndividual;
    int vehicle = 0;
    int ind;

    for (ind = (*index) - 2; ind < *index; ind++)
    {

        // Define if it will have or not mutation
        // mutateIndividual = rand() % (int)(1.0 / MUTATIONRATE);

        mutateIndividual = 0; // Only for testing the mutation

        printf("mutateIndividual: %d\n", mutateIndividual);

        printf("Individuo antes da mutacao: \n");
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                printf("%d ", nextPop[ind].route[i][j]);
            }
            printf("\n");
        }

        if (mutateIndividual == 0)
        {
            int vehicle1, vehicle2;
            int point;

            do
            {
                vehicle1 = rand() % (NUM_VEHICLES);
                vehicle2 = rand() % (NUM_VEHICLES);
            } while (vehicle1 == vehicle2);

            // Here we will make sure that the points are different
            do
            {
                point = rand() % (NUM_CLIENTS + 1);
                printf("Ponto selecionado: %d\n", point);
            } while (nextPop[ind].route[vehicle1][point] == 0);

            printf("No veiculo %d: Ponto 1: %d\n", i, point);

            // Here we will mutate the individual (change the position of the genes that were selected)
            int temp = nextPop[ind].route[vehicle1][point];
            nextPop[ind].route[vehicle1][point] = nextPop[ind].route[vehicle2][point];
            nextPop[ind].route[vehicle2][point] = temp;

            printf("\nOCORRENDO MUTAÇÃO\n");
            for (i = 0; i < NUM_VEHICLES; i++)
            {
                for (j = 0; j < NUM_CLIENTS + 1; j++)
                {
                    printf("%d ", nextPop[ind].route[i][j]);
                }
                printf("\n");
            }
        }
    }
}