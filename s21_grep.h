#ifndef S21_GREP

#include <stdio.h>

typedef struct arguments {
    int e, i, v, c, l ,n, h, s, f, o, error;
} arg_t;

arg_t parse_arguments(int, char**);

void print_file(FILE*, char**, arg_t);
void arg_i(char* line, char* word);
#endif