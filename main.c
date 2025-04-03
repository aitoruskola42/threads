#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  

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

    char *content = read_file(filename);
    int numbers_per_thread = 0;
    int thread_num = 0;
    int lists_initialized = 0; // Flag para saber si las listas se inicializaron

    if (content) {
        if (validate_key_value_format(content, &numbers_per_thread, &thread_num)) {
            print_content(content, numbers_per_thread, thread_num);

            // Llamar a initialize_lists DESPUÉS de obtener numbers_per_thread
            if (initialize_lists(numbers_per_thread)) {
                lists_initialized = 1; // Marcar como inicializadas
                // Mover el bucle y las impresiones aquí dentro
                // Ejemplo de uso
                for (int i = 0; i < numbers_per_thread; i++) {
                    NumberEntry entry;
                    entry.index = i;
                    entry.thread = 1; // Ejemplo de thread
                    snprintf(entry.time, sizeof(entry.time), "2023-04-03 12:00:%02d", i); // Formato de tiempo

                    if (i % 2 == 0) {
                        entry.value = i;
                        add_to_even_list(entry);
                    } else {
                        entry.value = i;
                        add_to_odd_list(entry);
                    }
                }

                print_even_list();
                print_odd_list();

            } else {
                fprintf(stderr, "Fallo al inicializar las listas. Saliendo.\n");
                // No es necesario llamar a free_lists si initialize_lists falló
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
