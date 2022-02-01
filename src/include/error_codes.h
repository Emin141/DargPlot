#ifndef ERROR_CODES_HEADER_INCLUDED
#define ERROR_CODES_HEADER_INCLUDED

#define NO_ERROR 0
#define ERR_NO_FILE 1
#define ERR_NO_MEMORY 2

#include <stdlib.h>

static void handle_error(const int errorCode) {
    fprintf(stderr, "Plotting failed with error code %d.\n", errorCode);
    // error values are explained at the top of the header
    exit(EXIT_FAILURE);
}

#endif  // ERROR_CODES_HEADER_INCLUDED