#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "population.h"
#include "initialization.h"
#include "fitness.h"
#include "selection.h"
#include "crossing.h"
#include "mutation.h"
#include "print.h"

/*
    -----------------------------------
              updatePop()
    -----------------------------------

    This function:
    - In this function we will update the population;
    - so the population[i] will be updated to nextpop[i];
*/
void updatePop(Individual *subPop, Individual *nextSubPop)
{
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        subPop[i] = nextSubPop[i];
    }

    // Resetting all individuals from nextPop
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        nextSubPop[i].id = -1;
    }
}

/*
    -----------------------------------
              isIdInNextPop()
    -----------------------------------

    This function:
    - the only purpose of this function is to verify if the selected individual is already present on the nextPop
*/
int isIdInNextPop(int index, Individual *nextPop)
{

    printf("ENTROU NA FUNCAO com id %d\n", index);

    for (int i = 0; i < ELITISM_SIZE_POP; i++)
    {
        printf("id %d\n", nextPop[i].id);
    }

    for (int i = 0; i < ELITISM_SIZE_POP; i++)
    {
        if (nextPop[i].id == index)
        {
            printf("O individuo com id %d esta presente no nextPop\n", index);
            return 1;
        }
    }

    return 0;
}

/*
    -----------------------------------
            compareSonSubPop()
    -----------------------------------

    This function:
    - In this function we will compare the son that is generated in the crossing with every subpopulation;
    - If the son had a fitness smaller than at least one individual of that subpopulation, the son is put on the next generation of that one subpopulation;
*/
void compareSonSubPop(Individual *newSon, Individual *subPop, Individual *nextPop, int *previousHighestFitnessID, int index)
{

    int replaced = 0;
    int highestFitnessIndex = -1;
    int individualVerified = 0;

    // First we have to pick the individual of the subpopulation selected with the lowest fitness
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        switch (index)
        {
        case 0:
            if (subPop[i].fitnessDistance > subPop[highestFitnessIndex].fitnessDistance || highestFitnessIndex == -1)
            {
                highestFitnessIndex = i;
            }
            break;

        case 1:
            if (subPop[i].fitnessTime > subPop[highestFitnessIndex].fitnessTime || highestFitnessIndex == -1)
            {
                highestFitnessIndex = i;
            }
            break;

        case 2:
            if (subPop[i].fitnessFuel > subPop[highestFitnessIndex].fitnessFuel || highestFitnessIndex == -1)
            {
                highestFitnessIndex = i;
            }
            break;

        case 3:
            if (subPop[i].fitness > subPop[highestFitnessIndex].fitness || highestFitnessIndex == -1)
            {
                highestFitnessIndex = i;
            }
            break;

        default:
            return;
        }
    }

    // Then we will replace the individual that have the higher fitness
    switch (index)
    {
    case 0:

        if (newSon[0].fitnessDistance < subPop[highestFitnessIndex].fitnessDistance)
        {
            printf("\nIntroduzindo Filho na NEXTSUBPOPDISTANCE\n");
            // printf("Fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessDistance);
            *previousHighestFitnessID = subPop[highestFitnessIndex].id;
            // printf("Id do individuo (que não aparecerá na nextPOP distance): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[highestFitnessIndex].id = newSon[0].id;
            nextPop[highestFitnessIndex].fitnessDistance = newSon[0].fitnessDistance;

            // printf("Id do individuo na nextPop: %d\n", nextPop[highestFitnessIndex].id);
            // printf("Fitness do individuo na nextPop: %d\n", nextPop[highestFitnessIndex].fitnessDistance);

            replaced = 1;
        }

        break;

    case 1:

        if (newSon[0].fitnessTime < subPop[highestFitnessIndex].fitnessTime)
        {
            printf("\nIntroduzindo Filho na NEXTSUBPOPTIME\n");
            // printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessTime);
            *previousHighestFitnessID = subPop[highestFitnessIndex].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP TIME): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[highestFitnessIndex].id = newSon[0].id;
            nextPop[highestFitnessIndex].fitnessTime = newSon[0].fitnessTime;

            // printf("Id do atual: %d\n", nextPop[highestFitnessIndex].id);
            // printf("Fitness do atual: %d\n", nextPop[highestFitnessIndex].fitnessTime);

            replaced = 1;
        }

        break;

    case 2:

        if (newSon[0].fitnessFuel < subPop[highestFitnessIndex].fitnessFuel)
        {
            printf("\nIntroduzindo Filho na NEXTSUBPOPFuel\n");
            // printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessFuel);
            *previousHighestFitnessID = subPop[highestFitnessIndex].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP combustivel): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[highestFitnessIndex].id = newSon[0].id;
            nextPop[highestFitnessIndex].fitnessFuel = newSon[0].fitnessFuel;

            // printf("Id do atual: %d\n", nextPop[highestFitnessIndex].id);
            // printf("Fitness do atual: %d\n", nextPop[highestFitnessIndex].fitnessFuel);

            replaced = 1;
        }

        break;

    case 3:

        if (newSon[0].fitness < subPop[highestFitnessIndex].fitness)
        {
            printf("\nIntroduzindo Filho na NEXTSubpopPonderacao\n");
            // printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitness);
            *previousHighestFitnessID = subPop[highestFitnessIndex].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP Pond): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[highestFitnessIndex].id = newSon[0].id;
            nextPop[highestFitnessIndex].fitness = newSon[0].fitness;

            // printf("Id do atual: %d\n", nextPop[highestFitnessIndex].id);
            // printf("Fitness do atual: %d\n", nextPop[highestFitnessIndex].fitness);

            replaced = 1;
        }

        break;

    default:
        break;
    }
}

/*
    -----------------------------------
            completeNextPop()
    -----------------------------------

    This function:
    - In this function we will complete the nextPop with all the individual remaining in the currer subpopulations
*/

void completeNextPop(Individual *subPop, Individual *nextPop, int *previousHighestFitnessID)
{
    // Primeiro precisamos pegar o indiviudo da subpop
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {

        int alreadyInNextPop = 0;

        // Depois precisamos comparar o individuo da subpop com cada individuo presente na nextPop
        for (int j = 0; j < SUBPOP_SIZE; j++)
        {

            if (subPop[i].id == nextPop[j].id)
            {
                alreadyInNextPop = 1;
                break;
            }
        }

        // Se o individuo ainda nao esta presente e eh diferente do previous
        if (!alreadyInNextPop && subPop[i].id != *previousHighestFitnessID)
        {
            for (int k = 0; k < SUBPOP_SIZE; k++)
            {
                if (nextPop[k].id == -1)
                {
                    nextPop[k] = subPop[i];
                    break;
                }
            }
        }
    }
}

/*
    -----------------------------------
              evolvePop()
    -----------------------------------

    This function:
    - The population will be evolved until the limit of generations is reached or the optimal solution is found;
*/

void evolvePop(int rodada, int *populationFitness, Individual *population, Individual *newSon, int *tournamentFitness, Individual *tournamentIndividuals, Individual *subpop1, Individual *subpop2, int startIndex, int *idTrack, int *previousHighestFitnessDistanceID, int *previousHighestFitnessTimeID,
               int *previousHighestFitnessFuelID, int *previousHighestFitnessWeightingID)
{
    int *index = &startIndex;

    // printf("=========================SUBPOP DISTANCIA=========================\n");
    // printSubPop(subPopDistance);

    // printf("=========================NEXTSUBPOP DISTANCIA=========================\n");
    // printSubPop(nextSubPopDistance);

    // Selection of the Elite of each subpopulation and put in nextPop
    selectElite(subPopDistance, nextSubPopDistance, 0);
    selectElite(subPopTime, nextSubPopTime, 1);
    selectElite(subPopFuel, nextSubPopFuel, 2);
    selectElite(subPopWeighting, nextSubPopWeighting, 3);

    // printf("=========================SUBPOP DISTANCIA depois de ELITE=========================\n");
    // printSubPop(subPopDistance);

    // printf("=========================NEXTSUBPOP DISTANCIA depois de ELITE=========================\n");
    // printSubPop(nextSubPopDistance);

    // Selection between the subpopulations and tournament
    subPopSelection(tournamentIndividuals, parent, tournamentFitness, subpop1, subpop2);

    // Crossing between the parents
    switch (CROSSINGTYPE)
    {
    case 1:
        onePointCrossing(index, parent, newSon, idTrack);
        break;

    case 2:
        twoPointCrossing(index, parent, newSon);
        break;
    }

    // Mutating the son of the crossing
    mutation(newSon);

    // Calculating the fitness of the son
    fitnessDistance(newSon, 0);
    fitnessTime(newSon, 0);
    fitnessFuel(newSon, 0);
    fitness(newSon, 0);

    // printSubPop(newSon, 1);

    // printf("previousHighestFitnessDistanceID ANTES %d\n", *previousHighestFitnessDistanceID);

    // Comparing the son created with each the worst individual of the population
    compareSonSubPop(newSon, subPopDistance, nextSubPopDistance, previousHighestFitnessDistanceID, 0);
    compareSonSubPop(newSon, subPopTime, nextSubPopTime, previousHighestFitnessTimeID, 1);
    compareSonSubPop(newSon, subPopFuel, nextSubPopFuel, previousHighestFitnessFuelID, 2);
    compareSonSubPop(newSon, subPopWeighting, nextSubPopWeighting, previousHighestFitnessWeightingID, 3);

    // printf("=========================NEXTSUBPOP DISTANCIA depois de COMPARE=========================\n");
    // printSubPop(nextSubPopDistance);

    // printf("previousHighestFitnessDistanceID DEPOIS %d\n", *previousHighestFitnessDistanceID);

    // Adding the son in the next population (only if it has a better fitness)
    completeNextPop(subPopDistance, nextSubPopDistance, previousHighestFitnessDistanceID);
    completeNextPop(subPopTime, nextSubPopTime, previousHighestFitnessTimeID);
    completeNextPop(subPopFuel, nextSubPopFuel, previousHighestFitnessFuelID);
    completeNextPop(subPopWeighting, nextSubPopWeighting, previousHighestFitnessWeightingID);

    // printf("=========================NEXTSUBPOP DISTANCIA depois de COMPlETE=========================\n");
    // printSubPop(nextSubPopDistance);

    // And now we will update the subPops with the individual of the nextSubPop
    updatePop(subPopDistance, nextSubPopDistance);
    updatePop(subPopTime, nextSubPopTime);
    updatePop(subPopFuel, nextSubPopFuel);
    updatePop(subPopWeighting, nextSubPopWeighting);

    // printf("=========================SUBPOP DISTANCIA depois de COMPlETE=========================\n");
    // printSubPop(subPopDistance);
}

void printSubPop(Individual *subPop, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        printf("Id: %d\n", subPop[i].id);
        printf("fitness Ponderado: %d\n", subPop[i].fitness);

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", subPop[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
}