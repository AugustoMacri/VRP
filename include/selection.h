#include "main.h"

void rouletteSelection(Individual *parent, int *populationFitness, Individual *population);
void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population);