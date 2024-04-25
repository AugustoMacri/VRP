#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "selection.h"

/*
    -----------------------------------
               Selection
    -----------------------------------

    - In this part of the code we will create the selection of the population;
    - We will use Roulette selection (just like in Stanley's implementation) and Tournament selection;
    - So, we're gona have one function for each of these selection;
    - rouletteSelection();
    - tournamentSelection();

*/

/*
    -----------------------------------
            rouletteSelection()
    -----------------------------------

    - The first selection will be Roulette selection;
    - In this selection we will select two individuals from the population;
    - Individuals will be chosen according to their fitness, the higher it is, the more likely they are to be chosen.

*/

void rouletteSelection(int **parent, int *populationFitness, int **populacaoAtual)
{
    int i, j, k;
    int sumFitness = 0;
    int numSort, fitnessAcumulated, cont;

    // First of all we will sum the fitness of all population
    for (i = 0; i < POP_SIZE; i++)
    {
        sumFitness += populationFitness[i];
        // printf("%d ", populationFitness[i]);
        // printf("%d ", sumFitness); está fazendo a soma de dois fitness corretamente como deveria
    }

    // Calcular as probabilidades de seleção para cada indivíduo
    double *probabilidade = (double *)malloc(POP_SIZE * sizeof(double));
    for (i = 0; i < POP_SIZE; i++)
    {
        probabilidade[i] = (double)populationFitness[i] / sumFitness;
        // printf("%.2f ", probabilidade[i]);
    }

    // Selecionar dois pais para o cruzamento
    for (i = 0; i < 2; i++)
    {
        cont = 0;

        // Girar a roleta
        numSort = rand() % sumFitness; // gera um intervalo de 0 a sumFitness
        fitnessAcumulated = 0;

        // Seleção de um dos pais
        for (j = 0; j < NUM_VEHICLES; j++)
        {
            fitnessAcumulated += populationFitness[j];
            if (fitnessAcumulated > numSort)
            {
                // Armazenar o cromossomo do pai selecionado
                for (k = 0; k < NUM_CLIENTS + 1; k++)
                {
                    parent[i][k] = populacaoAtual[j][k];
                    printf("poppulacaoAtual[%d][%d] = %d\n", j, k, populacaoAtual[j][k]);
                }
                break; // Sai do loop após encontrar o pai
            }
        }

        if (i > 0 && memcmp(parent[0], parent[1], (NUM_CLIENTS + 1) * sizeof(int)) == 0)
        {
            i--; // Selecionar outro pai diferente
        }
    }

    for (i = 0; i < 2; i++)
    {
        printf("pai numero %d: ", i + 1);
        for (j = 0; j < NUM_CLIENTS + 1; j++)
        {
            printf("%d ", parent[i][j]);
        }
        printf("\n");
    }
}

/*
    -----------------------------------
            tournamentSelection()
    -----------------------------------

    - The second selection will be Tournament selection;
    - In this selection we will select two individuals from the population;
    - Individuals will be chosen randomly in a population;
    - After they are chosen, the best (greater fitness) will be selected.

*/

void tournamentSelection(int **tournamentIndividuals, int **parent, int *tournamentFitness, int *populationFitness, int **populacaoAtual)
{
    int h, i, j, k;
    int individual[QUANTITYSELECTED];
    int cont = 0;
    int tournamentSize = 5;

    // Loop para cada pai a ser selecionado
    for (i = 0; i < 2; i++)
    {
        // Loop para realizar o torneio e selecionar o vencedor
        int vencedorTorneio = -1;
        int maiorAptidao = -1;

        for (j = 0; j < tournamentSize; j++)
        {
            // Seleciona aleatoriamente um indivíduo da população
            int participanteTorneio = rand() % POP_SIZE;

            // Avalia a aptidão do participante do torneio
            int aptidaoParticipante = populationFitness[participanteTorneio]; //na lógica teria que acessar o valor do fitness da população

            // Atualiza o vencedor do torneio se a aptidão for maior
            if (aptidaoParticipante > maiorAptidao)
            {
                maiorAptidao = aptidaoParticipante;
                vencedorTorneio = participanteTorneio;
            }
        }

        // Armazena o cromossomo do vencedor do torneio como pai selecionado
        for (k = 0; k < NUM_CLIENTS + 1; k++)
        {
            parent[i][k] = populacaoAtual[vencedorTorneio][k];
            printf("poppulacaoAtual[%d][%d] = %d\n", vencedorTorneio, k, populacaoAtual[vencedorTorneio][k]);
        }
    }
}