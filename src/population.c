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
}

/*
    -----------------------------------
            compareSonSubPop()
    -----------------------------------

    This function:
    - In this function we will compare the son that is generated in the crossing with every subpopulation;
    - If the son had a fitness smaller than at least one individual of that subpopulation, it is replaced;
*/
void compareSonSubPop(Individual *newSon, Individual *subPop, int index)
{

    int replaced = 0;
    int highestFitnessIndex = 0;
    int individualVerified = 0;

    // First we have to pick the individual of the subpopulation selected with the lowest fitness
    for (int i = 1; i < SUBPOP_SIZE; i++)
    {
        switch (index)
        {
        case 0:
            if (subPop[i].fitnessDistance > subPop[highestFitnessIndex].fitnessDistance)
            {
                highestFitnessIndex = i;
            }
            break;

        case 1:
            if (subPop[i].fitnessTime > subPop[highestFitnessIndex].fitnessTime)
            {
                highestFitnessIndex = i;
            }
            break;

        case 2:
            if (subPop[i].fitnessFuel > subPop[highestFitnessIndex].fitnessFuel)
            {
                highestFitnessIndex = i;
            }
            break;

        case 3:
            if (subPop[i].fitness > subPop[highestFitnessIndex].fitness)
            {
                highestFitnessIndex = i;
            }
            break;

        default:
            return;
        }
    }

    switch (index)
    {
    case 0:

        if (newSon[0].fitnessDistance < subPop[highestFitnessIndex].fitnessDistance)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPDISTANCE\n");
            printf("Fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessDistance);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = newSon[0].id;
            subPop[highestFitnessIndex].fitnessDistance = newSon[0].fitnessDistance;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessDistance);

            replaced = 1;
        }

        break;

    case 1:

        if (newSon[0].fitnessTime < subPop[highestFitnessIndex].fitnessTime)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPTIME\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessTime);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = newSon[0].id;
            subPop[highestFitnessIndex].fitnessTime = newSon[0].fitnessTime;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessTime);

            replaced = 1;
        }

        break;

    case 2:

        if (newSon[0].fitnessFuel < subPop[highestFitnessIndex].fitnessFuel)
        {
            printf("\nSUBSTITUINDO O FILHO NA SUBPOPFuel\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitnessFuel);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = newSon[0].id;
            subPop[highestFitnessIndex].fitnessFuel = newSon[0].fitnessFuel;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitnessFuel);

            replaced = 1;
        }

        break;

    case 3:

        if (newSon[0].fitness < subPop[highestFitnessIndex].fitness)
        {
            printf("\nSUBSTITUINDO O FILHO NA SubpopPonderacao\n");
            printf("fitness do individuo subst: %d\n", subPop[highestFitnessIndex].fitness);
            printf("Id do individuo substituido: %d\n", subPop[highestFitnessIndex].id);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    subPop[highestFitnessIndex].route[j][k] = newSon[0].route[j][k];
                }
            }

            subPop[highestFitnessIndex].id = newSon[0].id;
            subPop[highestFitnessIndex].fitness = newSon[0].fitness;

            printf("Id do atual: %d\n", subPop[highestFitnessIndex].id);
            printf("Fitness do atual: %d\n", subPop[highestFitnessIndex].fitness);

            replaced = 1;
        }

        break;

    default:
        break;
    }
}

/*
    -----------------------------------
              evolvePop()
    -----------------------------------

    This function:
    - The population will be evolved until the limit of generations is reached or the optimal solution is found;
*/

void evolvePop(int rodada, int *populationFitness, Individual *population, Individual *newSon, int *tournamentFitness, Individual *tournamentIndividuals,
               Individual *subpop1, Individual *subpop2, int startIndex)
{

    // int startIndex = 1;
    int *index = &startIndex;

    // Selection of the Elite of each subpopulation
    selectElite(subPopDistance, nextSubPopDistance, 0);
    selectElite(subPopTime, nextSubPopTime, 1);
    selectElite(subPopFuel, nextSubPopFuel, 2);
    selectElite(subPopWeighting, nextSubPopWeighting, 3);

    // Integration of the sons in the nextPop
    for (int i = ELITISM_SIZE_POP; i < SUBPOP_SIZE; i++)
    {
        // Selection between the subpopulations and tournament
        subPopSelection(tournamentIndividuals, parent, tournamentFitness, subpop1, subpop2);

        // Crossing between the parents
        switch (CROSSINGTYPE)
        {
        case 1:
            onePointCrossing(index, parent, newSon);
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

        // Adding the son in the next population
        nextSubPopDistance[i] = *newSon;
        nextSubPopTime[i] = *newSon;
        nextSubPopFuel[i] = *newSon;
        nextSubPopWeighting[i] = *newSon;
    }

    // And now we will update the subPops with the individual of the nextSubPop
    updatePop(subPopDistance, nextSubPopDistance);
    updatePop(subPopTime, nextSubPopTime);
    updatePop(subPopFuel, nextSubPopFuel);
    updatePop(subPopWeighting, nextSubPopWeighting);

    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        printf("\nID do indivíduo %d \n", subPopDistance[i].id);
        printf("Fitness do indivíduo %d \n", subPopDistance[i].fitnessDistance);
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", subPopDistance[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    /*
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        printf("\nID do indivíduo %d \n", nextSubPopDistance[i].id);
        printf("Fitness do indivíduo %d \n", nextSubPopDistance[i].fitnessDistance);
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                printf("%d ", nextSubPopDistance[i].route[j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }

    // Comparing the son with every individual from subpopulations
    for (int i = 0; i < NUM_SUBPOP + 1; i++)
    {
        switch (i)
        {
        case 0:
            compareSonSubPop(newSon, subPopDistance, i);
            break;

        case 1:
            compareSonSubPop(newSon, subPopTime, i);
            break;

        case 2:
            compareSonSubPop(newSon, subPopFuel, i);
            break;

        case 3:
            compareSonSubPop(newSon, subPopWeighting, i);
            break;

        default:
            break;
        }
    }
    */
}
