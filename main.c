#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  

#include "main.h"

void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
void print_content(const char *content, int numbers_per_thread, int thread_num);
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num);


void print_content(const char *content, int numbers_per_thread, int thread_num) {
    printf("Contenido del fichero:\n%s\n", content);
    printf("Las variables obtenidas son:\n");
    printf("Número de hilos: %d\n", numbers_per_thread);
    printf("Número de threads: %d\n", thread_num);
}


int main(int argc, char *argv[]) {
    char *filename = NULL;
    handle_arguments(argc, argv, &filename);

    char *content = read_file(filename);
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
