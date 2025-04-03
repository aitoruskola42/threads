#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

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
