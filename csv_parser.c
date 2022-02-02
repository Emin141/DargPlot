#define MAX_STRING_LENGTH 128
#define EPSILON 1e-6

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "csv_parser.h"
#include "error_codes.h"

/**************************************************

 First run determines the dimensions of the plot

**************************************************/
static int get_dimensions(const char* sourceFile, int* xSize, int* ySize) {
    (*xSize) = 0;
    (*ySize) = 0;

    FILE* inputFile = NULL;
    if (!(inputFile = fopen(sourceFile, "r"))) {  // Handles error
        return ERR_NO_FILE;
    }

    // Line buffer
    size_t lineSize = MAX_STRING_LENGTH;
    char* lineBuffer = (char*)malloc(lineSize * sizeof(char));
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

int parse_csv(const char* sourceFile, PlotData* plotData) {
    /* Calls the dimensions reader function */
    handle_error(get_dimensions(sourceFile, &(plotData->xAxis.numOfValues),
                                &(plotData->yAxis.numOfValues)));

    /* Allocates adequate memory for the plot data */
    /* x axis allocation */
    plotData->xAxis.values =
        (double*)malloc(plotData->xAxis.numOfValues * sizeof(double));
    if (!(plotData->xAxis.values)) return ERR_NO_MEMORY;

    /* y axis allocation */
    plotData->yAxis.values =
        (double*)malloc(plotData->yAxis.numOfValues * sizeof(double));
    if (!(plotData->yAxis.values)) return ERR_NO_MEMORY;

    /* z values allocation */
    plotData->zValues =
        (ZValue*)malloc(plotData->xAxis.numOfValues *
                        plotData->yAxis.numOfValues * sizeof(ZValue));
    if (!(plotData->zValues)) return ERR_NO_MEMORY;

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

    /* The algorithm has to run three times.
    First time it will acquire the x values, in the second run
    it will acquire the y values, and in the final run it will
    aqcuire the z values. This has to be done this way because of
    the specific way CSV files are organized */

    /* Each run is inside a block, so that the variables can be dealloced */
    { /* x run */
        getline(&lineBuffer, &lineSize,
                inputFile); /* Skips the variable names */
        getline(&lineBuffer, &lineSize, inputFile);  /* Gets the first line */
        double xPrevious = strtod(lineBuffer, NULL); /* Gets the first value */
        double xCurrent = 0.0f;
        int xIndex = 0;
        plotData->xAxis.values[xIndex++] = xPrevious;
        while (getline(&lineBuffer, &lineSize, inputFile) != EOF) {
            xCurrent = strtod(lineBuffer, NULL);
            if (fabs(xCurrent - xPrevious) > EPSILON) {
                plotData->xAxis.values[xIndex++] = xCurrent;
            }
            xPrevious = xCurrent;
        }
        rewind(inputFile);
    }
    { /* y run */
        getline(&lineBuffer, &lineSize,
                inputFile); /* Skips the variable names */
        getline(&lineBuffer, &lineSize, inputFile); /* Gets the first line */
        double xPrevious =
            strtod(lineBuffer, &lineBuffer); /* Gets the first x value */
        double xCurrent = 0.0f;
        int yIndex = 0;
        plotData->yAxis.values[yIndex++] = strtod(lineBuffer + 1, NULL);
        while (1) {
            getline(&lineBuffer, &lineSize, inputFile);
            xCurrent = strtod(lineBuffer, &lineBuffer);
            plotData->yAxis.values[yIndex++] = strtod(lineBuffer + 1, NULL);
            if (fabs(xCurrent - xPrevious) > EPSILON) {
                break;
            }
            xPrevious = xCurrent;
        }
        rewind(inputFile);
    }
    { /* z run */
        getline(&lineBuffer, &lineSize,
                inputFile); /* Skips the variable names */
        for (int xIndex = 0; xIndex < plotData->xAxis.numOfValues; xIndex++) {
            for (int yIndex = 0; yIndex < plotData->yAxis.numOfValues;
                 yIndex++) {
                getline(&lineBuffer, &lineSize, inputFile);
                /* Skips the x and y values in the line */
                strtod(lineBuffer, &lineBuffer);
                strtod(lineBuffer + 1, &lineBuffer);
                /* Assigns the values of xIndex, yIndex, and z to the zValue
                 * member */
                plotData->zValues[yIndex + xIndex * plotData->xAxis.numOfValues]
                    .value = strtod(lineBuffer + 1, &lineBuffer);
                plotData->zValues[yIndex + xIndex * plotData->xAxis.numOfValues]
                    .xIndex = xIndex;
                plotData->zValues[yIndex + xIndex * plotData->xAxis.numOfValues]
                    .yIndex = yIndex;
            }
        }
    }

    // fclose(inputFile);
    return NO_ERROR;
}
