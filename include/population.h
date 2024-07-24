#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
int evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, int solutionFound);
void compareSonSubPop(Individual *nextPop, Individual *subPop, int index);