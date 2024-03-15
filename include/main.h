/*
This part is for crating the static and global variables
*/

// Defines
#define POPSIZE 500
#define NUM_CLIENTS 7
#define NUM_VEHICLES 2
#define RANGE_COORDINATES 100
#define ELITISMRATE 0.2
#define MUTATIONRATE 0.1
#define ROUNDS 300
#define SELECTIONTYPE 3    // 1- Random. 2- Roulette. 3- Tournament
#define CROSSINGTYPE 1     // 1- One. 2- Two. 3- Three. 4- Four. Crossing points

//Global variables
int **populacaoAtual;    
int *fitnessDaPopulacao; 