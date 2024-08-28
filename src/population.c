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

        nextSubPop[i].fitnessDistance = __INT_MAX__;
        nextSubPop[i].fitnessTime = __INT_MAX__;
        nextSubPop[i].fitnessFuel = __INT_MAX__;
        nextSubPop[i].fitness = __INT_MAX__;

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                nextSubPop[i].route[j][k] = 0;
            }
        }
    }
}

int resetSon(Individual *subPop)
{
    for (int i = 0; i < 1; i++)
    {
        subPop[i].id = -1;

        subPop[i].fitnessDistance = 0;
        subPop[i].fitnessTime = 0;
        subPop[i].fitnessFuel = 0;
        subPop[i].fitness = 0;

        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                subPop[i].route[j][k] = 0;
            }
        }
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

    // Then we will replace the individual if it have a higher fitness
    switch (index)
    {
    case 0:

        if (newSon[0].fitnessDistance < subPop[individual].fitnessDistance)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPDISTANCE\n");
            // printf("Fitness do individuo subst: %f\n", subPop[individual].fitnessDistance);
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
            // printf("Fitness do individuo na nextPop: %f\n", nextPop[individual].fitnessDistance);

            replaced = 1;

            return 1;
        }
        else
        {
            return 0;
        }

        break;

    case 1:

        if (newSon[0].fitnessTime < subPop[individual].fitnessTime)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPTIME\n");
            // printf("fitness do individuo subst: %f\n", subPop[individual].fitnessTime);
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
            // printf("Fitness do atual: %f\n", nextPop[individual].fitnessTime);

            replaced = 1;

            return 1;
        }
        else
        {
            return 0;
        }

        break;

    case 2:

        if (newSon[0].fitnessFuel < subPop[individual].fitnessFuel)
        {
            // printf("\nIntroduzindo Filho na NEXTSUBPOPFuel\n");
            // printf("fitness do individuo subst: %f\n", subPop[individual].fitnessFuel);
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
            // printf("Fitness do atual: %f\n", nextPop[individual].fitnessFuel);

            replaced = 1;

            return 1;
        }
        else
        {
            return 0;
        }

        break;

    case 3:

        if (newSon[0].fitness < subPop[individual].fitness)
        {
            // printf("\nIntroduzindo Filho na NEXTSubpopPonderacao\n");
            // printf("fitness do individuo subst: %f\n", subPop[individual].fitness);
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
            // printf("Fitness do atual: %f\n", nextPop[individual].fitness);

            replaced = 1;

            return 1;
        }
        else
        {
            return 0;
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

    // For the first 5 generations we will complete the nextPop with new sons without comparing with the individuals already present in the current population, so that way we can guarantee the diversity
    if (startIndex < GENERATIONS_BEFORE_COMPARATION)
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

            // Introducing the new son in the nextPop
            nextSubPopDistance[i] = newSon[0];
            nextSubPopTime[i] = newSon[0];
            nextSubPopFuel[i] = newSon[0];
            nextSubPopWeighting[i] = newSon[0];

            // printf("--------------------FIlho antes de zerar------------------\n");
            // printFilho(newSon);

            // Zerando os valores de neuSon
            resetSon(newSon);

            // printf("--------------------FIlho depois de zerado------------------\n");
            // printFilho(newSon);
        }

        /*
        if ((startIndex % 10) == 0)
        {
            int dpResult = calcValsPond(nextSubPopWeighting);

            // se o desvio padrao for 0
            if (dpResult == 1)
            {
                // printSubPop(nextSubPopWeighting, 3);

                printf("Desvio padrao 0\n");
            }
        }
        */
    }
    else
    {
        for (int i = ELITISM_SIZE_POP; i < SUBPOP_SIZE; i++)
        {
            int findBetterDist = 0;
            int findBetterTime = 0;
            int findBetterFuel = 0;
            int findBetterWeight = 0;

            // colocar um limite para quando isso vai rodar, se nao encontrar, precisa fazer com que complete ela daquele jeito
            for (int j = 0; j < 5; j++)
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

                // Comparing the son created with each individual of each subpopulation
                if (findBetterDist != 1)
                {
                    // printf("Comparando na dist\n");
                    findBetterDist = compareSonSubPop(newSon, subPopDistance, nextSubPopDistance, previousHighestFitnessDistanceID, 0, i);

                    if (findBetterDist == 0)
                    {
                        // printf("Copia o individuo [%d] para a prox populacao de dist\n", i);
                        nextSubPopDistance[i] = subPopDistance[i];
                    }
                }

                if (findBetterTime != 1)
                {
                    // printf("Comparando na time\n");
                    findBetterTime = compareSonSubPop(newSon, subPopTime, nextSubPopTime, previousHighestFitnessTimeID, 1, i);

                    if (findBetterTime == 0)
                    {
                        // printf("Copia o individuo [%d] para a prox populacao de Time\n", i);
                        nextSubPopTime[i] = subPopTime[i];
                    }
                }

                if (findBetterFuel != 1)
                {
                    // printf("Comparando na fuel\n");
                    findBetterFuel = compareSonSubPop(newSon, subPopFuel, nextSubPopFuel, previousHighestFitnessFuelID, 2, i);

                    if (findBetterFuel == 0)
                    {
                        // printf("Copia o individuo [%d] para a prox populacao de Fuel\n", i);
                        nextSubPopFuel[i] = subPopFuel[i];
                    }
                }

                if (findBetterWeight != 1)
                {
                    // printf("Comparando na Pond\n");
                    findBetterWeight = compareSonSubPop(newSon, subPopWeighting, nextSubPopWeighting, previousHighestFitnessWeightingID, 3, i);

                    if (findBetterWeight == 0)
                    {
                        // printf("Copia o individuo [%d] para a prox populacao de Fuel\n", i);
                        nextSubPopWeighting[i] = subPopWeighting[i];
                    }
                }

                if (findBetterDist == 1 && findBetterTime == 1 && findBetterFuel == 1 && findBetterWeight == 1)
                {
                    break;
                }

                // printf("findBetterDist : %d\n", findBetterDist);
                // printf("findBetterTime : %d\n", findBetterTime);
                // printf("findBetterFuel : %d\n", findBetterFuel);
                // printf("findBetterWeight : %d\n", findBetterWeight);
                // Zerando os valores de newSon
                resetSon(newSon);
            }
        }
    }

    // And now we will update the subPops with the individual of the nextSubPop
    updatePop(subPopDistance, nextSubPopDistance);
    updatePop(subPopTime, nextSubPopTime);
    updatePop(subPopFuel, nextSubPopFuel);
    updatePop(subPopWeighting, nextSubPopWeighting);

    if (startIndex % 50 == 0)
    {
        // calcValsPond(subPopWeighting);
    }

    // if (startIndex == 100)
    //{
    //     printf("RODADA %d\n", startIndex);
    //     calcValsPond(subPopWeighting);
    // }
}

void printFilho(Individual *subPop)
{
    for (int i = 0; i < 1; i++)
    {
        printf("Id: %d\n", subPop[i].id);
        printf("fitness dist: %.4f\n", subPop[i].fitnessDistance);
        printf("fitness Time: %.4f\n", subPop[i].fitnessTime);
        printf("fitness Fuel: %.4f\n", subPop[i].fitnessFuel);
        printf("fitness Ponderado: %.4f\n", subPop[i].fitness);

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

void printSubPop(Individual *subPop, int index)
{
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        printf("Id: %d\n", subPop[i].id);

        if (index == 0)
        {
            printf("fitness dist: %.4f\n", subPop[i].fitnessDistance);
        }
        else if (index == 1)
        {
            printf("fitness Time: %.4f\n", subPop[i].fitnessTime);
        }
        else if (index == 2)
        {
            printf("fitness Fuel: %.4f\n", subPop[i].fitnessFuel);
        }
        else if (index == 3)
        {
            printf("fitness Ponderado: %.4f\n", subPop[i].fitness);
        }

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

int calcValsPond(Individual *subPop)
{
    // Calculating the average, best fitness and dp from Wheighting
    double valWeighting = 0;
    double bestWeightingFitness = __INT_MAX__;

    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        valWeighting += subPop[i].fitness;

        if (subPop[i].fitness < bestWeightingFitness)
        {
            bestWeightingFitness = subPop[i].fitness;
        }
    }

    double media_val_Weighting = valWeighting / SUBPOP_SIZE;

    double vWeighting = 0;
    double varianciaWeighting = 0;
    double dpWeighting = 0;
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        double desvio = 0;
        desvio = subPop[k].fitness - media_val_Weighting;
        vWeighting += pow(desvio, 2);
    }
    varianciaWeighting = vWeighting / SUBPOP_SIZE;
    dpWeighting = sqrt(varianciaWeighting);

    printf("--------------------Weighting------------------\n");
    printf("A melhor fitness da subPop Weighting eh: %.4f\n", bestWeightingFitness);
    printf("A media dos fitness da subPop Weighting eh: %.4f\n", media_val_Weighting);
    printf("O desvio Padrao da subPop Weighting eh: %.5f\n", dpWeighting);

    // Verificar se o desvio padrão é 0 e retornar 1 para sinalizar isso
    if (dpWeighting < 0.01)
    {
        return 1;
    }

    return 0;
}