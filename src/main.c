#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"
#include "fitness.h"
#include "selection.h"
#include "crossing.h"
#include "mutation.h"
#include "initialization.h"
#include "print.h"

int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
int **populacaoAtual;
int *populationFitness;
int *tournamentFitness;

Individual *population, *parent;
Individual *tournamentIndividuals;
Individual *nextPop;
Individual *newSon;

Individual *subPopDistance;
Individual *subPopTime;
Individual *subPopFuel;
Individual *subPopCapacity;
Individual *subPopWeighting;

Individual *nextSubPopDistance;
Individual *nextSubPopTime;
Individual *nextSubPopFuel;
Individual *nextSubPopWeighting;

Individual *subpop1;
Individual *subpop2;

Storage *distance_clients, *time_clients_end;
Client clients[NUM_CLIENTS + 1];

int firstfitness = -1;
int secondfitness = -1;
int cont = 0;

int main()
{
    printf("Executing\n");
    srand(time(NULL));

    // Calculating the time spent executing
    double time_spent = 0.0;
    clock_t begin = clock();

    // int solutionFound = 0;
    int i, rouds, idTrack;
    FILE *file;

    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS + 1));
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);

    population = (Individual *)malloc(sizeof(Individual) * POP_SIZE);
    parent = (Individual *)malloc(sizeof(Individual) * 2);
    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);
    tournamentIndividuals = (Individual *)malloc(sizeof(Individual) * (QUANTITYSELECTEDTOURNAMENT));
    nextPop = (Individual *)malloc(sizeof(Individual) * (1));
    newSon = (Individual *)malloc(sizeof(Individual) * (1));

    subPopDistance = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopTime = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopFuel = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopCapacity = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subPopWeighting = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    nextSubPopDistance = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopTime = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopFuel = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    nextSubPopWeighting = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    subpop1 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));
    subpop2 = (Individual *)malloc(sizeof(Individual) * (SUBPOP_SIZE));

    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);

    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS + 1));
    }

    // Verifying the memory allocation
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL || subPopDistance == NULL || subPopTime == NULL ||
        subPopFuel == NULL || subPopCapacity == NULL || subPopWeighting == NULL || nextSubPopDistance == NULL || nextSubPopTime == NULL || nextSubPopFuel == NULL || nextSubPopWeighting == NULL)
    {
        printf("Fail locating memory!\n");
        return 0;
    }

    //---------------------------------------------------------------------------------------------------------------------
    // Initializating the population
    initPop(population);

    // Distributing the population in subpops
    distributeSubpopulation(population);

    // Calculando o fitness de cada subpopulação
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        fitnessDistance(subPopDistance, i);

        fitnessTime(subPopTime, i);

        fitnessFuel(subPopFuel, i);

        fitness(subPopWeighting, i);
    }

    // printf("numero de individuos de ELITE %d\n", ELITISM_SIZE_POP);
    // printf("numero de individuos da prox poplacao %d\n", SUBPOP_SIZE);

    file = fopen("output/dataVRP.xls", "a+");

    int previousHighestFitnessDistanceID = -1;
    int previousHighestFitnessTimeID = -1;
    int previousHighestFitnessFuelID = -1;
    int previousHighestFitnessWeightingID = -1;

    // Colocando todos os id das nextPop com -1

    // Resetting all individuals from nextPop
    for (int i = 0; i < SUBPOP_SIZE; i++)
    {
        nextSubPopDistance[i].id = -1;
        nextSubPopTime[i].id = -1;
        nextSubPopFuel[i].id = -1;
        nextSubPopWeighting[i].id = -1;
    }

    idTrack = 1;

    printf("FITNESS A BATER\n");
    printf("Distancia: %d\n", subPopDistance[0].fitnessDistance);
    printf("tempo: %d\n", subPopTime[0].fitnessTime);
    printf("Combustivel: %d\n", subPopFuel[0].fitnessFuel);
    printf("Ponderacao: %d\n", subPopWeighting[0].fitness);

    for (int i = 0; i < ROUNDS; i++)
    {
        evolvePop(rouds, populationFitness, population, newSon, tournamentFitness, tournamentIndividuals, subpop1, subpop2, i, &idTrack, &previousHighestFitnessDistanceID,
                  &previousHighestFitnessTimeID, &previousHighestFitnessFuelID, &previousHighestFitnessWeightingID);

        // if (newSon[0].fitness != subPopWeighting[0].fitness)
        //{
        //     printf("%d\n", newSon[0].fitness);
        // }

        if ((i % 1000) == 0)
        {
            printf("DISTANCE %d \n", subPopDistance[0].fitnessDistance);
            printf("TIME %d \n", subPopTime[0].fitnessTime);
            printf("FUEL %d \n", subPopFuel[0].fitnessFuel);
            printf("POND %d \n", subPopWeighting[0].fitness);
        }
    }

    // Printaremos todos os individuos de todas as subpopulações ao final, assim conseguiremos ver quais mudaram e quais nao

    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    else
    {
        fprintf(file, "|==================================Subpopulacaoo ao final DISTANCIA==================================|\n");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopDistance[i].id, subPopDistance[i].fitnessDistance);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopDistance[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Time==================================|\n ");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopTime[i].id, subPopTime[i].fitnessTime);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopDistance[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Fuel==================================|\n ");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopFuel[i].id, subPopFuel[i].fitnessFuel);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopFuel[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }

        fprintf(file, "|==================================Subpopulacaoo ao final Weighting==================================|\n");
        for (int i = 0; i < SUBPOP_SIZE; i++)
        {
            fprintf(file, "Individuo %d com id %d e fitness de %d\n", i + 1, subPopWeighting[i].id, subPopWeighting[i].fitness);
            for (int j = 0; j < NUM_VEHICLES; j++)
            {
                for (int k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    fprintf(file, "%d ", subPopWeighting[i].route[j][k]);
                }
                fprintf(file, "\n");
            }
            fprintf(file, "\n");
        }
    }

    // Calculating the average, best fitness and dp from distance
    int valDistance = 0;
    int bestDistanceFitness = __INT_MAX__;

    for (i = 0; i < SUBPOP_SIZE; i++)
    {
        valDistance += subPopDistance[i].fitnessDistance;
        if (subPopDistance[i].fitnessDistance < bestDistanceFitness)
        {
            bestDistanceFitness = subPopDistance[i].fitnessDistance;
        }
    }
    int media_val_Dist = valDistance / SUBPOP_SIZE;

    double vDist = 0;
    double varianciaDist = 0;
    double dpDist = 0;
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        double desvio = 0;
        desvio = subPopDistance[k].fitnessDistance - media_val_Dist;
        vDist += pow(desvio, 2);
    }
    varianciaDist = vDist / SUBPOP_SIZE;
    dpDist = sqrt(varianciaDist);

    // Calculating the average, best fitness and dp from Time
    int valTime = 0;
    int bestTimeFitness = __INT_MAX__;

    for (i = 0; i < SUBPOP_SIZE; i++)
    {
        valTime += subPopTime[i].fitnessTime;
        if (subPopTime[i].fitnessTime < bestTimeFitness)
        {
            bestTimeFitness = subPopTime[i].fitnessTime;
        }
    }
    int media_val_Time = valTime / SUBPOP_SIZE;

    double vTime = 0;
    double varianciaTime = 0;
    double dpTime = 0;
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        double desvio = 0;
        desvio = subPopTime[k].fitnessTime - media_val_Time;
        vTime += pow(desvio, 2);
    }
    varianciaTime = vTime / SUBPOP_SIZE;
    dpTime = sqrt(varianciaTime);

    // Calculating the average, best fitness and dp from Fuel
    int valFuel = 0;
    int bestFuelFitness = __INT_MAX__;

    for (i = 0; i < SUBPOP_SIZE; i++)
    {
        valFuel += subPopFuel[i].fitnessFuel;
        if (subPopFuel[i].fitnessFuel < bestFuelFitness)
        {
            bestFuelFitness = subPopFuel[i].fitnessFuel;
        }
    }
    int media_val_Fuel = valFuel / SUBPOP_SIZE;

    double vFuel = 0;
    double varianciaFuel = 0;
    double dpFuel = 0;
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        double desvio = 0;
        desvio = subPopFuel[k].fitnessFuel - media_val_Fuel;
        vFuel += pow(desvio, 2);
    }
    varianciaFuel = vFuel / SUBPOP_SIZE;
    dpFuel = sqrt(varianciaFuel);

    // Calculating the average, best fitness and dp from Wheighting
    int valWeighting = 0;
    int bestWeightingFitness = __INT_MAX__;

    for (i = 0; i < SUBPOP_SIZE; i++)
    {
        valWeighting += subPopWeighting[i].fitness;
        if (subPopWeighting[i].fitness < bestWeightingFitness)
        {
            bestWeightingFitness = subPopWeighting[i].fitness;
        }
    }
    int media_val_Weighting = valWeighting / SUBPOP_SIZE;

    double vWeighting = 0;
    double varianciaWeighting = 0;
    double dpWeighting = 0;
    for (int k = 0; k < SUBPOP_SIZE; k++)
    {
        double desvio = 0;
        desvio = subPopWeighting[k].fitness - media_val_Weighting;
        vWeighting += pow(desvio, 2);
    }
    varianciaWeighting = vWeighting / SUBPOP_SIZE;
    dpWeighting = sqrt(varianciaWeighting);

    // time spent executing in seconds
    clock_t end = clock();
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    if (file == NULL)
    {
        printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
        fclose(file);
        return 0;
    }
    else
    {

        fprintf(file, "\n");
        fprintf(file, "Population Size: %d\n", POP_SIZE);
        fprintf(fprintf, "SubPopulations size: %d\n", SUBPOP_SIZE);
        fprintf(file, "Number of Vehicles: %d\n", NUM_VEHICLES);
        fprintf(file, "Number of Clients: %d\n", NUM_CLIENTS);
        fprintf(file, "Vehicle Capacity: %d\n", VEHICLES_CAPACITY);
        fprintf(file, "Selection Type: Tournament\n");
        fprintf(file, "Crossover Points: %d points\n", CROSSINGTYPE);
        fprintf(file, "Mutation Rate: %f\n", MUTATIONRATE);
        fprintf(file, "Elitism Rate: %f\n", ELITISMRATE);
        fprintf(file, "Rounds: %d\n", ROUNDS);
        fprintf(file, "Time: %f\n", time_spent);
        // fprintf(file, "O primeiro fitness foi de: %d\n", firstfitness);
        fprintf(file, "--------------------Distance------------------\n");
        fprintf(file, "A melhor fitness da subPop Distancia eh: %d\n", bestDistanceFitness);
        fprintf(file, "A media dos fitness da subPop Distancia eh: %d\n", media_val_Dist);
        fprintf(file, "O desvio Padrao da subPop Distancia eh: %.5f\n", dpDist);
        fprintf(file, "--------------------Time------------------\n");
        fprintf(file, "A melhor fitness da subPop Time eh: %d\n", bestTimeFitness);
        fprintf(file, "A media dos fitness da subPop Time eh: %d\n", media_val_Time);
        fprintf(file, "O desvio Padrao da subPop Time eh: %.5f\n", dpTime);
        fprintf(file, "--------------------Fuel------------------\n");
        fprintf(file, "A melhor fitness da subPop Fuel eh: %d\n", bestFuelFitness);
        fprintf(file, "A media dos fitness da subPop Fuel eh: %d\n", media_val_Fuel);
        fprintf(file, "O desvio Padrao da subPop Fuel eh: %.5f\n", dpFuel);
        fprintf(file, "--------------------Weighting------------------\n");
        fprintf(file, "A melhor fitness da subPop Weighting eh: %d\n", bestWeightingFitness);
        fprintf(file, "A media dos fitness da subPop Weighting eh: %d\n", media_val_Weighting);
        fprintf(file, "O desvio Padrao da subPop Weighting eh: %.5f\n", dpWeighting);
        // fprintf(file, "Solution Found = %d\n", solutionFound);
    }

    // Releasing memory
    for (i = 0; i < NUM_CLIENTS + 1; i++)
    {
        free(populacaoAtual[i]);
    }

    free(populacaoAtual);
    free(populationFitness);
    free(population);
    free(parent);
    free(tournamentFitness);
    free(tournamentIndividuals);
    free(nextPop);
    free(newSon);

    free(subPopDistance);
    free(subPopTime);
    free(subPopFuel);
    free(subPopCapacity);
    free(subPopWeighting);

    free(nextSubPopDistance);
    free(nextSubPopTime);
    free(nextSubPopFuel);
    free(nextSubPopWeighting);

    free(distance_clients);
    free(time_clients_end);

    return 0;
}
