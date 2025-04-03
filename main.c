#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  

#include "main.h"

void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
void print_content(const char *content);


void handle_arguments(int argc, char *argv[], char **filename) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "%s\n", ARG_ERROR);
        exit(1);
    }
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            fprintf(stderr, "%s\n", HELP_MESSAGE);
            exit(0);
        } else {
            fprintf(stderr, "%s\n", ARG_ERROR);
            exit(1);
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) {
            *filename = argv[2];
            const char *ext = strrchr(*filename, '.');
            if (!ext || strcmp(ext, ".txt") != 0 || strlen(ext) != 4 || *filename == ext) {
                fprintf(stderr, "%s\n", EXT_ERROR);
                exit(1);
            }
        } else {
            fprintf(stderr, "%s\n", ARG_ERROR);
            exit(1);
        }
    }
}

char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "%s\n", FILE_NOT_FOUND_ERROR);
        return NULL;
    }

    struct stat file_stat;
    if (fstat(fileno(file), &file_stat) != 0) {
        fprintf(stderr, "%s\n", FILE_NOT_FOUND_ERROR);
        fclose(file);
        return NULL;
    }

    long file_size = file_stat.st_size;

    if (file_size > 0) {
        char *content = (char *)malloc(file_size + 1);
        fread(content, 1, file_size, file);
        content[file_size] = '\0';
        fclose(file);
        return content;
    } else {
        fclose(file);
        fprintf(stderr, "%s\n", FILE_EMPTY_MESSAGE);
        return NULL;
    }
}

void print_content(const char *content) {
    printf("Contenido del fichero:\n%s\n", content);
}


int main(int argc, char *argv[]) {
    char *filename = NULL;
    handle_arguments(argc, argv, &filename);

    char *content = read_file(filename);
    if (content) {
        print_content(content);
        free(content);
    }

    return 0;
}
