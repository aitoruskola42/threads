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
            const char *ext = strrchr(argv[2], '.');
            if (!ext || strcmp(ext, ".txt") != 0 || strlen(ext) != 4 || ext == argv[2]) {
                fprintf(stderr, "%s\n", EXT_ERROR);
                return 1;
            }
            FILE *file = fopen(argv[2], "r");
            if (!file) {
                fprintf(stderr, "El fichero no existe o no se puede abrir\n");
                return 1;
            }

            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            if (file_size > 0) {
                char *content = (char *)malloc(file_size + 1);
                fread(content, 1, file_size, file);
                content[file_size] = '\0';
                printf("Contenido del fichero:\n%s\n", content);
                free(content);
            } else {
                printf("El fichero está vacío.\n");
            }

            fclose(file);
        } else {
            fprintf(stderr, "%s\n", ARG_ERROR);
            return 1;
        }
    }

    // Aquí puedes continuar con la lógica para crear hilos si es necesario
    return 0;
}