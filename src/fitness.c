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
    - Calculates the fitness of every individual of the population;
    - fitness = distance + time + other costs;
    - Maybe we can use different types of fuel, so using 3 diferent types, we can calculate if for some vehicle route the fuel what fuel type is the least expensive;
    - That way we can calculate the total cost of the fuel per distance, different types of fuel will have different costs and different distance range;
    - timeStorage will be used to store the time each vehicle will take to complete the route;
    - After that we will gonna use each of tima and calculate separately what fuel will gona be better for eatch vehicle;
    - And then, with that we will add each of those values to the fitness of the individual;
    - Each individual wll hava a fitness that corresponds to the total cost of the fuel(best type) time and distance;

*/

int fitness(Individual *population, int *populationFitness, int solutionFound)
{
    printf("\n------------------------------------------------\n");
    printf("\tTESTANDO Fitness\n");
    printf("------------------------------------------------\n");

    int i, j, k, l;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];
    double fuelStorage[NUM_FUEL_TYPES];

    printf("Primeiramente iremos printar os individuos da populacao atual\n");
    printf("Na teoria eh para terem modificado após a mutacao\n");
    for (i = 0; i < POP_SIZE; i++)
    {
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d", population[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    printf("\n");

    /*
        printf("-----------------------Conferindo os valores da distancia antigos-----------------------\n");
        for (i = 0; i < POP_SIZE; i++)
        {
            for (j = 0; j < NUM_VEHICLES; j++)
            {
                for (k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    printf("%f ", distance_clients[i].route[j][k]);
                }
                printf("\n");
            }
            printf("\n");
        }

        printf("\n");
    */

    // Recalculando as novas distâncias de acordo com a nova rota do veículo
    printf("-----------------------VALORES RECALCULADOS DA DISTÂNCIA-----------------------\n");
    for (i = 0; i < POP_SIZE; i++)
    {
        printf("=========================Indivíduo %d=========================\n", i + 1);
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                distance_clients[i].route[j][k] = 0; // zera a matriz antiga (evita que ocorra bug de depois da mutacao, ter um cliente na última posição do cromossomo e calcular a distância a mais dele)
            }
        }

        for (j = 0; j < NUM_VEHICLES; j++)
        {
            for (k = 0; k < NUM_CLIENTS; k++)
            {
                int clienteAtual = population[i].route[j][k];
                int proximoCliente = population[i].route[j][k + 1];

                if (clienteAtual == 0 && k > 0)
                {
                    break;
                }

                Client currentCClient = clients[clienteAtual];
                Client nextCClient = clients[proximoCliente];

                double dist = calculateDistance(currentCClient, nextCClient);
                printf("Distância recalculada entre cliente %d e %d: %f\n", clienteAtual, proximoCliente, dist);

                distance_clients[i].route[j][k] = dist;
            }
        }
    }

    printf("\n");
    /*
        -Here we recalculate the time window of each client again;
        -This is because we have changed the visit order of each client
        -That way, we can minimize the number of violations of each individual
    */
    printf("----------------------VALORES RECALCULADOS DO TEMPO-----------------------\n");
    for (int h = 0; h < POP_SIZE; h++)
    {
        printf("=========================Indivíduo %d=========================\n", h + 1);

        for (i = 0; i < NUM_VEHICLES; i++)
        {
            printf("Vehicle %d: \n", i + 1);

            double currentStartTime = MAX_START_TIME;

            for (j = 0; j < VEHICLES_CAPACITY + 1; j++)
            {
                int currentClient = population[h].route[i][j];

                if (currentClient < NUM_CLIENTS + 1)
                {
                    clients[currentClient].start_time = currentStartTime;
                    clients[currentClient].end_time = fmin(currentStartTime + WINDOW_SIZE, 20.0);
                    currentStartTime = clients[currentClient].end_time;

                    time_clients_end[h].route[i][j] = clients[currentClient].end_time;

                    printf("client %d end time: %.2f ", currentClient, clients[currentClient].end_time);
                    // printf(" !ATENCAO!: client %d end time: %.2f ", population[h].route[i][j], time_clients_end[h].route[i][j]);
                }
                printf("\n");
            }
        }
    }

    for (i = 0; i < POP_SIZE; i++)
    {
        printf("========================= Individuo %d ==========================\n", i + 1);
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
                double travel_time = (distance_clients[i].route[j][k] / VEHICLES_SPEED);

                current_time += travel_time;

                timeStorage[j][k] = travel_time;

                // Check for time window violation
                if (current_time > time_clients_end[i].route[j][k] && time_clients_end[i].route[j][k] != 0 && population[i].route[j][k] != 0)
                {
                    printf("No cliente %d o veiculo %d violou as %.2f horas porque passou as %.2f horas\n", population[i].route[j][k], j + 1, time_clients_end[i].route[j][k], current_time);
                    numViolations++;
                }

                soma_distance += distance_clients[i].route[j][k]; // sum of distance of each vehicle
                totalDistance += distance_clients[i].route[j][k]; // sum of distance of all vehicles
                soma_tempo += travel_time;                        // sum of time of each vehicle
                totalTime += timeStorage[j][k];                   // sum of time of all vehicles
            }
            printf("Tempo para o veiculo %d concluir a rota de distancia %.2f: %.2f\n", j + 1, soma_distance, soma_tempo);

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

            printf("The best fuel for the vehicle %d: %s spending %.2f\n", j + 1, nameFuel[aux], best_fuel);

            printf("\n");
        }

        printf("Violations: %d\n", numViolations);

        /*
        Here we gona calculate the fitness of the individual
        - To calculate the fitness, we will consider the distance, time, cost of the fuel and number of time violations;
        - The capacity of esch vehicle normally is consider a violation when it is over the capacity, but in this code, we generate the initial population already with the capacity of each vehicle
        so that we can calculate wich individual will be in wich vehicle.
        - With that, is not necessary to calculate the vehicle capacity again.
        */

        totalCost = (totalDistance * 1.0) + (totalTime * 0.5) + (totalFuel * 0.75);
        totalFitness = (NUM_VEHICLES * WEIGHT_NUM_VEHICLES) + (numViolations * WEIGHT_NUM_VIOLATIONS) + totalCost;
        // printf("Pesos Ponderados: %d\n", totalFitness);

        populationFitness[i] = totalFitness;
        population[i].fitness = totalFitness;
        printf("Fitness do individuo (ponteiro) %d: %d\n", i + 1, populationFitness[i]);
        // printf("teste com fitness do individuo %d na struct: %d\n", i + 1, population[i].fitness);

        // Here is a condition to know if the solution that is ok is found
        if (populationFitness[i] < 500)
        {
            solutionFound = 1;
        }
    }

    return solutionFound;
}