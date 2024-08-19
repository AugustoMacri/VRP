#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
void evolvePop(int rodada, int *populationFitness, Individual *population, Individual *nextPop, int *tournamentFitness, Individual *tournamentIndividuals, Individual *subpop1, Individual *subpop2, int startIndex, int *idTrack, int *previousHighestFitnessDistanceID, int *previousHighestFitnessTimeID,
               int *previousHighestFitnessFuelID, int *previousHighestFitnessWeightingID);
int compareSonSubPop(Individual *newSon, Individual *subPop, Individual *nextPop, int *previousHighestFitnessID, int index, int individual);
void completeNextPop(Individual *subPop, Individual *nextPop, int *previousHighestFitnessID);
