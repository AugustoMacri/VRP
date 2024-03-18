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
    - The population is crated using the Gillet & Miller algorithm:
        - First of all we need to put the coordinates of the distribution center in 50,50;
        - Then we need to put the coordinates of the clientes in a random way; (just for testing)
        - Then we will need to calculate the distance between the distribution center and the clients;
        - Then we need to group all the clients, that way we will group them in order from the closest to the furthest from de distribution center.
        - Then, using the capacity of each of the vehicles, we will put the vehicles to visit each client, but the number of clients that each vehicle will visit can't
        be greater than VEHICLES_CAPACITY.
        - After that we will implement the greedy algorithm for, rather than the vehicle visit the closest client, and then the another closest, and so on
        the vehicle will visit another client that are closer to the client that he is now, not the closest from the distribution center;

*/

typedef struct
{
    double x;
    double y;
} Coordinates;

// The objective of thies function is to compare the distance values between two points of the array distance.
int compare(const void *a, const void *b)
{
    double *distanceA = (double *)a;
    double *distanceB = (double *)b;
    return (*distanceA > *distanceB) - (*distanceA < *distanceB);
}

void initPop2()
{
    // Using the gillet & miller algorithm
    int i, j, k, l;
    int cont, vehicleAtendence[NUM_VEHICLES][NUM_CLIENTS] = {0};
    double distance[NUM_CLIENTS];

    srand(time(NULL));

    Coordinates d_center = {50.0, 50.0}; // Distribution center
    Coordinates clients[NUM_CLIENTS];    // Clients coordinates

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

    qsort(distance, NUM_CLIENTS, sizeof(double), compare); // Sorting the distances, from the closest to the furthest

    for (j = 0; j < NUM_CLIENTS; j++)
    {
        printf("Cliente: %d Coordenada x: %.2f Coordenada y: %.2f Distance: %.2f\n", j + 1, clients[j].x, clients[j].y, distance[j]);
    }

    // Traversing the clients and grouping them
    /*
    The group will be as follows:
    The VEHICLE_CAPACITY clients that are closest to the distribution center, will be visited by the first vehicle;
    The others NUM_CLIENTS clients will be visited by the second vehicle, and so on;
    That way, if there is less than VEHICLE_CAPACITY clients, they will be visited by the last vehicle, so is possible that the last vehicle will have less than VEHICLE_CAPACITY clients.
    */
    int currentClient = 0;

    for (i = 0; i < NUM_VEHICLES; i++)
    {
        printf("\nVehicle: %d ", i + 1);
        for (j = 0; j < VEHICLES_CAPACITY; j++)
        {

            if (currentClient < NUM_CLIENTS)
            {
                vehicleAtendence[i][currentClient] = 1;
                printf("client %d (%.2f, %.2f)", currentClient + 1, clients[currentClient].x, clients[currentClient].y);

                currentClient++;
            }
        }
    }
    printf("\n");
    for (i = 0; i < NUM_VEHICLES; i++)
    {
        for (j = 0; j < NUM_CLIENTS; j++)
        {
            printf("%d ", vehicleAtendence[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /*
    After grouping the clients, we need to use the Greedy Algorithm to create the initial population.
    */
}