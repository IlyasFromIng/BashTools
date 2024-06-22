#define _GNU_SOURCE
#include "s21_grep.h"
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    arg_t opt = parse_arguments(argc, argv);
    if (!opt.error) {
        //if (!opt.e && !opt.f) patte
        for(int i = optind; i < argc; ++i) {


            FILE* file = fopen(argv[++i], "r");

            if (file == NULL) {
                printf("No such file or directory\n");
                exit(EXIT_FAILURE);
            } 

            print_file(file, argv, opt);
            fclose(file);
            
        }
    } else {
        fprintf(stderr, "Usage: %s [OPTION]... PATTERNS [FILE]...\n", argv[0]);
    }

    fflush(stderr);
    return 0;
}

void print_file(FILE* file, char** argv, arg_t opt){
    char* line = NULL;
    size_t len = 0;
    char* word = argv[optind];
    while(getline(&line, &len, file) != -1) {
        if (opt.i && *line != '\n') arg_i(line, word);
    }
    free(line);
}


void arg_i(char* line, char* word) {
    //приведение паттерна к нижнему регистру
    for (size_t i = 0; word[i] != '\0'; ++i) {
        word[i] = tolower(word[i]);
    }
    //сохранение оригинальной строки
    char* original_line = strdup(line);
    //приведение строки к нижнему регистру
    for( size_t i = 0; line[i] != '\n'; ++i) {
        line[i] = tolower(line[i]);
    }
    //поиск слова и печать
    char* find;
    if ((find = strstr(line, word)) != NULL) {
        printf("%s\n", original_line);
    } 
    free(original_line);
}

arg_t parse_arguments(int argc, char** argv) {
    arg_t opt = {0};

    const char* short_options = "eivclnhsfo";

    const struct option long_options[] = {
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
        {0, 0, 0, 0}
    };

    int tmp_opt = 0;

    while((tmp_opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        switch (tmp_opt){
            case 'e':
                opt.e = 1;
                break;
            case 'i':
                opt.i = 1;
                break;
            case 'v':
                opt.v = 1;
                break;
            case 'c':
                opt.c = 1;
                break;
            case 'l':
                opt.l = 1;
                break;
            case 'n':
                opt.n = 1;
                break;
            case 'h':
                opt.h = 1;
                break;
            case 's':
                opt.s = 1;
                break;
            case 'f':
                opt.f = 1;
                break;
            case 'o':
                opt.o = 1;
                break;
            default:
                opt.error = 1; 
                break;
        }
    }

    return opt;
}