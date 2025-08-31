# Vehicle Routing Problem (VRP) 

## About This Project

This repository contains a Genetic Algorithm implementation for solving the Vehicle Routing Problem (VRP), developed as part of a Scientific Initiation project. The VRP is a combinatorial optimization problem that aims to find optimal routes for a fleet of vehicles to deliver goods or services to customers while minimizing costs.

## Disclaimer

This code was developed in early 2023 during my fourth semester at university. As a developing programmer at that time, the implementation has certain limitations:

- Some unnecessary loops and code repetition (particularly in the fitness function)
- Non-optimal algorithmic choices that I would approach differently today
- Code organization that reflects my programming experience at that time

Despite these limitations, the project successfully achieved its main objective: validating the AEMMT algorithm compared to its mono-objective version. Due to challenges with continuing development in C (including my own limitations with the language), the implementation was later evolved in Java, where all aspects were improved.

If you're studying or modifying this code and encounter difficulties, feel free to contact me.

*Note: Before making this version available, I removed many redundant loops and unnecessary comments, though some may remain.*

## Running the Code

To run the code, use GDB with the following commands in the terminal:

```bash
gdb ./output/main.exe
run
```

## Project Structure

The solution is implemented in C and consists of several key components:

### Core Components

1. **Initialization**: Creates the initial population using the Gillet & Miller algorithm
2. **Fitness Evaluation**: Calculates route quality considering distance, time, and fuel costs
3. **Selection**: Implements both tournament and roulette selection with elitism
4. **Crossover**: Implements one-point and two-point crossover methods
5. **Mutation**: Provides two different mutation operators
6. **Population Management**: Handles population evolution and updates

## Technical Documentation

### Initialization Module

- **compare()**: Compares distances between two points in the distance array.

- **calculateDistance()**: Calculates Euclidean distance between two points on the Cartesian plane, representing client locations relative to the distribution center.

- **findClosestClient()**: Identifies the nearest unvisited client to the current position.

- **initPop()**: Initializes the population using the Gillet & Miller algorithm:
  - Places the distribution center at the origin of the Cartesian plane
  - Generates client points with random coordinates
  - Orders clients based on their distance from the distribution center
  - Constructs routes using a greedy nearest-neighbor approach
  - Creates a matrix of NUM_VEHICLES × (NUM_CLIENTS + 1) representing routes

### Fitness Module

- **fitness()**: Evaluates route quality by calculating:
  - Total distance traveled
  - Time required to complete routes
  - Fuel costs (selects optimal fuel type from three options)
  - Time window violations
  - Number of vehicles required
  
  The final fitness value is a weighted sum of these factors, with lower values indicating better solutions.

### Selection Module

- **elitism()**: Preserves top-performing individuals by copying them directly to the next generation.

- **rouletteSelection()**: Selects parent individuals with probability inversely proportional to their fitness value, favoring better solutions (lower fitness).

- **tournamentSelection()**: Randomly selects pairs of individuals and advances the one with lower fitness to become a parent.

### Crossover Module

- **indicaFaltantes()**, **indicaFaltantes2()**: Identify clients present in a parent route but missing from offspring routes.

- **compareFatherSon()**: Compares offspring and parent routes to find missing clients.

- **onePointCrossing()**: Implements single-point crossover by:
  - Selecting a random cut point
  - Taking the first segment from parent 1
  - Taking the second segment from parent 2
  - Resolving any missing or duplicate clients

- **twoPointCrossing()**: Similar to one-point crossing but uses two cut points to create three segments.

### Mutation Module

- **mutation()**: Swaps two randomly selected alleles (clients) within a single route line.

- **mutation2()**: Swaps a client between two randomly selected route lines, maintaining the same position in each line.

### Population Module

- **updatePop()**: Replaces the current population with the next generation.

- **evolvePop()**: Manages the complete evolution process:
  1. Applies elitism
  2. Performs selection
  3. Applies crossover
  4. Introduces mutations
  5. Recalculates fitness for the new population



