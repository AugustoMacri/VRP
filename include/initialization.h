#include "main.h"

void initPop(Individual *population, Client *clients);
double calculateDistance(Client c1, Client c2);
int findClosestClient(int currentClient, Client *clients, int visited[]);
