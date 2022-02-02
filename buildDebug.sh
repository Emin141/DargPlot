#!/bin/bash

gcc -Wall -Werror -g *.c -I src/include/ -D DEBUG -o DargPlot -lm
