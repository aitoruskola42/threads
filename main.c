#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  
#include <time.h>

#include "main.h"
// Asegúrate de incluir el archivo de gestión de listas

void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
void print_content(const char *content, int numbers_per_thread, int thread_num);
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num);
int initialize_lists(int size);
void free_lists();


void print_content(const char *content, int numbers_per_thread, int thread_num) {
    printf("Contenido del fichero:\n%s\n", content);
    printf("Las variables obtenidas son:\n");
    printf("Numero de hilos: %d\n", numbers_per_thread);
    printf("Numero de threads: %d\n", thread_num);
}


int main(int argc, char *argv[]) {
    char *filename = NULL;
    handle_arguments(argc, argv, &filename);
    srand(time(NULL)); // Inicializar la semilla para números aleatorios

    char *content = read_file(filename);
    int numbers_per_thread = 0;
    int thread_num = 0;
    int lists_initialized = 0; // Flag para saber si las listas se inicializaron

    if (content) {
        if (validate_key_value_format(content, &numbers_per_thread, &thread_num)) {
            print_content(content, numbers_per_thread, thread_num);

            // Calcular el tamaño total para las listas
            int total_size = numbers_per_thread * thread_num;
            printf("Inicializando listas con tamaño total: %d (%d hilos * %d números/hilo)\n", 
                   total_size, thread_num, numbers_per_thread);

            // Llamar a initialize_lists con el tamaño total
            if (initialize_lists(total_size)) {
                lists_initialized = 1; // Marcar como inicializadas
                
                // Llamar a la función para crear y gestionar hilos
                create_and_manage_threads(thread_num, numbers_per_thread);

                print_even_list();
                print_odd_list();

            } else {
                fprintf(stderr, "Fallo al inicializar las listas. Saliendo.\n");
            }

        } else {
            fprintf(stderr, "El fichero no tiene el formato correcto de clave-valor.\n");
        }
        free(content); // Liberar content independientemente de si las listas se inicializaron
    } else {
        // El error ya se imprimió en read_file si content es NULL
    }

    // Liberar la memoria de las listas ANTES de salir si fueron inicializadas
    if (lists_initialized) {
        free_lists();
    }

    return 0;
}
