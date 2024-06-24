#include "s21_grep.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct grep_opt
{
    int pattern,
     ignore_up_low,
     invert,
     count_lines,
     count_files,
     num_a_line,
     no_filename,
     without_error_gost_item,
     take_regexes,
     show_target_in_line,
     error;
} grep_opt;

grep_opt pars_argument(int, char**);
void print_file(FILE* file, grep_opt* opt);
int get_line(unsigned char** line, unsigned long* len, FILE* file);


int main(int argc, char** argv) {
    grep_opt opt = pars_argument(argc, argv);

    if (opt.error == 0) {
        for (int i = optind; i < argc; ++i) {
            FILE* file = fopen(argv[i], "r");
            if (file == NULL) {
                fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
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

void print_file(FILE* file, grep_opt* opt) {
    unsigned long len = 0;
    unsigned char* line = NULL;
    int num_line = 1, empty_line = 0;

    while (get_line(&line, &len, file) != -1) {
        
    }
    free(line);
    fflush(stdout);
}

grep_opt pars_argument(int argc, char** argv) {
    grep_opt opt = {0};

    const char short_options[] = "eivclnhsfo";


    struct option long_options[] = {
        {"regexp", 0, NULL, 'e'},
        {"ignore-case", 0, NULL, 'i'},
        {"invert-match", 0, NULL, 'v'},
        {"count", 0, NULL, 'c'},
        {"files-with-matches", 0, NULL, 'l'},
        {"line-number", 0, NULL, 'n'},
        {"no-filename", 0, NULL, 'h'},
        {"no-messages", 0, NULL, 's'},
        {"file", 0, NULL, 'f'},
        {"only-matching", 0, NULL, 'o'},
        {0,0,0,0}
    };
    
    int op = 0;

    while ((op = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (op) {
            case 'e':
                opt.pattern = 1;
                break;
            case 'i':
                opt.ignore_up_low = 1;
                break;
            case 'v':
                opt.invert = 1;
                break;
            case 'c':
                opt.count_lines = 1;
                break;
            case 'l':
                opt.count_files = 1;
                break;
            case 'n':
                opt.num_a_line = 1;
                break;
            case 'h':
                opt.no_filename = 1;
                break;
            case 's':
                opt.without_error_gost_item = 1;
                break;
            case 'f':
                opt.take_regexes = 1;
                break;
            case 'o':
                opt.show_target_in_line = 1;
                break;
            default:
                opt.error = 1;
                break;
        }
    }
    
    return opt;
}