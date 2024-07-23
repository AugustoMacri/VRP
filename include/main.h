#ifndef MAIN_H
#define MAIN_H

/*
This part is for creating the static and global variables
*/

// Defines
#define POP_SIZE 10
#define NUM_CLIENTS 7
#define VEHICLES_SPEED 50 // 50km/h
#define NUM_VEHICLES 2
#define VEHICLES_CAPACITY 4
#define RANGE_COORDINATES 100
#define ELITISMRATE 0.15
#define MUTATIONRATE 0.01
#define ROUNDS 1
#define SELECTION 1    // 1- Roulette. 2- Tournament
#define CROSSINGTYPE 1 // 1- One Crossing point. 2 - Two Crossing points
#define NUM_SUBPOP 3   // This represents the number of subpopulations
#define SUBPOP_SIZE (POP_SIZE / NUM_SUBPOP)

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
    double x;
    double y;
    double distance;
    double start_time; // start of the time window
    double end_time;   // end of the time window
} Client;

extern Client clients[NUM_CLIENTS + 1];

// Individual struct
typedef struct
{
    int id;
    int route[NUM_VEHICLES][NUM_CLIENTS + 1];
    int fitness;
    int fitnessDistance;
    int fitnessTime;
    int fitnessFuel;
    int fitnessCapacity;
} Individual;

extern Individual *population;
extern Individual *parent;
extern Individual *tournamentIndividuals;
extern Individual *nextPop;

extern Individual *subPopDistance;
extern Individual *subPopTime;
extern Individual *subPopFuel;
extern Individual *subPopCapacity;
extern Individual *subPopWeighting;

extern Individual *subpop1;
extern Individual *subpop2;

// Double Struct
typedef struct
{
    double route[NUM_VEHICLES][NUM_CLIENTS + 1];
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
extern int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
extern int cont;

#endif