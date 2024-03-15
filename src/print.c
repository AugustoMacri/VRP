#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "print.h"

/*
This part of the code is for printing the representations of our chromossome.
So, the idea is to print the cromossome in the following way:

Imagine that there is 2 vehicles (lines) and 4 clients (columns). The first vehicle has 2 clients (A and D) and the second vehicle has 2 clients (B and C).
1001
0110

Whith that in mind, first we need to print a matrix with the clients and vehicles that are in the main.h.
So we need to create a matrix with the dimensions of the vehicles and clients.
Then, we need to print the matrix with the chromossome.

*/

/*
    -----------------------------------
              showPopulation()
    -----------------------------------

    This function:
    - Prints the population and its fitness;
    - Show the best individual (higher fitness);
    - Show the average fitness;
    - Save all the data in the file "dataVRP.xls".

*/

void showPopulation(){
    int i, j;
    FILE *file;
    float avgFitness = 0.0;

    file = fopen("dataVRP.xls", "a+");
    if(file == NULL){
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
    }else{
        for(i = 0; i < NUM_VEHICLES; i++){
            for(j = 0; j < NUM_CLIENTS; j++){
                //printf("0 ");

                printf("%d ", populacaoAtual[i][j]);
                fprintf(file, "%d ", populacaoAtual[i][j]);
            }
            printf("\n");
            fprintf(file, "\n");
        }
        printf("\n");
        fprintf(file, "\n");

    }

    printf("%f", avgFitness);
    fclose(file);


}