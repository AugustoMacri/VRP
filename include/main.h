#ifndef MAIN_H
#define MAIN_H

/*
This part is for creating the static and global variables
*/
#include <math.h>

// Defines
#define POP_SIZE 10 // Verify the QUANTITYSELECTEDTOURNAMENT before
#define VEHICLES_SPEED 50 // 50km/h
#define RANGE_COORDINATES 100
#define ELITISMRATE 0.05
#define MUTATIONRATE 0.01
#define ROUNDS 1
#define SELECTION 1    // 1- Roulette. 2- Tournament
#define CROSSINGTYPE 1 // 1- One Crossing point. 2 - Two Crossing points
#define NUM_SUBPOP 3   // This represents the number of subpopulations
#define SUBPOP_SIZE (POP_SIZE / NUM_SUBPOP)
#define ELITISM_SIZE_POP ((int)ceil(SUBPOP_SIZE * ELITISMRATE))

#define BEFORE_COMPARATION_RATE 0.05
#define GENERATIONS_BEFORE_COMPARATION ((int)ceil(ROUNDS * BEFORE_COMPARATION_RATE))

//New Variables (Now that we need to read the benchmarks, we can't use #define)
extern int NUM_CLIENTS;
extern int NUM_VEHICLES;
extern int VEHICLES_CAPACITY;


// Defines Fuel
// 1- Gasoline, 2- Ethanol, 3- Diesel
#define NUM_FUEL_TYPES 3
#define G_FUEL_PRICE 5.48
#define E_FUEL_PRICE 3.99
#define D_FUEL_PRICE 8.79
#define G_FUEL_CONSUMPTION 7.53
#define E_FUEL_CONSUMPTION 5
#define D_FUEL_CONSUMPTION 12

// Client struct
typedef struct
{
    int id; // Nova identificação para os clientes
    double x;
    double y;
    int demand; // Nova variável de demanda de pacotes por cliente
    double distance;
    double start_time; // start of the time window
    double end_time;   // end of the time window

    /*
    As seguintes variaveis estao presentes no txt, mas ainda nao serao trabalhadas por conta da janela de tempo, que
    temos funções específicas para trabalharem com elas, e sua implementacao acarretará muitas mudancas, que não é o foco no momento
    */

    int readyTime;
    int dueDate;
    int serviceTime;

} Client;

extern Client *clients;

// Individual struct
typedef struct
{
    int id;
    int **route;
    double fitness;
    double fitnessDistance;
    double fitnessTime;
    double fitnessFuel;
    int fitnessCapacity;
} Individual;

extern Individual *population;
extern Individual *parent;
extern Individual *tournamentIndividuals;
extern Individual *nextPop;
extern Individual *newSon;

// Store every individual in diferent subpopulations
extern Individual *subPopDistance;
extern Individual *subPopTime;
extern Individual *subPopFuel;
extern Individual *subPopCapacity;
extern Individual *subPopWeighting;

// Store the individual that will be going to the next population
extern Individual *nextSubPopDistance;
extern Individual *nextSubPopTime;
extern Individual *nextSubPopFuel;
extern Individual *nextSubPopWeighting;

extern Individual *subpop1;
extern Individual *subpop2;

// Double Struct
typedef struct
{
    double **route;
} Storage;

extern Storage *distance_clients;
extern Storage *time_clients_end;

// Selection
#define QUANTITYSELECTEDTOURNAMENT 2 // Quantity of selected individuals
extern int *tournamentFitness;

// Weights
#define WEIGHT_NUM_VEHICLES 0.25
#define WEIGHT_NUM_VIOLATIONS 0.5
#define WEIGHT_TOTAL_COST 0.75
#define WEIGHT_CAPACITY 0.65

// TIME - Here we define the time window, so that way we can calculate the time that each client will have in this time window (8AM - 8PM)
#define MAX_START_TIME 8
#define MAX_END_TIME 20
#define WINDOW_SIZE 0.5

// fitness
extern int *populationFitness;

// Global variables
extern int **populacaoAtual;
extern int ***currentClientArray;
extern int cont;

#endif