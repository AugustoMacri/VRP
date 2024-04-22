/*
This part is for crating the static and global variables
*/

// Defines
#define POP_SIZE 1
#define NUM_CLIENTS 7
#define VEHICLES_SPEED 50 // 50km/h
#define NUM_VEHICLES 2
#define VEHICLES_CAPACITY 4
#define RANGE_COORDINATES 100
#define ELITISMRATE 0.2
#define MUTATIONRATE 0.1
#define ROUNDS 300
#define SELECTIONTYPE 3 // 1- Random. 2- Roulette. 3- Tournament
#define CROSSINGTYPE 1  // 1- One. 2- Two. 3- Three. 4- Four. Crossing points

// Defines Fuel
// 1- Gasoline, 2- Ethanol, 3- Diesel
#define NUM_FUEL_TYPES 3
#define G_FUEL_PRICE 5.48
#define E_FUEL_PRICE 3.99
#define D_FUEL_PRICE 8.79
#define G_FUEL_CONSUMPTION 7.53
#define E_FUEL_CONSUMPTION 5
#define D_FUEL_CONSUMPTION 12

// Weights
#define WEIGHT_NUM_VEHICLES 0.25
#define WEIGHT_NUM_VIOLATIONS 0.5
#define WEIGHT_TOTAL_COST 0.75

// TIME - Here we define the time window, so that way we can calculate the time that each client will have in this time window (8AM - 8PM)
#define MAX_START_TIME 8
#define MAX_END_TIME 20
#define WINDOW_SIZE 0.5

//fitness
extern int *currentFitness;

// Global variables
extern int **populacaoAtual;
//extern int *fitnessPopulation;
extern int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
extern double distance_clients[NUM_VEHICLES][NUM_CLIENTS + 1];
extern double time_clients_end[NUM_VEHICLES][NUM_CLIENTS + 1];

