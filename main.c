#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  

#include "main.h"

void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
void print_content(const char *content, int numbers_per_thread, int thread_num);
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num);


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
        char *content = (char *)malloc(file_size + 2);
        fread(content, 1, file_size, file);
        content[file_size] = '\0';
        fclose(file);

        if (content[file_size - 1] != '\n') {
            content[file_size] = '\n';
            content[file_size + 1] = '\0';
        } else {
            content[file_size] = '\0';
        }
        return content;
    } else {
        fclose(file);
        fprintf(stderr, "%s\n", FILE_EMPTY_MESSAGE);
        return NULL;
    }
}

void print_content(const char *content, int numbers_per_thread, int thread_num) {
    printf("Contenido del fichero:\n%s\n", content);
    printf("Las variables obtenidas son:\n");
    printf("Número de hilos: %d\n", numbers_per_thread);
    printf("Número de threads: %d\n", thread_num);
}

int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num) {
    const char *required_keys[] = {"numbers_per_thread", "thread_num"};
    int keys_found[2] = {0, 0};

    char *content_copy = strdup(content);
    char *line = strtok(content_copy, "\n");
    while (line != NULL) {
        for (int i = 0; i < 2; i++) {
            if (strncmp(line, required_keys[i], strlen(required_keys[i])) == 0) {
                char *equals_sign = strchr(line, '=');
                if (equals_sign && *(equals_sign + 1) != '\0') {
                    keys_found[i] = 1;
                    if (i == 0) {
                        *numbers_per_thread = atoi(equals_sign + 1);
                    } else if (i == 1) {
                        *thread_num = atoi(equals_sign + 1);
                    }
                }
                break;
            }
        }
        line = strtok(NULL, "\n");
    }

    free(content_copy);
    return keys_found[0] && keys_found[1];
}

int main(int argc, char *argv[]) {
    char *filename = NULL;
    handle_arguments(argc, argv, &filename);

    char *content = read_file(filename);
    printf("Contenido del fichero1:\n%s\n", content);
    if (content) {
        int numbers_per_thread = 0;
        int thread_num = 0;
        if (validate_key_value_format(content, &numbers_per_thread, &thread_num)) {
            print_content(content, numbers_per_thread, thread_num);
        } else {
            fprintf(stderr, "El fichero no tiene el formato correcto de clave-valor.\n");
        }
        free(content);
    }

    return 0;
}
