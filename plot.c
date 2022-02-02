#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plot.h"
#include "csv_parser.h"
#include "error_codes.h"

int freePlotData(PlotData* plotData) {
    if (!plotData) return ERR_NULL_POINTER;

    free(plotData->xAxis.values);
    free(plotData->yAxis.values);
    free(plotData->zValues);

    return NO_ERROR;
}

int plot(const char* sourceFile) {
    PlotData plotData = {(Axis){0, NULL}, (Axis){0, NULL}, (ZValue*){NULL}};
    handle_error(parse_csv(sourceFile, &plotData));

#ifdef DEBUG
    for (int i = 0; i < plotData.xAxis.numOfValues; i++) {
        printf("%g ", plotData.xAxis.values[i]);
    }
    puts("");
    for (int i = 0; i < plotData.yAxis.numOfValues; i++) {
        printf("%g ", plotData.yAxis.values[i]);
    }
    puts("");
    for (int xIndex = 0; xIndex < plotData.xAxis.numOfValues; xIndex++) {
        for (int yIndex = 0; yIndex < plotData.yAxis.numOfValues; yIndex++) {
            printf(
                "z[%2$d][%3$d] = %1$g\n",
                plotData.zValues[yIndex + xIndex * plotData.xAxis.numOfValues]
                    .value,
                plotData.zValues[yIndex + xIndex * plotData.xAxis.numOfValues]
                    .xIndex,
                plotData.zValues[yIndex + xIndex * plotData.xAxis.numOfValues]
                    .yIndex);
        }
    }
#endif
    return NO_ERROR;
}