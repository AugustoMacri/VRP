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
int findClosestClient(int currentClient, Client *clients, int visited[])
{

    double minDistance = DBL_MAX;
    int i, closestClient = -1;

    for (i = 0; i < NUM_CLIENTS; i++)
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

void initPop(Individual *population, Client *clients)
{
    // Teste para depuração:
    // printf("Depurando na funcao initpop\n");
    // for (int g = 0; g < NUM_CLIENTS; g++)
    //{
    //    printf("cliente %d demanda %d\n", g, clients[g].demand);
    //}

    // Using the Gillet & Miller algorithm
    int i, j, k, l;

    for (int h = 0; h < POP_SIZE; h++)
    {
        int visited[NUM_CLIENTS] = {0}; // keep track of visited clients

        // Calculando as distâncias entre o cliente e o centro de distribuição
        for (i = 0; i < NUM_CLIENTS; i++)
        {
            clients[i].distance = calculateDistance(clients[i], clients[0]);
        }

        // Ordenando as distâncias
        qsort(clients, NUM_CLIENTS, sizeof(Client), compare);

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

        //printf("Clientes depois de serem organizados\n");
        //for (i = 0; i < NUM_CLIENTS; i++)
        //{
        //    printf("cliente %d distancia %f\n", clients[i].id, clients[i].distance);
        //}

        int currentClient = 0; // Inicia do primeiro cliente

        for (i = 0; i < NUM_VEHICLES; i++)
        {
            currentClient = 0;
            double currentStartTime = MAX_START_TIME;
            int currentCapacity = 0; // Capacidade atual do veículo
            int j = 0;               // Cliente atual na rota

            // printf("Veículo %d iniciado. Capacidade atual: %d\n", i, currentCapacity);

            while (currentCapacity < VEHICLES_CAPACITY && currentClient < NUM_CLIENTS)
            {
                int currentDemand = clients[currentClient].demand;
                // printf("Tentando adicionar cliente %d com demanda %d\n", currentClient, currentDemand);

                if (currentCapacity + currentDemand <= VEHICLES_CAPACITY)
                {
                    visited[currentClient] = 1;
                    population[h].route[i][j] = clients[currentClient].id; // Before we were putting the position of the client in the array, now we are putting the id of the client
                    distance_clients[h].route[i][currentClient] = clients[currentClient].distance;

                    clients[currentClient].start_time = clients[currentClient].readyTime; //Mudamos aqui para readyTime e também removemos a parte que soma meia hora por cliente
                    currentStartTime = clients[currentClient].end_time;

                    time_clients_end[h].route[i][currentClient] = clients[currentClient].dueDate; //vai ser necessário ainda?

                    currentCapacity += currentDemand;
                    j++;

                    // printf("Cliente %d adicionado ao veículo %d. Capacidade atual: %d\n", currentClient, i, currentCapacity);

                    int nextClient = findClosestClient(currentClient, clients, visited);
                    // printf("Cliente atual: %d | Próximo cliente: %d\n", currentClient, nextClient);

                    if (nextClient == -1)
                    {
                        // printf("There's not a next client %d.\n", i);
                        break;
                    }

                    currentClient = nextClient;
                }
                else
                {
                    // printf("Capacidade excedida para o cliente %d.\n", currentClient);
                    break;
                }
            }
        }

        // Inicializando o fitness de cada indivíduo
        population[h].id = h;
        population[h].fitness = 0;
        population[h].fitnessDistance = 0;
        population[h].fitnessTime = 0;
        population[h].fitnessFuel = 0;
        population[h].fitnessCapacity = 0;
    }

    //printPopulation(population);
}

// Function to distribute the population initialized in subpopulations
// Even if in that case all the individual in the initial population are equal, the correct way to divide it is selecting different individuals, not the same always
void distributeSubpopulation(Individual *population)
{

    for (int i = 0; i < POP_SIZE; i++)
    {
        int index = i / SUBPOP_SIZE;
        int index2 = i % SUBPOP_SIZE;

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS; k++)
            {
                switch (index)
                {
                case 0:
                    subPopDistance[index2].route[j][k] = population[i].route[j][k];
                    subPopDistance[index2].id = population[i].id;
                    break;

                case 1:
                    subPopTime[index2].route[j][k] = population[i].route[j][k];
                    subPopTime[index2].id = population[i].id;
                    break;

                case 2:
                    subPopFuel[index2].route[j][k] = population[i].route[j][k];
                    subPopFuel[index2].id = population[i].id;
                    break;

                default:
                    break;
                }

                // Adding individual to subPopWeighting (it will be any individual, even if it already is on another subpop)
                subPopWeighting[index2].route[j][k] = population[i].route[j][k];
                subPopWeighting[index2].id = population[i].id;
            }
        }
    }
}

void printPopulation(Individual *population)
{
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        printf("  Veículo %d: ", i);
        for (int j = 0; j < NUM_CLIENTS; j++)
        {
            int clientId = population[1].route[i][j];
            if (clientId == -1) // Indicador de fim de rota
                break;
            printf("%d ", clientId);
        }
        printf("\n");
    }
    printf("\n");
}