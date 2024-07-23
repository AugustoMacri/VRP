#include "main.h"

int fitness(Individual *population, int *populationFitness, int solutionFound);
void fitnessDistance(Individual *subPopDistance, int index);
void fitnessTime(Individual *subPopTime, int index);
void fitnessFuel(Individual *subPopFuel, int index);
void fitnessCapacity(Individual *subPopCapacity, int index);