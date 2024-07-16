#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
void rouletteSelection(Individual *parent, int *populationFitness, Individual *population);
void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population);
void tournamentSelection2(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop, int index);
void elitism(int *index, Individual *nextPop, Individual *population);
void subPopSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop1, Individual *subpop2);
