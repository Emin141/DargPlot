#include <stdio.h>
#include <string.h>

#include "plot.h"
#include "csv_parser.h"
#include "error_codes.h"

int plot(const char* sourceFile) {
    PlotData plotData;
    int error = parse_csv(sourceFile, &plotData);
    if (error) handle_error(error);

    return NO_ERROR;
}