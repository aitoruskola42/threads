#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "%s\n", ARG_ERROR);
        return 1;
    }
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            fprintf(stderr, "%s\n", HELP_MESSAGE);
            return 0;
        } else {
            fprintf(stderr, "%s\n", ARG_ERROR);
            return 1;
        }
    }
    if (argc == 3) {
        if (strcmp(argv[1], "-f") == 0 || strcmp(argv[1], "--file") == 0) {
            printf("Nombre del fichero: %s\n", argv[2]);
        } else {
            fprintf(stderr, "%s\n", ARG_ERROR);
            return 1;
        }
    }

    // Aquí puedes continuar con la lógica para crear hilos si es necesario
    return 0;
}