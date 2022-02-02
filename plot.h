#ifndef PLOT_HEADER_INCLUDED
#define PLOT_HEADER_INCLUDED

/* Plot data object. This object contains the dimensions of the plot,
as well as the values of z.

****************************************************************
                            IMPORTANT:

Since the return values of functions in this program are used for error
handling, the returned PlotData from parsing the csv file should be passed as an
out parameter.

****************************************************************

*/

typedef struct Axis_S {
    int numOfValues;
    double* values;
} Axis;

typedef struct ZValue_S {
    int xIndex, yIndex;
    double value;
} ZValue;

typedef struct PlotData_S {
    Axis xAxis, yAxis;
    ZValue* zValues;
} PlotData;

/* Plot function */
int plot(const char* sourceFile);

#endif  // PLOT_HEADER_INCLUDED