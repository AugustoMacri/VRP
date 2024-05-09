#include "main.h"

void initPop(Individual *population);
double calculateDistance(Client c1, Client c2);
int findClosestClient(int currentClient, Client clients[], int visited[]);
