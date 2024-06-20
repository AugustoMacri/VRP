# Vehicle Routing Problem (VRP)

Welcome to the Vehicle Routing Problem repository for my Scientific Initiation project.

## Overview

This repository is dedicated to solving the Vehicle Routing Problem (VRP) using a Genetic Algorithm. The VRP is a classic problem in the field of operations research and optimization, aiming to optimize the routes of vehicles to deliver packages or services to a set of customers while minimizing costs or maximizing efficiency.

## Current Status

This project is currently in progress. I am developing a Genetic Algorithm in the C programming language.

## Documentation

### Initialization.c

- **compare()**:
  This function aims to compare the distances between two points in the distance array.

- **calculateDistance()**:
  This function calculates the distance between two points on a graph in a Cartesian plane. These points are the clients in our route, each having an X and Y value and their distance from the distribution center, which is located at the center of the plane.

- **findClosestClient()**:
  This function aims to find the closest point to the current point. In other words, if we are at point 2 on the graph, it will calculate to find the closest point to where we are currently.

- **initPop()**:
  Using the Gillet & Miller algorithm, this function initializes the initial population of individuals. After placing the distribution center at the center of the Cartesian plane, it generates points and positions them randomly on the graph. Then, after randomly positioning the points on the graph, it will order them from 1 to (NUM_CLIENTS + 1) based on their distance from the distribution center. This results in a graph that will be traversed starting from the distribution center and then moving to the point closest to the distribution center. Using a greedy algorithm, it will trace a route by always looking for the next closest point to the current point, until reaching VEHICLES_CAPACITY number of visited points, completing the first line of the route. The next lines will be created similarly, resulting in a matrix of NUM_VEHICLES by NUM_CLIENTS + 1 (the addition of one in NUM_CLIENTS accounts for the distribution center).

### fitness.c

- **fitness()**:
  This function calculates the fitness of each individual in the population, based on distance + time + fuel, each with different weights. It also considers factors such as different types of fuel, number of time window violations, and other factors. First, it recalculates the distance between each point in the visit order of the route and the time window for each client on the route. Then, it calculates the best fuel type for a specific route, choosing from three different types, each with different km/l and price values, always selecting the fuel that will generate the lowest cost. Finally, the fitness value is calculated from the data gathered during the fitness execution. The total cost is the sum of the total distance traveled times its weight, the total time to travel the route times its weight, and the total fuel cost times its weight. The final fitness value is calculated by multiplying the number of vehicles needed to perform the route by its weight, plus the number of time window violations times its weight, plus the total cost previously calculated.

### selection.c

This part of the code is responsible for selecting the individuals to be crossed to generate offspring for the next population, using two different types of selection: roulette selection or tournament selection. It also includes elitism.

- **elitism()**:
  This function performs elitism, selecting the best individuals from the current population based on a proportion and copying them to the next population.

- **rouletteSelection()**:
  This function performs roulette selection, where two individuals are chosen to be crossed, generating an offspring for the next population. The roulette selection gives higher probability to individuals with lower fitness, aiming to cross the best individuals in the population.

- **tournamentSelection()**:
  This selection randomly selects two individuals from the population and holds a tournament between them, choosing the one with lower fitness. This process is repeated to choose the second parent.

### crossing.c

- **indicaFaltantes()**:
  This function traverses the route of parent[0], creating a vector that indicates which clients exist in the parent’s route but not in the offspring’s route, placing them in a vector.

- **indicaFaltantes2()**:
  This function performs the same task as the previous one, but the parent selection is random.

- **compareFatherSon()**:
  This function compares a line in the offspring’s route with the same line in the parent’s route, indicating which clients are missing in the offspring’s route.

- **onePointCrossing()**:
  This function generates an offspring from the crossing of two previously selected parents by choosing a random cut point. Clients from parent[0] before this cut point will form the first half of the offspring’s route, and clients from parent[1] after the cut point will form the second half.

- **twoPointCrossing()**:
  This function performs the same process as the previous one but with two random cut points instead of one.

### mutation.c

- **mutation()**:
  This mutation function works as follows: two points are randomly generated, each indicating an allele of the offspring’s chromosome. In one of the offspring’s lines, the alleles indicated by the generated points will swap places.

- **mutation2()**:
  This mutation works as follows: only one point is selected for mutation. Then, two lines of the offspring’s route are randomly chosen, and the allele at column x and line y, for example, will swap places with the allele at column x and another randomly chosen line z.

### population.c

- **updatePop()**:
  This function updates the population, replacing the current population with nexPop.

- **evolvePop()**:
  This function evolves the population by first performing elitism to retain the best individuals from the previous population, then selection, crossing, and finally mutation. After this process, the fitness of the new population is recalculated.

