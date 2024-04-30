#include "main.h"

void rouletteSelection(Individual *parent, int *populationFitness, Individual *population);
void tournamentSelection(int **tournamentIndividuals, int **parent, int *tournamentFitness, int *populationFitness, int **populacaoAtual);