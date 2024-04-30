#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "fitness.h"

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

void fitness()
{
    int i, j, k, l;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];
    double fuelStorage[NUM_FUEL_TYPES];

    for (i = 0; i < NUM_FUEL_TYPES; i++)
    {
        fuelStorage[i] = 0;
    }

    printf("\n");

    // Loop through every individual in the population
    for (i = 0; i < POP_SIZE; i++)
    {
        int numViolations = 0;
        double totalCost = 0;
        int totalFitness = 0;
        double totalDistance = 0;
        double totalTime = 0;
        double totalFuel = 0;

        // Loop through every vehicle in the individual
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            double soma_tempo = 0;
            double soma_distance = 0;
            double current_time = MAX_START_TIME;

            // Loop through every client in the individual and calculate the time for each client, so then we can calculate the time each vehicle will take to complete the route
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                // Here we gona calculate the time for each client, so then we can calculate the time each vehicle will take to complete the route
                timeStorage[j][k] = (distance_clients[j][k] / VEHICLES_SPEED);

                // printf("%f\n", distance_clients[j][k]); Está devolvendo as distâncias corretamente

                if (current_time + timeStorage[j][k] > time_clients_end[j][k] && time_clients_end[j][k] != 0)
                {                                           // it has to be different from 0 because wen is 0 is a client that does not gonna be visited by that vehicle
                    //printf("%f\n", time_clients_end[j][k]); // saber qual horário esta criando uma violacao
                    numViolations++;
                }

                current_time += timeStorage[j][k];

                soma_distance += distance_clients[j][k]; // sum of distance of each vehicle
                totalDistance += distance_clients[j][k]; // sum of distance of all vehicles
                soma_tempo += timeStorage[j][k];         // sum of time of each vehicle
                totalTime += timeStorage[j][k];          // sum of time of all vehicles
            }
            printf("Tempo para o veiculo %d concluir a rota de distancia %.2f: %.2f\n", j + 1, soma_distance, soma_tempo);

            // Here we gona calculate de distance per fuel, for each fuel type we need to calculate the fuel per distance
            const char *nameFuel[NUM_FUEL_TYPES] = {"Gasolina", "Etanol", "Diesel"};
            int aux = 0;
            // Loop through every fuel type
            for (l = 0; l < NUM_FUEL_TYPES; l++)
            {
                // Here we gona calculate the distance for each fuel type
                if (l == 0)
                {
                    // Here we gona calculate the gasolin per distance
                    fuelStorage[l] = round((soma_distance / G_FUEL_CONSUMPTION)) * G_FUEL_PRICE;
                }
                else if (l == 1)
                {
                    // Here we gona calculate the ethanol per distance
                    fuelStorage[l] = round((soma_distance / E_FUEL_CONSUMPTION)) * E_FUEL_PRICE;
                }
                else if (l == 2)
                {
                    // Here we gona calculate the diesel per distance
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

            printf("Melhor combustivel para veiculo %d: %s com gasto de %.2f\n", j + 1, nameFuel[aux], best_fuel);

            /*
            for (l = 0; l < NUM_FUEL_TYPES; l++)
            {
                printf("%s: %.2f\n", nameFuel[l], fuelStorage[l]);
            }
            */
            
            printf("\n");
        }

        printf("Violations: %d\n\n", numViolations);

        /*
        Here we gona calculate the fitness of the individual
        - To calculate the fitness, we will consider the distance, time, cost of the fuel and number of time violations;
        - The capacity of esch vehicle normally is consider a violation when it is over the capacity, but in this code, we generate the initial population already with the capacity of each vehicle
        so that we can calculate wich individual will be in wich vehicle.
        - With that, is not necessary to calculate the vehicle capacity again.
        */

        //printf("%.2f\n", totalDistance);
        //printf("%.2f\n", totalTime);
        //printf("%.2f\n", totalFuel);

        totalCost = totalDistance + totalTime + totalFuel;
        totalFitness = (NUM_VEHICLES * WEIGHT_NUM_VEHICLES) + (numViolations * WEIGHT_NUM_VIOLATIONS) + totalCost;

        populationFitness[i] = totalFitness;
        population[i].fitness = totalFitness;
        printf("Fitness do individuo (ponteiro) %d: %d\n", i + 1, populationFitness[i]);
        printf("teste com fitness do individuo %d na struct: %d\n", i + 1, population[i].fitness);

        
    }
}
