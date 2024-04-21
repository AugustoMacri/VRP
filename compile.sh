#!/bin/bash

gcc -Wall -Wextra -g3 \
    src/main.c \
    src/fitness.c \
    src/selection.c \
    src/crossing.c \
    src/mutation.c \
    src/ordering.c \
    src/population.c \
    src/print.c \
    src/initialization.c \
    -o output/main \
    -Iinclude \
    -lm
