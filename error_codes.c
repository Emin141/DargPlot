#include <stdio.h>
#include "error_codes.h"

void handle_error(const int errorCode) {
    if (errorCode) {
        fprintf(stderr, "Plotting failed with error code %d.\n", errorCode);
        exit(EXIT_FAILURE);
    }
}
