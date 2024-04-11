/*
This part is for crating the static and global variables
*/

// Defines
#define POP_SIZE 500
#define NUM_CLIENTS 7
#define VEHICLES_SPEED 50 //50km/h
#define NUM_VEHICLES 2
#define VEHICLES_CAPACITY 4
#define RANGE_COORDINATES 100
#define ELITISMRATE 0.2
#define MUTATIONRATE 0.1
#define ROUNDS 300
#define SELECTIONTYPE 3    // 1- Random. 2- Roulette. 3- Tournament
#define CROSSINGTYPE 1     // 1- One. 2- Two. 3- Three. 4- Four. Crossing points

// Defines Fuel 
// 1- Gasoline, 2- Ethanol, 3- Diesel
#define NUM_FUEL_TYPES 3
#define G_FUEL_PRICE 4
#define E_FUEL_PRICE 3
#define D_FUEL_PRICE 6
#define G_FUEL_CONSUMPTION 10
#define E_FUEL_CONSUMPTION 7
#define D_FUEL_CONSUMPTION 12

//Global variables
int **populacaoAtual;
int *fitnessPopulation; 
int **currentClientArray[NUM_VEHICLES][NUM_CLIENTS + 1];
extern double distance_clients[NUM_VEHICLES][NUM_CLIENTS + 1];
