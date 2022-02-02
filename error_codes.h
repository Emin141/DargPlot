#ifndef ERROR_CODES_HEADER_INCLUDED
#define ERROR_CODES_HEADER_INCLUDED

#define NO_ERROR 0
#define ERR_NO_FILE 1
#define ERR_NO_MEMORY 2
#define ERR_NULL_POINTER 3

#include <stdlib.h>

void handle_error(const int errorCode);

#endif  // ERROR_CODES_HEADER_INCLUDED