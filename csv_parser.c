#define MAX_STRING_LENGTH 128
#define EPSILON 1e-6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "csv_parser.h"
#include "error_codes.h"

int get_dimensions(const char* sourceFile, int* xSize, int* ySize);

int parse_csv(const char* sourceFile, PlotData* plotData) {
    int xSize = 0, ySize = 0;

    /* Calls the dimensions reader function */
    handle_error(get_dimensions(sourceFile, &xSize, &ySize));
    plotData->xSize = xSize;
    plotData->ySize = ySize;

    /* Allocates adequate memory for the plot data */
    plotData->z = (double*)malloc(xSize * ySize * sizeof(double));

    /* Reads the file for full parsing */
    FILE* inputFile = NULL;
    if (!(inputFile = fopen(sourceFile, "r"))) {  // Handles error
        return ERR_NO_FILE;
    }

    /* Line buffer */
    size_t lineSize = MAX_STRING_LENGTH;
    char* lineBuffer = (char*)malloc(lineSize);
    if (!lineBuffer) {  // Handles error
        return ERR_NO_MEMORY;
    }

    while (getline(&lineBuffer, &lineSize, inputFile) != EOF) {
        plotData->z[]
    }

    return NO_ERROR;
}

/**************************************************

 First run determines the dimensions of the plot

**************************************************/
int get_dimensions(const char* sourceFile, int* xSize, int* ySize) {
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

    /* Determines the number of rows in the source file
    since the first line contains the name of variables, the
    numRows container starts from -1 */
    int numRows = -1;
    while (getline(&lineBuffer, &lineSize, inputFile) != EOF) {
        if (lineBuffer[0] != '\n') numRows++;
    }
    rewind(inputFile);

    /* The y dimension is the number of rows read when
    the value of x changes for the first time. The x dimension
    is the number of rows read until the end of the file divided
    by the y dimension value */

    double xPrevious = 0.0f, xCurrent = 0.0f;

    // Skips the first line, because it contains the variable names
    getline(&lineBuffer, &lineSize, inputFile);

    // Uses the next line as the base case
    int condition = getline(&lineBuffer, &lineSize, inputFile);
    xPrevious = strtod(lineBuffer, NULL);
    (*ySize)++;

    while (condition != EOF) {
        condition = getline(&lineBuffer, &lineSize, inputFile);
        xCurrent = strtod(lineBuffer, NULL);
        if (fabs(xCurrent - xPrevious) > EPSILON) {
            condition = getline(&lineBuffer, &lineSize, inputFile);
        } else {
            xPrevious = xCurrent;
            (*ySize)++;
        }
    }
    *xSize = numRows / *ySize;

#ifdef _DEBUG
    printf("numRows = %d, xSize = %d, ySize = %d\n", numRows, *xSize, *ySize);
#endif
    fclose(inputFile);
    return NO_ERROR;
}