#define _GNU_SOURCE
#include "s21_grep.h"

#include <getopt.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    arg_t opt = {0};
    templates_t templates;
    templates.capacity = 2;
    templates.data = malloc(sizeof(char*) * templates.capacity); 
    if (templates.data == NULL) {
        printf("Не получилось выделить память");
    } else {
        parse_arguments(argc, argv, &opt, &templates);
        printf("%s\n", templates.data[0]);
        if (!opt.error) {

        }
        if (templates.data != NULL) {
            for (size_t i = 0; i < templates.size; ++i) {
                free(templates.data[i]);
            }
        }
        free(templates.data);
    }


    return 0;
}
#if 0
void print_file(FILE* file, char** argv, arg_t opt){
    char* line = NULL;
    size_t len = 0;
    char* word = argv[optind];
    int empty_line = 0;

    while(getline(&line, &len, file) != -1) {
        if (opt.i  && *line != '\0') arg_i(line, word, &empty_line);


        if (!empty_line && *line != '\n'){
        printf("%s", line);
        }
        empty_line = 0;
    }
    free(line);
}


void arg_i(char* line, char* word, int* empty_line) {
    //приведение паттерна к нижнему регистру
    for (size_t i = 0; word[i] != '\0'; ++i) {
        word[i] = tolower(word[i]);
    }
    //сохранение оригинальной строки и паттерна
    char* original_line = strdup(line);
    char* original_word = strdup(word);
    //приведение строки к нижнему регистру
    for( size_t i = 0; line[i] != '\n'; ++i) {
        line[i] = tolower(line[i]);
    }
    //поиск слова и печать
    char* find;
    if ((find = strstr(line, word)) != NULL) {
        line = original_line;
    } else {
        line = original_line;
        *empty_line = 1;
        }
    word = original_word;

    free(original_line);
    free(original_word);
}
#endif

void parse_arguments(int argc, char **argv, arg_t *opt, templates_t *templates) {
    const char *short_options = "e:ivclnhsf:o";
    int tmp_opt = 0;

    while ((tmp_opt = getopt(argc, argv, short_options)) != -1) {
        switch (tmp_opt) {
            case 'e':
                opt->e = 1;
                pars_for_e(templates, opt);
                break;
            case 'i':
                opt->i = 1;
                break;
            case 'v':
                opt->v = 1;
                break;
            case 'c':
                opt->c = 1;
                break;
            case 'l':
                opt->l = 1;
                break;
            case 'n':
                opt->n = 1;
                break;
            case 'h':
                opt->h = 1;
                break;
            case 's':
                opt->s = 1;
                break;
            case 'f':
                opt->f = 1;
                // pars_for_f(templates, opt);
                break;
            case 'o':
                opt->o = 1;
                break;
            default:
                opt->error = 1;
                break;
        }
    }
}

void get_memory(templates_t* templates, arg_t* opt) {
    if (templates->size + 1 == templates->capacity) {
        templates->capacity *= 2;
        char** tmp = malloc(sizeof(char*) * templates->capacity);
        if (tmp == NULL) {
            opt->error = 1;
        } else {
            for (size_t i = 0; i < templates->size; ++i) {
                tmp[i] = templates->data[i];
            }
            free(templates->data);
            templates->data = tmp;
        }
    }
}

void pars_for_e(templates_t* templates, arg_t* opt) {
    get_memory(templates, opt);
    int length = strlen(optarg);
    templates->data = calloc(length + 1, sizeof(char));
    if (templates->data == NULL) {
        opt-> error = 1;
    } else {
        strcpy(templates->data[templates->size++], optarg);
        
    }
     
}

