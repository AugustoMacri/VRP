#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
void evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, Individual *subpop1, Individual *subpop2, int startIndex);
void compareSonSubPop(Individual *nextPop, Individual *subPop, int index);