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
    int numViolations = 0;
    double timeStorage[NUM_VEHICLES][NUM_CLIENTS + 1];
    double fuelStorage[NUM_FUEL_TYPES];

    for (i = 0; i < NUM_FUEL_TYPES; i++)
    {
        fuelStorage[i] = 0;
    }

    printf("\n");

    // Loop through every individual in the population
    for (i = 0; i < 1; i++) // after testing we will use NUM_INDIVIDUALS
    {
        // Loop through every vehicle in the individual
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            double soma_tempo = 0;
            double soma_distance = 0;

            // Loop through every client in the individual and calculate the time for each client, so then we can calculate the time each vehicle will take to complete the route
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                // Here we gona calculate the time for each client, so then we can calculate the time each vehicle will take to complete the route
                timeStorage[j][k] = (distance_clients[j][k] / VEHICLES_SPEED);

                // printf("%f\n", distance_clients[j][k]); Está devolvendo as distâncias corretamente

                soma_distance += distance_clients[j][k];
                soma_tempo += timeStorage[j][k];
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

            printf("Melhor combustivel para veiculo %d: %s com gasto de %.2f\n", j + 1, nameFuel[aux], best_fuel);
            for (l = 0; l < NUM_FUEL_TYPES; l++)
            {
                printf("%s: %.2f\n", nameFuel[l], fuelStorage[l]);
            }
            printf("\n");
        }
    }
}
