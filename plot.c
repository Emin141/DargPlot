#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "plot.h"
#include "csv_parser.h"
#include "error_codes.h"

#ifdef FREE_WORKING
static int freePlotData(PlotData *plotData)
{
    if (!plotData)
        return ERR_NULL_POINTER;

    free(plotData->xAxis.values);
    free(plotData->yAxis.values);
    free(plotData->zValues);

    plotData->xAxis.values = NULL;
    plotData->yAxis.values = NULL;
    plotData->zValues = NULL;

    return NO_ERROR;
}
#else
static int freePlotData(PlotData *plotData) { return NO_ERROR; }
#endif

static void debug_print(PlotData *plotData)
{
    for (int xIndex = 0; xIndex < plotData->xAxis.numOfValues; xIndex++)
    {
        for (int yIndex = 0; yIndex < plotData->yAxis.numOfValues; yIndex++)
        {
            printf("z(%2$f, %3$f) = %1$f\n",
                   plotData->zValues[yIndex + xIndex * plotData->xAxis.numOfValues].value,
                   plotData->xAxis.values[xIndex],
                   plotData->yAxis.values[yIndex]);
        }
    }
}

int plot(const char *sourceFile)
{
    PlotData plotData = {(Axis){0, NULL}, (Axis){0, NULL}, (ZValue *){NULL}};
    handle_error(parse_csv(sourceFile, &plotData));

    debug_print(&plotData);

    freePlotData(&plotData);
    return NO_ERROR;
}