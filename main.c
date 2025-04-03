#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>  
#include <time.h>
#include "main.h"





int main(int argc, char *argv[]) {
    char *filename = NULL;
    handle_arguments(argc, argv, &filename);
    srand(time(NULL)); 

    char *content = read_file(filename);
    int numbers_per_thread = 0;
    int thread_num = 0;
    int lists_initialized = 0;

    if (content) {
        if (validate_key_value_format(content, &numbers_per_thread, &thread_num)) {
            int total_size = numbers_per_thread * thread_num;

            if (initialize_lists(total_size)) {
                lists_initialized = 1;
                create_and_manage_threads(thread_num, numbers_per_thread);
                print_sorted_lists();
            } else {
                fprintf(stderr, "Fallo al inicializar las listas. Saliendo.\n");
            }
        } else {
            fprintf(stderr, "El fichero no tiene el formato correcto de clave-valor.\n");
        }
        free(content);
    }

    if (lists_initialized) {
        free_lists();
    }

    return 0;
}
