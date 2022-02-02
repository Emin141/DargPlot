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
    int error = parse_csv(sourceFile, &plotData);
    if (error) handle_error(error);

    return NO_ERROR;
}