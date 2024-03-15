#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "print.h"
#include "initialization.h"

/*
This part of the code is for initialize the population.
The initialization is done using the Gillett & Miller algorithm.

After initialize, the cromossome will recive the population
*/

/*
    -----------------------------------
              initPop()
    -----------------------------------

    This function:
    - The population is crated randomly;

*/

void initPop()
{
    int i, j, k, l;
    int cont;

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS; j++)
        {

            populacaoAtual[i][j] = rand() % NUM_CLIENTS;

            do
            {
                cont = 0;
                if (i > 0)
                {
                    for (k = 0; k < i; k++)
                    {
                        for (l = 0; l < NUM_CLIENTS; l++)
                        {
                            if (populacaoAtual[i][j] == populacaoAtual[k][l])
                            {
                                cont++;
                            }
                        }
                    }
                }
                for (k = 0; k < NUM_CLIENTS; k++)
                {
                    populacaoAtual[i][k] = rand() % NUM_CLIENTS;
                }

            } while (cont == NUM_CLIENTS);
        }
    }

    printf("*****Populacao inicial criada*****\n");
    showPopulation();
}