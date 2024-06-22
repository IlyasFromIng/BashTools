#include "s21_cat.h"

#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    cat_opt opt = put_opt(argc, argv);

    if (!opt.error) {
        for (int i = optind; i < argc; ++i) {
            FILE* file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "cat: %s: No such file or directory\n", argv[i]);
            } else {
                print_file(file, &opt);

                fclose(file);
            }
        }
    }

    return 0;
}

int get_line(unsigned char** line, unsigned long* len, FILE* file) {
    unsigned char c;
    unsigned long capacity = 4, size = 0;

    if (*line != NULL) free(*line);
    *line = (unsigned char*)calloc(capacity, sizeof(unsigned char));

    while ((c = fgetc(file)) != '\n' && !feof(file) && !ferror(file)) {
        if (size + 2 == capacity) {
            capacity *= 2;
            unsigned char* tmp = calloc(capacity, sizeof(unsigned char));

            for (unsigned long i = 0; i < size; ++i) {
                tmp[i] = (*line)[i];
            }
            free(*line);
            *line = tmp;
        }
        (*line)[size] = c;
        ++size;
    }
    if (c == '\n') {
        (*line)[size++] = c;
    }
    int result = 0;
    if (size == 0 && (feof(file) || ferror(file))) {
        result = -1;
    }
    *len = size;
    return result;
}

void print_non_visible(unsigned char c) {
    if (c == 127) {
        printf("^?");
    } else if (c == 255) {
        printf("M-^?");
    } else if (iscntrl(c)) {
        printf("^%c", c + 64);
    } else if (c > 127 && c < 160) {
        printf("M-^%c", c - 64);
    } else if (c >= 159) {
        printf("M-%c", c - 128);
    } else {
        printf("%c", c);
    }
}

void print_line(unsigned char* line, unsigned long len, const cat_opt* opt) {
    for (unsigned long i = 0; i < len; ++i) {
        if (opt->e && line[i] == '\n') {
            printf("$");
        }
        if (opt->t && line[i] == '\t') {
            printf("^I");
        } else if ((opt->v && !isprint(line[i])) && line[i] != '\n' && line[i] != '\t') {
            print_non_visible(line[i]);
        } else {
            printf("%c", line[i]);
        }
    }
}

void print_file(FILE* file, const cat_opt* opt) {
    unsigned long len = 0;
    unsigned char* line = NULL;
    int num_line = 1, empty_line = 0;

    while (get_line(&line, &len, file) != -1) {
        if (opt->n || (opt->b && line[0] != '\n')) {
            printf("%6.d\t", num_line);
            num_line++;
        }
        if (opt->s && line[0] == '\n' && empty_line) {
            continue;
        }
        print_line(line, len, opt);
        if (line[0] == '\n') {
            empty_line = 1;
        } else {
            empty_line = 0;
        }
    }
    free(line);
    fflush(stdout);
}

cat_opt put_opt(int argc, char** argv) {
    cat_opt opt = {0};

    const char short_opt[] = "bevnstET";

   

    struct option long_opt[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                {"number", no_argument, NULL, 'n'},
                                {"squeeze-blank", no_argument, NULL, 's'},
                                {0, 0, 0, 0}};

    int op = 0;

    while ((op = getopt_long(argc, argv, short_opt, long_opt, NULL)) != -1) {
        switch (op) {
            case 'b':
                opt.b = 1;
                opt.n = 0;
                break;
            case 'e':
                opt.e = 1;
                opt.v = 1;
                break;
            case 's':
                opt.s = 1;
                break;
            case 'n':
                opt.n = !opt.b;
                break;
            case 't':
                opt.t = 1;
                opt.v = 1;
                break;
            case 'v':
                opt.v = 1;
                break;
            case 'T':
                opt.t = 1;
                break;
            case 'E':
                opt.e = 1;
                break;
            default:
                opt.error = 1;
        }
    }

    return opt;
}
