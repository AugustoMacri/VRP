#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "main.h"
#include "print.h"
#include "initialization.h"

/*
This part of the code is for initialize the population.
The initialization is done using the Gillett & Miller algorithm (strategy A).

After initialize, the cromossome will recive the population
*/

/*
    -----------------------------------
              initPop()
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

// The objective of this function is to compare the distance values between two points of the array distance.
int compare(const void *a, const void *b)
{
    Client *coordA = (Client *)a;
    Client *coordB = (Client *)b;
    return (coordA->distance > coordB->distance) - (coordA->distance < coordB->distance);
}

// This function calculates the distance between two points.
double calculateDistance(Client c1, Client c2)
{
    return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}

// Function to find closest client to a given client
int findClosestClient(int currentClient, Client clients[], int visited[])
{

    double minDistance = DBL_MAX; 
    int i, closestClient;

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        if (i != currentClient && visited[i] == 0)
        {
            clients[i].distance = calculateDistance(clients[i], clients[currentClient]);
            if (clients[i].distance < minDistance)
            {
                minDistance = clients[i].distance;
                closestClient = i;
            }
        }
    }
    return closestClient;
}

void initPop(Individual *population)
{
    printf("\n------------------------------------------------\n");
    printf("\tIniciando Populacao\n");
    printf("------------------------------------------------\n");

    // Using the gillet & miller algorithm
    int i, j, k, l;

    for (int h = 0; h < POP_SIZE; h++)
    {
        printf("\n=========== Population %d ===========", h + 1);

        int visited[NUM_CLIENTS + 1] = {0}; // keep track of visited clients
        
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                // distance_clients[i][j] = 0;
                // time_clients_end[i][j] = 0;
            }
        }

        srand(time(NULL));

        Client d_center = {RANGE_COORDINATES / 2, RANGE_COORDINATES / 2}; // Distribution center -> Always in the middle of the graph


        // printf("-------------Clientes desordenados-------------\n");

        // Distribution center
        clients[0].x = RANGE_COORDINATES / 2;
        clients[0].y = RANGE_COORDINATES / 2;
        clients[0].distance = 0.0;

        for (i = 1; i < NUM_CLIENTS + 1; i++)
        {
            double x = (double)(rand() % RANGE_COORDINATES);
            double y = (double)(rand() % RANGE_COORDINATES);

            clients[i].x = x;
            clients[i].y = y;

            // Calculating the distance between the client and the distribution center (using two points distance)
            clients[i].distance = calculateDistance(clients[i], d_center);

            // printf("Cliente: %d Coordenada x: %.2f Coordenada y: %.2f Distance: %.2f\n", i, clients[i].x, clients[i].y, clients[i].distance);
        }


        // Sorting the distances, from the closest to the furthest
        qsort(clients, NUM_CLIENTS + 1, sizeof(Client), compare);

        // printf("-------------Clientes ordenados-------------\n");
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            /* Define the time window of each client
            clients[j].start_time = currentStartTime;
            clients[j].end_time = fmin(currentStartTime + WINDOW_SIZE, 20.0);
            currentStartTime = clients[j].end_time;

            time_clients_end[j] = clients[j].end_time; //saving the time of each client
            */

            // printf("Cliente: %d Coordenada x: %.2f Coordenada y: %.2f Distance: %.2f\n", j, clients[j].x, clients[j].y, clients[j].distance);
        }

        /*
        Traversing the clients and grouping them
        The group will be as follows:
        The VEHICLE_CAPACITY clients that are closer to the distribution center, will be visited by the first vehicle;
        The others NUM_CLIENTS clients will be visited by the second vehicle, and so on;
        That way, if there is less than VEHICLE_CAPACITY clients, they will be visited by the last vehicle, so is possible that the last vehicle will have less than VEHICLE_CAPACITY clients.
        After grouping the clients, we need to use the Greedy Algorithm to create the initial population.
        The objective of the Greedy Algorithm is to visit the closest client first, and then the closest to the first client, and so on.
        The Greedy Algorithm is as follows:
        */

        int currentClient = 0;

        for (i = 0; i < NUM_VEHICLES; i++)
        {
            printf("\nVehicle %d: ", i + 1);

            currentClient = 0;
            double currentStartTime = MAX_START_TIME;

            for (j = 0; j < VEHICLES_CAPACITY + 1; j++)
            {

                if (currentClient < NUM_CLIENTS + 1)
                {
                    visited[currentClient] = 1;

                    population[h].route[i][j] = currentClient;

                    distance_clients[h].route[i][currentClient] = clients[currentClient].distance;

                    clients[currentClient].start_time = currentStartTime;
                    clients[currentClient].end_time = fmin(currentStartTime + WINDOW_SIZE, 20.0);
                    currentStartTime = clients[currentClient].end_time;

                    time_clients_end[h].route[i][currentClient] = clients[currentClient].end_time;

                    printf("client %d (%.2f, %.2f) End: %.2f |", currentClient, clients[currentClient].x, clients[currentClient].y, clients[currentClient].end_time);

                    int nextClient = findClosestClient(currentClient, clients, visited);

                    currentClient = nextClient;
                }

                if (j == VEHICLES_CAPACITY)
                {
                    printf(" Returning to the distribution center\n");
                }
            }
        }

        //Nao esta considerando, ate o momento, na inicializacao, a distancia de volta ao deposito, mas e recalculado no fitness
        // showPopulation(h);
        /*
        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf("testando print usando time clients\n");
        for (i = 0; i < NUM_VEHICLES; i++)
        {
            for (j = 0; j < NUM_CLIENTS + 1; j++)
            {
                //printf("%f ", time_clients_end[h].route[i][j]);
                //printf("%f ", distance_clients[h].route[i][j]);
                //printf("%d ", population[h].route[i][j]);
            }
            printf("\n");
        }

        printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        */
    }
}