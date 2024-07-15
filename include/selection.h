#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
void rouletteSelection(Individual *parent, int *populationFitness, Individual *population);
void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population);
void elitism(int *index, Individual *nextPop, Individual *population);
void subPopSelection();
void printTudo(Individual *subpop1);