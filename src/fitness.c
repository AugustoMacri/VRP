#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "fitness.h"
#include "initialization.h"

/*
    -----------------------------------
              fitness()
    -----------------------------------

    This function:
    - Calculates the fitness of every individual of the subPop;
    - fitness = distance + time + other costs;
    - Maybe we can use different types of fuel, so using 3 diferent types, we can calculate if for some vehicle route the fuel what fuel type is the least expensive;
    - That way we can calculate the total cost of the fuel per distance, different types of fuel will have different costs and different distance range;
    - timeStorage will be used to store the time each vehicle will take to complete the route;
    - After that we will gonna use each of tima and calculate separately what fuel will gona be better for eatch vehicle;
    - And then, with that we will add each of those values to the fitness of the individual;
    - Each individual wll hava a fitness that corresponds to the total cost of the fuel(best type) time and distance;

*/

void fitness(Individual *subPop, int index)
{
    int j, k, l;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];
    double fuelStorage[NUM_FUEL_TYPES];

    // First of all, we need to recalculate the distance of each route

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            distance_clients[index].route[j][k] = 0; // zera a matriz antiga (evita que ocorra bug de depois da mutacao, ter um cliente na última posição do cromossomo e calcular a distância a mais dele)
        }
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS; k++)
        {
            int clienteAtual = subPop[index].route[j][k];
            int proximoCliente = subPop[index].route[j][k + 1];

            if (clienteAtual == 0 && k > 0)
            {
                break;
            }

            Client currentCClient = clients[clienteAtual];
            Client nextCClient = clients[proximoCliente];

            double dist = calculateDistance(currentCClient, nextCClient);

            distance_clients[index].route[j][k] = dist;
        }
    }

    /*
        -Here we recalculate the time window of each client again;
        -This is because we have changed the visit order of each client
        -That way, we can minimize the number of violations of each individual
    */

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        double currentStartTime = MAX_START_TIME;

        for (j = 0; j < VEHICLES_CAPACITY + 1; j++)
        {
            int currentClient = subPop[index].route[i][j];

            if (currentClient < NUM_CLIENTS + 1)
            {
                clients[currentClient].start_time = currentStartTime;
                clients[currentClient].end_time = fmin(currentStartTime + WINDOW_SIZE, 20.0);
                currentStartTime = clients[currentClient].end_time;

                time_clients_end[index].route[i][j] = clients[currentClient].end_time;
            }
        }
    }

    int numViolations = 0;
    double totalCost = 0;
    int totalFitness = 0;
    double totalDistance = 0;
    double totalTime = 0;
    double totalFuel = 0;

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            timeStorage[j][k] = 0;
        }
    }

    for (j = 0; j < NUM_FUEL_TYPES; j++)
    {
        fuelStorage[j] = 0;
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        double soma_tempo = 0;
        double soma_distance = 0;
        double current_time = MAX_START_TIME;

        // Loop through every client in the individual and calculate the time for each client
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            double travel_time = (distance_clients[index].route[j][k] / VEHICLES_SPEED);

            current_time += travel_time;

            timeStorage[j][k] = travel_time;

            // Check for time window violation
            if (current_time > time_clients_end[index].route[j][k] && time_clients_end[index].route[j][k] != 0 && subPop[index].route[j][k] != 0)
            {
                numViolations++;
            }

            soma_distance += distance_clients[index].route[j][k]; // sum of distance of each vehicle
            totalDistance += distance_clients[index].route[j][k]; // sum of distance of all vehicles
            soma_tempo += travel_time;                            // sum of time of each vehicle
            totalTime += timeStorage[j][k];                       // sum of time of all vehicles
        }

        // Here we gona calculate de distance per fuel, for each fuel type we need to calculate the fuel per distance
        const char *nameFuel[NUM_FUEL_TYPES] = {"Gasolina", "Etanol", "Diesel"};
        int aux = 0;

        for (l = 0; l < NUM_FUEL_TYPES; l++)
        {
            // Here we gona calculate the distance for each fuel type
            if (l == 0)
            {
                // gasolin
                fuelStorage[l] = round((soma_distance / G_FUEL_CONSUMPTION)) * G_FUEL_PRICE;
            }
            else if (l == 1)
            {
                // ethanol
                fuelStorage[l] = round((soma_distance / E_FUEL_CONSUMPTION)) * E_FUEL_PRICE;
            }
            else if (l == 2)
            {
                // diesel
                fuelStorage[l] = round((soma_distance / D_FUEL_CONSUMPTION)) * D_FUEL_PRICE;
            }
        }

        double best_fuel = fuelStorage[0];

        // Here we gona calculate the best fuel type
        for (l = 0; l < NUM_FUEL_TYPES; l++)
        {
            if (fuelStorage[l] < best_fuel)
            {
                best_fuel = fuelStorage[l];
                aux = l;
            }
        }

        totalFuel += best_fuel;
    }

    /*
    Here we gona calculate the fitness of the individual
    - To calculate the fitness, we will consider the distance, time, cost of the fuel and number of time violations;
    - The capacity of esch vehicle normally is consider a violation when it is over the capacity, but in this code, we generate the initial subPop already with the capacity of each vehicle
    so that we can calculate wich individual will be in wich vehicle.
    - With that, is not necessary to calculate the vehicle capacity again.
    */

    totalCost = (totalDistance * 1.0) + (totalTime * 0.5) + (totalFuel * 0.75);
    totalFitness = (NUM_VEHICLES * WEIGHT_NUM_VEHICLES) + (numViolations * WEIGHT_NUM_VIOLATIONS) + totalCost;

    // subPopFitness[i] = totalFitness;
    subPop[index].fitness = totalFitness;

    /*
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", subPop[index].route[i][j]);
        }
        printf("\n");
    }
    printf("Fitness do individuo %d : %d com o id de %d\n", index, subPop[index].fitness, subPop[index].id);
    */
}

/*
    -----------------------------------
            fitnessDistance()
    -----------------------------------

    This function:
    - Calculates the fitness of every individual of the subPop;
    - The objective of this function is minimize the distance;

*/

void fitnessDistance(Individual *subPop, int index)
{
    int j, k, l, m;

    // printf("Fitness do primeiro individuo [0]: %d\n", subPop[0].fitnessDistance);

    for (int i = 0; i < POP_SIZE; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                distance_clients[i].route[j][k] = 0; // A matriz tem POP_size tamanho, mas aqui só iremos precisar do POP_size / 3
            }
        }
    }

    // First of all, we need to recalculate the distance of each route

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            distance_clients[index].route[j][k] = 0; // A matriz tem POP_size tamanho, mas aqui só iremos precisar do POP_size / 3
        }
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS; k++)
        {
            int valCurrentClient = subPop[index].route[j][k];
            int valNextClient = subPop[index].route[j][k + 1];

            if (valCurrentClient == 0 && k > 0)
            {
                break;
            }

            Client currentClient = clients[valCurrentClient];
            Client nextClient = clients[valNextClient];

            double dist = calculateDistance(currentClient, nextClient);

            distance_clients[index].route[j][k] = dist;
        }
    }

    // And now we need to put the value of the distance times the wheight as a distanceFitness for the individuals

    double totalDistance = 0;
    int totalDistanceFitness = 0;

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            totalDistance += distance_clients[index].route[j][k]; // sum of distance of all vehicles
        }
    }

    totalDistanceFitness = (totalDistance * 1);
    subPop[index].fitnessDistance = totalDistanceFitness;

    /*
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", subPop[index].route[i][j]);
        }
        printf("\n");
    }
    printf("Fitness do individuo %d : %d com o id de %d\n", index, subPop[index].fitnessDistance,  subPop[index].id);
    */
}

/*
    -----------------------------------
            fitnessTime()
    -----------------------------------

    This function:
    - Calculates the fitness of every individual of the subPop;
    - The objective of this function is minimize the time and the violations that every route in the subPop have;

*/

void fitnessTime(Individual *subPop, int index)
{
    // We need to recalculte de distance again, so that way is possible to recalculate the time window of each client
    int j, k, l;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];

    for (int i = 0; i < POP_SIZE; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                distance_clients[i].route[j][k] = 0;
            }
        }
    }

    // First of all, we need to recalculate the distance of each route

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            distance_clients[index].route[j][k] = 0;
        }
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS; k++)
        {
            int valCurrentClient = subPop[index].route[j][k];
            int valNextClient = subPop[index].route[j][k + 1];

            if (valCurrentClient == 0 && k > 0)
            {
                break;
            }

            Client currentClient = clients[valCurrentClient];
            Client nextClient = clients[valNextClient];

            double dist = calculateDistance(currentClient, nextClient);

            distance_clients[index].route[j][k] = dist;
        }
    }

    /*
        -Here we recalculate the time window of each client again;
        -This is because we have changed the visit order of each client
        -That way, we can minimize the number of violations of each individual
    */

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        double currentStartTime = MAX_START_TIME;

        for (j = 0; j < VEHICLES_CAPACITY + 1; j++)
        {
            int currentClient = subPop[index].route[i][j];

            if (currentClient < NUM_CLIENTS + 1)
            {
                clients[currentClient].start_time = currentStartTime;
                clients[currentClient].end_time = fmin(currentStartTime + WINDOW_SIZE, 20.0);
                currentStartTime = clients[currentClient].end_time;

                time_clients_end[index].route[i][j] = clients[currentClient].end_time;
            }
        }
    }

    int numViolations = 0;
    int totalTimeFitness = 0;
    double totalDistance = 0;
    double totalTime = 0;

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            timeStorage[j][k] = 0;
        }
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        double soma_tempo = 0;
        double soma_distance = 0;
        double current_time = MAX_START_TIME;

        // Loop through every client in the individual and calculate the time for each client
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            double travel_time = (distance_clients[index].route[j][k] / VEHICLES_SPEED);

            current_time += travel_time;

            timeStorage[j][k] = travel_time;

            // Check for time window violation
            if (current_time > time_clients_end[index].route[j][k] && time_clients_end[index].route[j][k] != 0 && subPop[index].route[j][k] != 0)
            {
                numViolations++;
            }

            soma_distance += distance_clients[index].route[j][k]; // sum of distance of each vehicle
            totalDistance += distance_clients[index].route[j][k]; // sum of distance of all vehicles
            soma_tempo += travel_time;                            // sum of time of each vehicle
            totalTime += timeStorage[j][k];                       // sum of time of all vehicles
        }
    }

    totalTimeFitness = (numViolations * WEIGHT_NUM_VIOLATIONS) + (totalTime * 0.5);
    subPop[index].fitnessTime = totalTimeFitness;

    // printf("O numero de violacoes eh: %d\n", numViolations);
    // printf("O tempo total que os veiculos demoram para percorrer eh: %f\n", totalTime);
    // printf("totalTimeFitness do individuo %d é %d\n", index, subPop[index].fitnessTime);

    /*
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", subPop[index].route[i][j]);
        }
        printf("\n");
    }
    printf("Fitness do individuo %d : %d com id de %d\n", index, subPop[index].fitnessTime, subPop[index].id);
    */
}

/*
    -----------------------------------
            fitnessFuel()
    -----------------------------------

    This function:
    - Calculates the fitness of every individual of the subPop;
    - The objective of this function is minimize the fuel consumption if every route in the subPop have;

*/

void fitnessFuel(Individual *subPop, int index)
{

    // We need to recalculte de distance again, so that way is possible to recalculate the time window of each client
    int j, k, l;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];
    double fuelStorage[NUM_FUEL_TYPES];

    for (int i = 0; i < POP_SIZE; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                distance_clients[i].route[j][k] = 0;
            }
        }
    }

    // First of all, we need to recalculate the distance of each route

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            distance_clients[index].route[j][k] = 0;
        }
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        for (k = 0; k < NUM_CLIENTS; k++)
        {
            int valCurrentClient = subPop[index].route[j][k];
            int valNextClient = subPop[index].route[j][k + 1];

            if (valCurrentClient == 0 && k > 0)
            {
                break;
            }

            Client currentClient = clients[valCurrentClient];
            Client nextClient = clients[valNextClient];

            double dist = calculateDistance(currentClient, nextClient);

            distance_clients[index].route[j][k] = dist;
        }
    }

    double totalDistance = 0;
    double totalFuel = 0;
    int totalFuelFitness = 0;

    for (j = 0; j < NUM_FUEL_TYPES; j++)
    {
        fuelStorage[j] = 0;
    }

    for (j = 0; j < NUM_VEHICLES; j++)
    {
        double soma_distance = 0;

        // Loop through every client in the individual and calculate the time for each client
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            soma_distance += distance_clients[index].route[j][k]; // sum of distance of each vehicle
            totalDistance += distance_clients[index].route[j][k]; // sum of distance of all vehicles
        }

        // Here we gona calculate de distance per fuel, for each fuel type we need to calculate the fuel per distance
        const char *nameFuel[NUM_FUEL_TYPES] = {"Gasolina", "Etanol", "Diesel"};
        int aux = 0;

        for (l = 0; l < NUM_FUEL_TYPES; l++)
        {
            // Here we gona calculate the distance for each fuel type
            if (l == 0)
            {
                // gasolin
                fuelStorage[l] = round((soma_distance / G_FUEL_CONSUMPTION)) * G_FUEL_PRICE;
            }
            else if (l == 1)
            {
                // ethanol
                fuelStorage[l] = round((soma_distance / E_FUEL_CONSUMPTION)) * E_FUEL_PRICE;
            }
            else if (l == 2)
            {
                // diesel
                fuelStorage[l] = round((soma_distance / D_FUEL_CONSUMPTION)) * D_FUEL_PRICE;
            }
        }

        double best_fuel = fuelStorage[0];

        // Here we gona calculate the best fuel type
        for (l = 0; l < NUM_FUEL_TYPES; l++)
        {
            if (fuelStorage[l] < best_fuel)
            {
                best_fuel = fuelStorage[l];
                aux = l;
            }
        }

        totalFuel += best_fuel;
    }

    totalFuelFitness = totalFuel * 0.75;
    subPop[index].fitnessFuel = totalFuelFitness;

    /*
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        for (int j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", subPop[index].route[i][j]);
        }
        printf("\n");
    }
    printf("Fitness do individuo %d : %d e o id eh %d\n", index, subPop[index].fitnessFuel, subPop[index].id);
    */
}

/*
    -----------------------------------
            fitnessCapacity()
    -----------------------------------

    This function:
    - Calculates the number of clients that the vehicle will visit;
    - Then is possible to us dethermine if is beeing well or bad used the capacity;

*/

void fitnessCapacity(Individual *subPopCapacity, int index)
{

    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        int totalCapacityUsed = 0;
        for (int i = 0; i < NUM_VEHICLES; i++)
        {
            int capacityUsed = 0;
            for (int j = 0; j < NUM_CLIENTS + 1; j++)
            {
                // printf("%d ", subPopCapacity[k].route[i][j]);
                if (subPopCapacity[k].route[i][j] != 0 && subPopCapacity[k].route[i][j] != -1)
                {
                    capacityUsed++;
                }
            }
            printf("\n");
            printf("Capacity Used on vehicle %d: %d\n", i + 1, capacityUsed);
            totalCapacityUsed += capacityUsed;
        }
        printf("Total Capacity Used (both of he vehicles) on individual %d : %d\n", k + 1, totalCapacityUsed);

        subPopCapacity[k].fitnessCapacity = totalCapacityUsed * WEIGHT_CAPACITY;
    }
}