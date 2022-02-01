#!/bin/bash

gcc -Wall -Werror -g src/*.c -I src/include/ -D _DEBUG -o DargPlot -lm
