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

*/

void fitness(int **currentClientArray, int *fitnessPopulation){
    int i, j, k, l;
    int totalDistance, totalTime, totalOtherCosts, fitnessIndividual;

    //Loop through every individual in the population
    for(i = 0; i < POP_SIZE; i++){
        int totalDistance = 0;
        int totalTime = 0;
        int totalOtherCosts = 0;

        //Loop through every vehicle in the individual
        for(j = 0; j < NUM_VEHICLES; j++){
            int distance = 0;
            int lastVisitedClient = 0;
            int currentTime = 0;

            //Loop through every client in the individual
            for(k = 0; k < NUM_CLIENTS + 1; k++){

            }
        }
    }
}
