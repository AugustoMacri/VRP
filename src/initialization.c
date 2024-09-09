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
    int i, closestClient = -1;

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
    // Using the gillet & miller algorithm
    int i, j, k, l;

    for (int h = 0; h < POP_SIZE; h++)
    {

        int visited[NUM_CLIENTS + 1] = {0}; // keep track of visited clients

        srand(time(NULL)); // initialize the random number generator with the same seed

        Client d_center = {RANGE_COORDINATES / 2, RANGE_COORDINATES / 2}; // Distribution center -> Always in the middle of the graph

        // Distribution center
        clients[0].x = RANGE_COORDINATES / 2;
        clients[0].y = RANGE_COORDINATES / 2;
        clients[0].distance = 0.0;

        //trecho de geração de clientes em posições aleatorias, tera que ser removido pela função de leitura 
        for (i = 1; i < NUM_CLIENTS + 1; i++)
        {
            double x = (double)(rand() % RANGE_COORDINATES);
            double y = (double)(rand() % RANGE_COORDINATES);

            clients[i].x = x;
            clients[i].y = y;

            // Calculating the distance between the client and the distribution center (using two points distance)
            clients[i].distance = calculateDistance(clients[i], d_center);
        }

        // Sorting the distances, from the closest to the furthest
        qsort(clients, NUM_CLIENTS + 1, sizeof(Client), compare);

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
            currentClient = 0;
            double currentStartTime = MAX_START_TIME;

            for (j = 0; j < VEHICLES_CAPACITY + 1; j++) // +1 because it need to consider the distribution center
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

                    int nextClient = findClosestClient(currentClient, clients, visited);

                    // printf("currentClient: %d | nextclient: %d \n", currentClient, nextClient); // if nextclient is -1, is gonna break

                    if (nextClient == -1)
                        break;
                    currentClient = nextClient;
                }
            }
        }

        // Initializing fitness of every individual
        population[h].id = h; // so por agora
        population[h].fitness = 0;
        population[h].fitnessDistance = 0;
        population[h].fitnessTime = 0;
        population[h].fitnessFuel = 0;
        population[h].fitnessCapacity = 0;
    }
}

// Function to distribute the population initialized in subpopulations
// Even if in that case all the individual in the initial population are equal, the correct way to divide it is selecting different individuals, not the same always
void distributeSubpopulation(Individual *population)
{

    for (int i = 0; i < POP_SIZE; i++)
    {
        int index = i / SUBPOP_SIZE;
        int index2 = i % SUBPOP_SIZE;

        // int randomIndiv = rand() % POP_SIZE;

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
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

/*
    -----------------------------------
              readBenchmark()
    -----------------------------------

    This function:
    - Will convert all the data in a .txt file (Solomon Benchmarks) to values in variables
    - In these benchmarks we need to cacth:
        - Number of vehicles;
        - Number of clients;
        - NO. of each client
        - Capacity of each vehicle;
        - X and Y coordenates;
        - Demand
    - A new client struct is required
    - We will need to edit how the capacity and num vehicles is defined. We need to, beside use static definition, we use global variables,
    so that way we can modify them in this function
*/
void readBenchmark(const char *filename, Client *clients)
{
    //Abrindo arquivo das benchMarks para leitura 
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("ERROR\n");
        return;
    }

    char buffer[100];

    //Ignorando a primeira linha que eh de titulo
    fgets(buffer, sizeof(buffer), file);    

    //Lendo as primeiras linhas de numero de veiculos e capacidade
    //fscanf(file, "VEHICLE\nNUMBER    CAPACITY\n%d         %d\n", numVehicles, capacity);

    //Ignorando as duas linhas de titulo das colunas
    fgets(buffer, sizeof(buffer), file);
    fgets(buffer, sizeof(buffer), file);

    //Lendo o numero de cliente
    // *numclients=0;
    //while (fscanf(file, "%d %d %d %d %d %d %d\n", 
    //       &customers[*numClients].id, 
    //       &customers[*numClients].x, 
    //       &customers[*numClients].y, 
    //       &customers[*numClients].demand, 
    //       &customers[*numClients].readyTime, 
    //       &customers[*numClients].dueDate, 
    //       &customers[*numClients].serviceTime) == 7) { //Espera exatamente 7 valores por linha 
    //    (*numClients)++;
    //}

    fclose(file);

    //Ignorando a primeira linha 
    fgets(buffer, sizeof(buffer), file);
}