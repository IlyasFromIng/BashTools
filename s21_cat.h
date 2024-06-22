#ifndef S21_CAT_H
#include <stdio.h>
typedef struct cat_opt {
    int b, e, n, s, t, v, error;
} cat_opt;

cat_opt put_opt(int argc, char** argv);
void print_file(FILE*, const cat_opt*);
int get_line(unsigned char** line, unsigned long* len, FILE* file);
void print_line(unsigned char* line, unsigned long len, const cat_opt* opt);

#endif