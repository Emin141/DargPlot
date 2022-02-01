#define MAX_STRING_LENGTH 128
#define EPSILON 1e-8

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "include/csv_parser.h"
#include "include/error_codes.h"

int parse_csv(const char* sourceFile, PlotData* plotData) {
    FILE* inputFile = NULL;
    if (!(inputFile = fopen(sourceFile, "r"))) {  // Handles error
        return ERR_NO_FILE;
    }

    // Line buffer
    size_t lineSize = MAX_STRING_LENGTH;
    char* lineBuffer = (char*)malloc(lineSize);
    if (!lineBuffer) {  // Handles error
        return ERR_NO_MEMORY;
    }

    int xSize = 0, ySize = 0;
    // First run determines the dimensions of the plot

    /* The y dimension is the number of rows read when
    the value of x changes for the first time. The x dimension
    is the number of rows read until the end of the file divided
    by the y dimension value */

    double xPrevious = 0.0f, xCurrent = 0.0f;
    int numRows = 0;
    while (fabs(xPrevious - xCurrent) > EPSILON) {
        getline(&lineBuffer, &lineSize, inputFile);
        xCurrent = strtod(lineBuffer, NULL);
        numRows++, ySize++;

        getline(&lineBuffer, &lineSize, inputFile);
        xPrevious = strtod(lineBuffer, NULL);
        numRows++, ySize++;
    }
    printf("ySize = %d", ySize);

LoopEscape:

    return NO_ERROR;
}
