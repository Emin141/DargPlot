#include <stdio.h>
#include <stdlib.h>

#include "plot.h"
#include "error_codes.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "No input file specified.\n");
        return EXIT_FAILURE;
    }

    // Plots the specified csv file
    handle_error(plot(argv[1]));

    return EXIT_SUCCESS;
}