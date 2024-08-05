#include "main.h"

void updatePop(Individual *population, Individual *nextPop);
void rouletteSelection(Individual *parent, int *populationFitness, Individual *population);
void tournamentSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, int *populationFitness, Individual *population);
void tournamentSelectionEquals(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop, int index, int *previousWinner, int fitnessType);
void elitism(int *index, Individual *nextPop, Individual *population);
void subPopSelection(Individual *tournamentIndividuals, Individual *parent, int *tournamentFitness, Individual *subpop1, Individual *subpop2);
void selectElite(Individual *subPop, Individual *subPopElite, int index);
