#!/bin/bash

# Loop para rodar 10 vezes
for ((i = 1; i <= 10; i++)); do
    echo "Rodando a $iª vez:"
    ./output/main.exe
    echo "-------------------------"
done
