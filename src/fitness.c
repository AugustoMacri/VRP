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

    printf("\n");

    // Loop through every individual in the population
    for (i = 0; i < 1; i++) // after testing we will use NUM_INDIVIDUALS
    {
        // Loop through every vehicle in the individual
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            double soma_tempo = 0;

            printf("Tempo para o veiculo %d concluir a rota: ", j + 1);

            // Loop through every client in the individual
            for (k = 0; k < NUM_CLIENTS + 1; k++)
            {
                // Here we gona calculate the time for each client, so then we can calculate the time each vehicle will take to complete the route
                timeStorage[j][k] = (distance_clients[j][k] / VEHICLES_SPEED);

                soma_tempo += timeStorage[j][k];
            }
            printf("%.2f ", soma_tempo);

            printf("\n");

            // Here we gona calculate de distance per fuel, for each fuel type we need to calculate the fuel per distance
            // f(x) = (x/autonomy) * fuel_type_price
        }
    }
}
