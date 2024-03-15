#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "print.h"
#include "initialization.h"

/*
This part of the code is for initialize the population.
The initialization is done (randomly) or (using the Gillett & Miller algorithm (strategy A)).

After initialize, the cromossome will recive the population
*/

/*
    -----------------------------------
              initPop()
    -----------------------------------

    This function:
    - The population (in this case) is crated randomly;

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

    printf("\n*****Populacao inicial criada*****\n");
    showPopulation();
}

/*
    -----------------------------------
              initPop2()
    -----------------------------------

    This function:
    - The population is crated using the Gillet & Miller algorithm;
    - First of all we need to put the coordinates of the distribution center in 0,0;
    - Then we need to put the coordinates of the clientes in a random way;

*/

typedef struct
{
    double x;
    double y;
} Coordinates;

void initPop2()
{
    // Using the gillet & miller algorithm
    int i, j, k, l;
    int cont;
    double distance[NUM_CLIENTS];

    srand(time(NULL));

    Coordinates d_center = {0.0, 0.0}; // Distribuition center
    Coordinates clients[NUM_CLIENTS];  // Clients coordinates

    for (i = 0; i < NUM_CLIENTS; i++)
    {
        double x = (double)(rand() % RANGE_COORDINATES);
        double y = (double)(rand() % RANGE_COORDINATES);

        // Calculating the distance between the client and the distribution center (using two points distance)
        distance[i] = sqrt(pow(x - d_center.x, 2) + pow(y - d_center.y, 2));

        // Saving the coordinates
        clients[i].x = x;
        clients[i].y = y;
    }

    for (j = 0; j < NUM_CLIENTS; j++)
    {
        printf("Cliente: %d Coordenada x: %.2f Coordenada y: %.2f Distance: %.2f\n", j + 1, clients[j].x, clients[j].y, distance[j]);
    }
}