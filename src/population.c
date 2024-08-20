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
        subPop[i].id = nextSubPop[i].id;
        subPop[i].fitnessDistance = nextSubPop[i].fitnessDistance;
        subPop[i].fitnessTime = nextSubPop[i].fitnessTime;
        subPop[i].fitnessFuel = nextSubPop[i].fitnessFuel;
        subPop[i].fitness = nextSubPop[i].fitness;

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                subPop[i].route[j][k] = nextSubPop[i].route[j][k];
            }
        }
    }

    // Resetting all individuals from nextPop
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        nextSubPop[i].id = -1;
    }
}

/*
    -----------------------------------
            compareSonSubPop()
    -----------------------------------

    This function:
    - In this function we will compare the son that is generated in the crossing with every subpopulation;
    - If the son had a fitness smaller than at least one individual of that subpopulation, the son is put on the next generation of that one subpopulation;
*/
int compareSonSubPop(Individual *newSon, Individual *subPop, Individual *nextPop, int *previousHighestFitnessID, int index, int individual)
{

    int replaced = 0;
    // int highestFitnessIndex = -1;
    int individualVerified = 0;

    // Then we will replace the individual that have the higher fitness
    switch (index)
    {
    case 0:

        if (newSon[0].fitnessDistance < subPop[individual].fitnessDistance)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPDISTANCE\n");
            // printf("Fitness do individuo subst: %d\n", subPop[individual].fitnessDistance);
            *previousHighestFitnessID = subPop[individual].id;
            // printf("Id do individuo (que não aparecerá na nextPOP distance): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[individual].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[individual].id = newSon[0].id;
            nextPop[individual].fitnessDistance = newSon[0].fitnessDistance;

            // printf("Id do individuo na nextPop: %d\n", nextPop[individual].id);
            // printf("Fitness do individuo na nextPop: %d\n", nextPop[individual].fitnessDistance);

            replaced = 1;

            return 1;
        }

        break;

    case 1:

        if (newSon[0].fitnessTime < subPop[individual].fitnessTime)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPTIME\n");
            //  printf("fitness do individuo subst: %d\n", subPop[individual].fitnessTime);
            *previousHighestFitnessID = subPop[individual].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP TIME): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[individual].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[individual].id = newSon[0].id;
            nextPop[individual].fitnessTime = newSon[0].fitnessTime;

            // printf("Id do atual: %d\n", nextPop[individual].id);
            //  printf("Fitness do atual: %d\n", nextPop[individual].fitnessTime);

            replaced = 1;

            return 1;
        }

        break;

    case 2:

        if (newSon[0].fitnessFuel < subPop[individual].fitnessFuel)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPFuel\n");
            //  printf("fitness do individuo subst: %d\n", subPop[individual].fitnessFuel);
            *previousHighestFitnessID = subPop[individual].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP combustivel): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[individual].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[individual].id = newSon[0].id;
            nextPop[individual].fitnessFuel = newSon[0].fitnessFuel;

            // printf("Id do atual: %d\n", nextPop[individual].id);
            //  printf("Fitness do atual: %d\n", nextPop[individual].fitnessFuel);

            replaced = 1;

            return 1;
        }

        break;

    case 3:

        if (newSon[0].fitness < subPop[individual].fitness)
        {
            // printf("\nIntroduzindo Filho na NEXTSubpopPonderacao\n");
            //  printf("fitness do individuo subst: %d\n", subPop[individual].fitness);
            *previousHighestFitnessID = subPop[individual].id;
            // printf("Id do individuo substituido (que não aparecerá na nextPOP Pond): %d\n", *previousHighestFitnessID);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    nextPop[individual].route[j][k] = newSon[0].route[j][k];
                }
            }

            nextPop[individual].id = newSon[0].id;
            nextPop[individual].fitness = newSon[0].fitness;

            // printf("Id do atual: %d\n", nextPop[individual].id);
            //  printf("Fitness do atual: %d\n", nextPop[individual].fitness);

            replaced = 1;

            return 1;
        }

        break;

    default:
        break;
    }

    return 0;
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

    // Selection of the Elite of each subpopulation and put in nextPop
    selectElite(subPopDistance, nextSubPopDistance, 0);
    selectElite(subPopTime, nextSubPopTime, 1);
    selectElite(subPopFuel, nextSubPopFuel, 2);
    selectElite(subPopWeighting, nextSubPopWeighting, 3);

    // printf("=========================SUBPOP POND=========================\n");
    // printSubPop(subPopWeighting, SUBPOP_SIZE);
    // printf("=========================NEXTSUBPOP POND=========================\n");
    // printSubPop(nextSubPopWeighting, SUBPOP_SIZE);

    // For the first 5 generations we will complete the nextPop with new sons without comparing with the individuals already present in the current population, so that way we can guarantee the diversity
    if (startIndex < 5)
    {

        for (int i = ELITISM_SIZE_POP; i < SUBPOP_SIZE; i++)
        {
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
            mutation(newSon, startIndex);

            // Calculating the fitness of the son
            fitnessDistance(newSon, 0);
            fitnessTime(newSon, 0);
            fitnessFuel(newSon, 0);
            fitness(newSon, 0);

            // printf("=========================SON=========================\n");
            // printSubPop(newSon, 1);

            // Introducing the new son in the nextPop
            nextSubPopDistance[i] = newSon[0];
            nextSubPopTime[i] = newSon[0];
            nextSubPopFuel[i] = newSon[0];
            nextSubPopWeighting[i] = newSon[0];
        }

        // printf("=========================NEXTSUBPOP APOS A GERACAO DE FILHOS ALEATORIOS PARA A NEXTPOP=========================\n");
        // printSubPop(nextSubPopWeighting, SUBPOP_SIZE);
    }
    else
    {
        if (startIndex == 5)
        {
            // printf("=========================POP ATUAL de POND chegando pela primeira vez=========================\n");
            // printSubPop(subPopWeighting, SUBPOP_SIZE);
        }

        for (int i = ELITISM_SIZE_POP; i < SUBPOP_SIZE; i++)
        {
            int findBetterDist = 0;
            int findBetterTime = 0;
            int findBetterFuel = 0;
            int findBetterWeight = 0;

            // colocar um limite para quando isso vai rodar, se nao encontrar, precisa fazer com que complete ela daquele jeito
            for (int j = 0; j < 5; j++)
            {
                // printf("para o individuo %d oa geracao esta em %d\n", i, j);

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
                mutation(newSon, startIndex);

                // Calculating the fitness of the son
                fitnessDistance(newSon, 0);
                fitnessTime(newSon, 0);
                fitnessFuel(newSon, 0);
                fitness(newSon, 0);

                // Comparing the son created with each the worst individual of the population
                if (findBetterDist != 1)
                {
                    findBetterDist = compareSonSubPop(newSon, subPopDistance, nextSubPopDistance, previousHighestFitnessDistanceID, 0, i);

                    if (findBetterDist == 0)
                    {

                        nextSubPopDistance[i] = subPopDistance[i];
                    }
                }

                if (findBetterTime != 1)
                {
                    findBetterTime = compareSonSubPop(newSon, subPopTime, nextSubPopTime, previousHighestFitnessTimeID, 1, i);

                    if (findBetterTime == 0)
                    {
                        nextSubPopTime[i] = subPopTime[i];
                    }
                }

                if (findBetterFuel != 1)
                {
                    findBetterFuel = compareSonSubPop(newSon, subPopFuel, nextSubPopFuel, previousHighestFitnessFuelID, 2, i);

                    if (findBetterFuel == 0)
                    {
                        nextSubPopFuel[i] = subPopFuel[i];
                    }
                }

                if (findBetterWeight != 1)
                {
                    findBetterWeight = compareSonSubPop(newSon, subPopWeighting, nextSubPopWeighting, previousHighestFitnessWeightingID, 3, i);

                    if (findBetterWeight == 0)
                    {
                        nextSubPopWeighting[i] = subPopWeighting[i];
                    }
                }

                if (findBetterDist == 1 && findBetterTime == 1 && findBetterFuel == 1 && findBetterWeight == 1)
                {
                    break;
                }
            }
            // printf("findBetterDist : %d\n", findBetterDist);
            // printf("findBetterTime : %d\n", findBetterTime);
            // printf("findBetterFuel : %d\n", findBetterFuel);
            // printf("findBetterWeight : %d\n", findBetterWeight);
        }
    }

    // And now we will update the subPops with the individual of the nextSubPop
    updatePop(subPopDistance, nextSubPopDistance);
    updatePop(subPopTime, nextSubPopTime);
    updatePop(subPopFuel, nextSubPopFuel);
    updatePop(subPopWeighting, nextSubPopWeighting);

    // printf("=========================POP ATUAL de POND ao final=========================\n");
    // printSubPop(subPopWeighting, SUBPOP_SIZE);
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