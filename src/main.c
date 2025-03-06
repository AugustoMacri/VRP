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

int NUM_VEHICLES;
int VEHICLES_CAPACITY;
int NUM_CLIENTS;

Client *clients;
Individual *population;
Individual *parent;
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

Storage *distance_clients;
Storage *time_clients_end;

int ***currentClientArray;
int **populacaoAtual;
int *populationFitness;
int *tournamentFitness;

// Client clients[NUM_CLIENTS];

int firstfitness = -1;
int secondfitness = -1;
int cont = 0;

/*
    -----------------------------------
              readBenchmark()
    -----------------------------------

    This function:
    - Will convert all the data in a .txt file (Solomon Benchmarks) to values in variables
    - In these benchmarks we need to cacth:
        - Number of vehicles;
        - Number of clients;
        - NO. of each client
        - Capacity of each vehicle;
        - X and Y coordenates;
        - Demand
    - A new client struct is required
    - We will need to edit how the capacity and num vehicles is defined. We need to, beside use static definition, we use global variables,
    so that way we can modify them in this function
*/
void readBenchmark(const char *filename, Client **clients)
{
    // Declaring necessary variables
    // int numVehicles, vehicleCapacity, numClients = 0;

    // Abrindo arquivo das benchMarks para leitura
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("ERROR: Não foi possível abrir o arquivo.\n");
        return;
    }
    else
    {
        printf("Arquivo aberto com sucesso.\n");

        char buffer[256];

        // Ignorando a primeira linha que é de título da benchmark
        fgets(buffer, sizeof(buffer), file);

        // Lendo as primeiras linhas de número de veículos e capacidade
        fscanf(file, "%*s %*s %*s %d %d", &NUM_VEHICLES, &VEHICLES_CAPACITY);

        printf("Numero de veiculos %d\n", NUM_VEHICLES);
        printf("Capacidade dos veiculos %d\n", VEHICLES_CAPACITY);

        // Ignorando as duas linhas de título das colunas
        fgets(buffer, sizeof(buffer), file); // ignora linha vazia abaixo
        fgets(buffer, sizeof(buffer), file); // Linha customer
        fgets(buffer, sizeof(buffer), file); // Linha títulos das colunas

        // Lendo o número de clientes
        // numClients = 0;
        // Further code to read clients would go here

        // Primeiro contando o número de clientes para não dar problema de alocação
        int num_clients = 0;
        while (fgets(buffer, sizeof(buffer), file))
        {
            if (sscanf(buffer, "%d %*d %*d %*d %*d %*d %*d", &num_clients) == 1)
            {
                num_clients++;
            }
        }

        NUM_CLIENTS = num_clients;

        // Fazendo de novo
        rewind(file);
        for (int i = 0; i < 5; i++)
            fgets(buffer, sizeof(buffer), file); // Ignora cabeçalhos

        // Alocando memória para clientes
        *clients = malloc(NUM_CLIENTS * sizeof(Client));
        if (*clients == NULL)
        {
            printf("Erro alocando a bomba da memoria");
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // Agora sim, continuando de onde tinha parado
        int idx = 0;
        while (fgets(buffer, sizeof(buffer), file))
        {
            // Utilizando sscanf para ler os valores e atribuí-los ao cliente atual
            int id, x, y, demand, readyTime, dueDate, serviceTime;
            int parsedValues = sscanf(buffer, "%d %d %d %d %d %d %d",
                                      &id, &x, &y, &demand, &readyTime, &dueDate, &serviceTime);

            // Verifica se todos os valores foram lidos corretamente
            if (parsedValues == 7)
            {
                (*clients)[idx].id = id;
                // printf("Cliente %d\n", clients[idx].id);
                (*clients)[idx].x = x;
                (*clients)[idx].y = y;
                (*clients)[idx].demand = demand;
                (*clients)[idx].readyTime = readyTime;
                (*clients)[idx].dueDate = dueDate;
                (*clients)[idx].serviceTime = serviceTime;
                (idx)++;
            }
            else
            {
                // printf("Não conseguiu ler todos os valores da linha. Valores lidos: %d\n", parsedValues);
            }
        }
    }

    // printf("CLIENTE 20\n");
    // printf("ID: %d\n", (*clients)[20].id);
    // printf("X: %f\n", (*clients)[20].x);
    // printf("Y: %f\n", (*clients)[20].y);
    // printf("Demanda: %d\n", (*clients)[20].demand);
    // printf("Tempo de inicio: %d\n", (*clients)[20].readyTime);
    // printf("Tempo de fim: %d\n", (*clients)[20].dueDate);
    // printf("Tempo de serviço: %d\n", (*clients)[20].serviceTime);

    //loop por cada cliente do array, printando sua posição e seu id
    // for (int i = 0; i < NUM_CLIENTS; i++)
    // {
    //     printf("Cliente %d ", i);
    //     printf("ID: %d\n", (*clients)[i].id);
    // }

    // Close the file after reading
    fclose(file);
}

// Função para alocar um array de 'count' indivíduos
Individual *allocateIndividuals(int count)
{
    Individual *arr = (Individual *)malloc(count * sizeof(Individual));
    if (arr == NULL)
    {
        printf("Erro ao alocar memória para indivíduos.\n");
        exit(1);
    }
    for (int i = 0; i < count; i++)
    {
        // Aloca a matriz de rotas para NUM_VEHICLES veículos,
        // cada rota com (NUM_CLIENTS+1) posições
        arr[i].route = (int **)malloc(NUM_VEHICLES * sizeof(int *));
        if (arr[i].route == NULL)
        {
            printf("Erro ao alocar memória para a rota do indivíduo %d.\n", i);
            exit(1);
        }
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            arr[i].route[j] = (int *)malloc((NUM_CLIENTS + 1) * sizeof(int));
            if (arr[i].route[j] == NULL)
            {
                printf("Erro ao alocar memória para a rota do veículo %d do indivíduo %d.\n", j, i);
                exit(1);
            }
            // Inicializa a rota com -1 para indicar fim da rota
            for (int k = 0; k < NUM_CLIENTS + 1; k++)
            {
                arr[i].route[j][k] = -1;
            }
        }
    }
    return arr;
}

void freeIndividuals(Individual *arr, int count)
{
    for (int i = 0; i < count; i++)
    {
        // Libera cada rota
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            free(arr[i].route[j]);
        }
        free(arr[i].route);
    }
    free(arr);
}

void printSubP(Individual *subPop)
{
    for (int h = 0; h < SUBPOP_SIZE; h++)
    {

        for (int i = 0; i < NUM_VEHICLES; i++)
        {
            printf("  Veículo %d: ", i);
            for (int j = 0; j < NUM_CLIENTS; j++)
            {
                int clientId = subPop[h].route[i][j];
                if (clientId == -1) // Indicador de fim de rota
                    break;
                printf("%d ", clientId);
            }
            printf("\n");
        }
        printf("\n");
    }
}

int main()
{
    printf("Executing!\n");
    srand(time(NULL));

    // Calculating the time spent executing
    double time_spent = 0.0;
    clock_t begin = clock();

    // int solutionFound = 0;
    int i, rouds, idTrack;
    FILE *file;
    double firstFitDist = 0;
    double firstFitTime = 0;
    double firstFitFuel = 0;
    double firstFitWeight = 0;

    Client *clients = NULL;

    // Chamada da função para ler as benchmark
    readBenchmark("solomon/C101.txt", &clients);

    //Verification if the clients array ate properly initialized
    if (clients == NULL)
    {
        printf("Error: clients array is not initialized\n");
        return EXIT_FAILURE;
    }

    // Alocando dinamicamente o currentClientArray
    currentClientArray = (int ***)malloc(NUM_VEHICLES * sizeof(int **));
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        currentClientArray[i] = (int **)malloc(NUM_CLIENTS * sizeof(int *));
    }

    // Realizando outras alocações
    population = allocateIndividuals(POP_SIZE);
    parent = allocateIndividuals(2);
    tournamentIndividuals = allocateIndividuals(QUANTITYSELECTEDTOURNAMENT);
    nextPop = allocateIndividuals(1);
    newSon = allocateIndividuals(1);

    subPopDistance = allocateIndividuals(SUBPOP_SIZE);
    subPopTime = allocateIndividuals(SUBPOP_SIZE);
    subPopFuel = allocateIndividuals(SUBPOP_SIZE);
    subPopCapacity = allocateIndividuals(SUBPOP_SIZE);
    subPopWeighting = allocateIndividuals(SUBPOP_SIZE);

    nextSubPopDistance = allocateIndividuals(SUBPOP_SIZE);
    nextSubPopTime = allocateIndividuals(SUBPOP_SIZE);
    nextSubPopFuel = allocateIndividuals(SUBPOP_SIZE);
    nextSubPopWeighting = allocateIndividuals(SUBPOP_SIZE);

    subpop1 = allocateIndividuals(SUBPOP_SIZE);
    subpop2 = allocateIndividuals(SUBPOP_SIZE);

    populacaoAtual = (int **)malloc(sizeof(int *) * (NUM_CLIENTS));
    populationFitness = (int *)malloc(sizeof(int) * POP_SIZE);

    tournamentFitness = (int *)malloc(sizeof(int) * POP_SIZE);

    distance_clients = (Storage *)malloc(sizeof(Storage) * POP_SIZE);
    for (int h = 0; h < POP_SIZE; h++)
    {
        distance_clients[h].route = (double **)malloc(NUM_VEHICLES * sizeof(double *));
        for (int i = 0; i < NUM_VEHICLES; i++)
        {
            distance_clients[h].route[i] = (double *)malloc(NUM_CLIENTS * sizeof(double));
        }
    }

    time_clients_end = (Storage *)malloc(sizeof(Storage) * POP_SIZE);

    for (i = 0; i < NUM_CLIENTS; i++)
    {
        populacaoAtual[i] = (int *)malloc(sizeof(int) * (NUM_CLIENTS));
    }

    // Verifying the memory allocation
    if (populacaoAtual == NULL || nextPop == NULL || populationFitness == NULL || tournamentIndividuals == NULL || parent == NULL || tournamentFitness == NULL || subPopDistance == NULL || subPopTime == NULL ||
        subPopFuel == NULL || subPopCapacity == NULL || subPopWeighting == NULL || nextSubPopDistance == NULL || nextSubPopTime == NULL || nextSubPopFuel == NULL || nextSubPopWeighting == NULL)
    {
        printf("Fail locating memory!\n");
        return 0;
    }

    //--------------------------------------------------------------------------------------------------------------------
    // Initializating the population
    initPop(population, clients);

    // Distributing the population in subpops
    distributeSubpopulation(population);

    printf("Subpopulacao de ponderacao\n");
    fitness(subPopWeighting, 0, clients);

    exit(0);
    // printf("Subpopulação de distância\n");
    // fitnessDistance(subPopDistance, 0);

    // printf("Subpopulação de tempo\n");
    // printSubP(subPopTime);

    // Calculando o fitness de cada subpopulação
    // for (int i = 0; i < SUBPOP_SIZE; i++)
    //{
    //    fitnessDistance(subPopDistance, i);
    //
    //    fitnessTime(subPopTime, i);
    //
    //    fitnessFuel(subPopFuel, i);
    //
    //    fitness(subPopWeighting, i);
    //}
    //
    //// printf("numero de individuos de ELITE %d\n", ELITISM_SIZE_POP);
    //// printf("numero de individuos da prox poplacao %d\n", SUBPOP_SIZE);
    //
    // file = fopen("output/dataVRP.xls", "a+");
    //
    // int previousHighestFitnessDistanceID = -1;
    // int previousHighestFitnessTimeID = -1;
    // int previousHighestFitnessFuelID = -1;
    // int previousHighestFitnessWeightingID = -1;
    //
    //// Colocando todos os id das nextPop com -1
    //
    //// Resetting all individuals from nextPop
    // for (int i = 0; i < SUBPOP_SIZE; i++)
    //{
    //     nextSubPopDistance[i].id = -1;
    //     nextSubPopTime[i].id = -1;
    //     nextSubPopFuel[i].id = -1;
    //     nextSubPopWeighting[i].id = -1;
    // }
    //
    // idTrack = 1;
    //
    //// printf("FITNESS A BATER\n");
    //// printf("Distancia: %d\n", subPopDistance[0].fitnessDistance);
    //// printf("tempo: %d\n", subPopTime[0].fitnessTime);
    //// printf("Combustivel: %d\n", subPopFuel[0].fitnessFuel);
    //// printf("Ponderacao: %d\n", subPopWeighting[0].fitness);
    //
    // for (int i = 0; i < ROUNDS; i++)
    //{
    //    // printf("Rodada Fora %d\n", i);
    //    evolvePop(rouds, populationFitness, population, newSon, tournamentFitness, tournamentIndividuals, subpop1, subpop2, i, &idTrack, &previousHighestFitnessDistanceID, &previousHighestFitnessTimeID, &previousHighestFitnessFuelID, &previousHighestFitnessWeightingID);
    //
    //    if ((i % 100) == 0)
    //    {
    //        // printf("DISTANCE %.4f \n", subPopDistance[0].fitnessDistance);
    //        // printf("TIME %.4f \n", subPopTime[0].fitnessTime);
    //        // printf("FUEL %.4f \n", subPopFuel[0].fitnessFuel);
    //        printf("POND %.4f \n", subPopWeighting[0].fitness);
    //    }
    //
    //    if (i == 0)
    //    {
    //        firstFitDist = subPopDistance[0].fitnessDistance;
    //        firstFitTime = subPopTime[0].fitnessTime;
    //        firstFitFuel = subPopFuel[0].fitnessFuel;
    //        firstFitWeight = subPopWeighting[0].fitness;
    //    }
    //}
    //
    //// Printaremos todos os individuos de todas as subpopulações ao final, assim conseguiremos ver quais mudaram e quais nao
    ///*
    // */
    // if (file == NULL)
    //{
    //    printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
    //    fclose(file);
    //    return 0;
    //}
    // else
    //{
    //    /*
    //    fprintf(file, "|==================================Subpopulacaoo ao final DISTANCIA==================================|\n");
    //    for (int i = 0; i < SUBPOP_SIZE; i++)
    //    {
    //        fprintf(file, "Individuo %d com id %d e fitness de %.4f\n", i + 1, subPopDistance[i].id, subPopDistance[i].fitnessDistance);
    //        for (int j = 0; j < NUM_VEHICLES; j++)
    //        {
    //            for (int k = 0; k < NUM_CLIENTS; k++)
    //            {
    //                fprintf(file, "%d ", subPopDistance[i].route[j][k]);
    //            }
    //            fprintf(file, "\n");
    //        }
    //        fprintf(file, "\n");
    //    }
    //
    //    fprintf(file, "|==================================Subpopulacaoo ao final Time==================================|\n ");
    //    for (int i = 0; i < SUBPOP_SIZE; i++)
    //    {
    //        fprintf(file, "Individuo %d com id %d e fitness de %.4f\n", i + 1, subPopTime[i].id, subPopTime[i].fitnessTime);
    //        for (int j = 0; j < NUM_VEHICLES; j++)
    //        {
    //            for (int k = 0; k < NUM_CLIENTS; k++)
    //            {
    //                fprintf(file, "%d ", subPopDistance[i].route[j][k]);
    //            }
    //            fprintf(file, "\n");
    //        }
    //        fprintf(file, "\n");
    //    }
    //
    //    fprintf(file, "|==================================Subpopulacaoo ao final Fuel==================================|\n ");
    //    for (int i = 0; i < SUBPOP_SIZE; i++)
    //    {
    //        fprintf(file, "Individuo %d com id %d e fitness de %.4f\n", i + 1, subPopFuel[i].id, subPopFuel[i].fitnessFuel);
    //        for (int j = 0; j < NUM_VEHICLES; j++)
    //        {
    //            for (int k = 0; k < NUM_CLIENTS; k++)
    //            {
    //                fprintf(file, "%d ", subPopFuel[i].route[j][k]);
    //            }
    //            fprintf(file, "\n");
    //        }
    //        fprintf(file, "\n");
    //    }
    //
    //    fprintf(file, "|==================================Subpopulacaoo ao final Weighting==================================|\n");
    //    for (int i = 0; i < SUBPOP_SIZE; i++)
    //    {
    //        fprintf(file, "Individuo %d com id %d e fitness de %.4f\n", i + 1, subPopWeighting[i].id, subPopWeighting[i].fitness);
    //        for (int j = 0; j < NUM_VEHICLES; j++)
    //        {
    //            for (int k = 0; k < NUM_CLIENTS; k++)
    //            {
    //                fprintf(file, "%d ", subPopWeighting[i].route[j][k]);
    //            }
    //            fprintf(file, "\n");
    //        }
    //        fprintf(file, "\n");
    //    }
    //    */
    //}

    // Calculating the average, best fitness and dp from distance
    // double valDistance = 0;
    // double bestDistanceFitness = __INT_MAX__;
    //
    // for (i = 0; i < SUBPOP_SIZE; i++)
    //{
    //    valDistance += subPopDistance[i].fitnessDistance;
    //    if (subPopDistance[i].fitnessDistance < bestDistanceFitness)
    //    {
    //        bestDistanceFitness = subPopDistance[i].fitnessDistance;
    //    }
    //}
    // double media_val_Dist = valDistance / SUBPOP_SIZE;
    //
    // double vDist = 0;
    // double varianciaDist = 0;
    // double dpDist = 0;
    // for (int k = 0; k < SUBPOP_SIZE; k++)
    //{
    //    double desvio = 0;
    //    desvio = subPopDistance[k].fitnessDistance - media_val_Dist;
    //    vDist += pow(desvio, 2);
    //}
    // varianciaDist = vDist / SUBPOP_SIZE;
    // dpDist = sqrt(varianciaDist);
    //
    //// Calculating the average, best fitness and dp from Time
    // double valTime = 0;
    // double bestTimeFitness = __INT_MAX__;
    //
    // for (i = 0; i < SUBPOP_SIZE; i++)
    //{
    //    valTime += subPopTime[i].fitnessTime;
    //    if (subPopTime[i].fitnessTime < bestTimeFitness)
    //    {
    //        bestTimeFitness = subPopTime[i].fitnessTime;
    //    }
    //}
    // double media_val_Time = valTime / SUBPOP_SIZE;
    //
    // double vTime = 0;
    // double varianciaTime = 0;
    // double dpTime = 0;
    // for (int k = 0; k < SUBPOP_SIZE; k++)
    //{
    //    double desvio = 0;
    //    desvio = subPopTime[k].fitnessTime - media_val_Time;
    //    vTime += pow(desvio, 2);
    //}
    // varianciaTime = vTime / SUBPOP_SIZE;
    // dpTime = sqrt(varianciaTime);
    //
    //// Calculating the average, best fitness and dp from Fuel
    // double valFuel = 0;
    // double bestFuelFitness = __INT_MAX__;
    //
    // for (i = 0; i < SUBPOP_SIZE; i++)
    //{
    //    valFuel += subPopFuel[i].fitnessFuel;
    //    if (subPopFuel[i].fitnessFuel < bestFuelFitness)
    //    {
    //        bestFuelFitness = subPopFuel[i].fitnessFuel;
    //    }
    //}
    // double media_val_Fuel = valFuel / SUBPOP_SIZE;
    //
    // double vFuel = 0;
    // double varianciaFuel = 0;
    // double dpFuel = 0;
    // for (int k = 0; k < SUBPOP_SIZE; k++)
    //{
    //    double desvio = 0;
    //    desvio = subPopFuel[k].fitnessFuel - media_val_Fuel;
    //    vFuel += pow(desvio, 2);
    //}
    // varianciaFuel = vFuel / SUBPOP_SIZE;
    // dpFuel = sqrt(varianciaFuel);
    //
    //// Calculating the average, best fitness and dp from Wheighting
    // double valWeighting = 0;
    // double bestWeightingFitness = __INT_MAX__;
    //
    // for (i = 0; i < SUBPOP_SIZE; i++)
    //{
    //    valWeighting += subPopWeighting[i].fitness;
    //    if (subPopWeighting[i].fitness < bestWeightingFitness)
    //    {
    //        bestWeightingFitness = subPopWeighting[i].fitness;
    //    }
    //}
    // double media_val_Weighting = valWeighting / SUBPOP_SIZE;
    //
    // double vWeighting = 0;
    // double varianciaWeighting = 0;
    // double dpWeighting = 0;
    // for (int k = 0; k < SUBPOP_SIZE; k++)
    //{
    //    double desvio = 0;
    //    desvio = subPopWeighting[k].fitness - media_val_Weighting;
    //    vWeighting += pow(desvio, 2);
    //}
    // varianciaWeighting = vWeighting / SUBPOP_SIZE;
    // dpWeighting = sqrt(varianciaWeighting);
    //
    //// time spent executing in seconds
    // clock_t end = clock();
    // time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    // if (file == NULL)
    //{
    //     printf("ERRO AO ABRIR O ARQUIVO PARA SALVAR DADOS DOS TESTES\n");
    //     fclose(file);
    //     return 0;
    // }
    // else
    //{
    //
    //    fprintf(file, "\n");
    //    fprintf(file, "Population Size: %d\n", POP_SIZE);
    //    fprintf(fprintf, "SubPopulations size: %d\n", SUBPOP_SIZE);
    //    fprintf(file, "Number of Vehicles: %d\n", NUM_VEHICLES);
    //    fprintf(file, "Number of Clients: %d\n", NUM_CLIENTS);
    //    fprintf(file, "Vehicle Capacity: %d\n", VEHICLES_CAPACITY);
    //    fprintf(file, "Selection Type: Tournament\n");
    //    fprintf(file, "Crossover Points: %d points\n", CROSSINGTYPE);
    //    fprintf(file, "Mutation Rate: %f\n", MUTATIONRATE);
    //    fprintf(file, "Elitism Rate: %f\n", ELITISMRATE);
    //    fprintf(file, "Rounds: %d\n", ROUNDS);
    //    fprintf(file, "Time: %f\n", time_spent);
    //    // fprintf(file, "O primeiro fitness foi de: %d\n", firstfitness);
    //    fprintf(file, "--------------------Distance------------------\n");
    //    fprintf(file, "A primeiro fitness da subPop Distancia eh: %.4f\n", firstFitDist);
    //    fprintf(file, "A melhor fitness da subPop Distancia eh: %.4f\n", bestDistanceFitness);
    //    fprintf(file, "A media dos fitness da subPop Distancia eh: %.4f\n", media_val_Dist);
    //    fprintf(file, "O desvio Padrao da subPop Distancia eh: %.5f\n", dpDist);
    //    fprintf(file, "--------------------Time------------------\n");
    //    fprintf(file, "A primeiro fitness da subPop Time eh: %.4f\n", firstFitTime);
    //    fprintf(file, "A melhor fitness da subPop Time eh: %.4f\n", bestTimeFitness);
    //    fprintf(file, "A media dos fitness da subPop Time eh: %.4f\n", media_val_Time);
    //    fprintf(file, "O desvio Padrao da subPop Time eh: %.5f\n", dpTime);
    //    fprintf(file, "--------------------Fuel------------------\n");
    //    fprintf(file, "A primeiro fitness da subPop Fuel eh: %.4f\n", firstFitFuel);
    //    fprintf(file, "A melhor fitness da subPop Fuel eh: %.4f\n", bestFuelFitness);
    //    fprintf(file, "A media dos fitness da subPop Fuel eh: %.4f\n", media_val_Fuel);
    //    fprintf(file, "O desvio Padrao da subPop Fuel eh: %.5f\n", dpFuel);
    //    fprintf(file, "--------------------Weighting------------------\n");
    //    fprintf(file, "A primeiro fitness da subPop Weight eh: %.4f\n", firstFitWeight);
    //    fprintf(file, "A melhor fitness da subPop Weighting eh: %.4f\n", bestWeightingFitness);
    //    fprintf(file, "A media dos fitness da subPop Weighting eh: %.4f\n", media_val_Weighting);
    //    fprintf(file, "O desvio Padrao da subPop Weighting eh: %.5f\n", dpWeighting);
    //    // fprintf(file, "Solution Found = %d\n", solutionFound);
    //}
    //
    //// Releasing memory
    //

    freeIndividuals(population, POP_SIZE);
    freeIndividuals(parent, 2);
    freeIndividuals(tournamentIndividuals, QUANTITYSELECTEDTOURNAMENT);
    freeIndividuals(nextPop, 1);
    freeIndividuals(newSon, 1);
    freeIndividuals(subPopDistance, SUBPOP_SIZE);
    freeIndividuals(subPopTime, SUBPOP_SIZE);
    freeIndividuals(subPopFuel, SUBPOP_SIZE);
    freeIndividuals(subPopCapacity, SUBPOP_SIZE);
    freeIndividuals(subPopWeighting, SUBPOP_SIZE);
    freeIndividuals(nextSubPopDistance, SUBPOP_SIZE);
    freeIndividuals(nextSubPopTime, SUBPOP_SIZE);
    freeIndividuals(nextSubPopFuel, SUBPOP_SIZE);
    freeIndividuals(nextSubPopWeighting, SUBPOP_SIZE);
    freeIndividuals(subpop1, SUBPOP_SIZE);
    freeIndividuals(subpop2, SUBPOP_SIZE);

    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        free(currentClientArray[i]);
    }
    free(currentClientArray);

    free(populacaoAtual);
    free(populationFitness);
    free(tournamentFitness);
    free(tournamentIndividuals);

    free(distance_clients);

    for (int i = 0; i < POP_SIZE; i++)
    {
        for (int j = 0; j < NUM_VEHICLES; j++)
        {
            free(distance_clients[i].route[j]);
        }
        free(distance_clients[i].route);
    }

    free(time_clients_end);

    return 0;
}
