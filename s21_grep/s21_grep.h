#ifndef S21_GREP_H

#include <stdio.h>

typedef struct arguments {
    int e, i, v, c, l, n, h, s, f, o, error;
} arg_t;

typedef struct {
    char **data;
    int size, capacity;
} templates_t;

void parse_arguments(int, char **, arg_t *, templates_t *);
void get_memory(templates_t* templates, arg_t* opt);
void pars_for_e(templates_t* templates, arg_t* opt);

void print_file(FILE *, char **, arg_t);
void arg_i(char *line, char *word, int *);
#endif