#include "main.h"

int fitness(Individual *population, int *populationFitness, int solutionFound);
void fitnessDistance(Individual *subPop, int index);
void fitnessTime(Individual *subPop, int index);
void fitnessFuel(Individual *subPop, int index);
void fitnessCapacity(Individual *subPopCapacity, int index);