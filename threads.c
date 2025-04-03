#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"

// Estructura para pasar datos a cada hilo
typedef struct {
    int thread_id;
    int numbers_to_process;
} thread_data_t;

// Función para verificar si un número ya existe en la lista correspondiente
int number_exists_in_list(int thread_id, int number, int is_even) {
    int exists = 0;
    if (is_even) {
        pthread_mutex_lock(&even_mutex);
        for (int i = 0; i < even_count; i++) {
            if (even_list[i].thread == thread_id && even_list[i].value == number) {
                exists = 1; // El número ya existe en la lista de pares para este hilo
                break;
            }
        }
        pthread_mutex_unlock(&even_mutex);
    } else {
        pthread_mutex_lock(&odd_mutex);
        for (int i = 0; i < odd_count; i++) {
            if (odd_list[i].thread == thread_id && odd_list[i].value == number) {
                exists = 1; // El número ya existe en la lista de impares para este hilo
                break;
            }
        }
        pthread_mutex_unlock(&odd_mutex);
    }
    return exists; // El número no existe
}

// Función que ejecutará cada hilo
void *thread_worker(void *arg) {
    thread_data_t *data = (thread_data_t *)arg; // Convertir el argumento a la estructura
    int thread_id = data->thread_id;
    int numbers_to_process = data->numbers_to_process;

    printf("Hilo %d ejecutándose...\n", thread_id);

    // Simulación de procesamiento de números
    for (int i = 0; i < numbers_to_process; i++) {
        NumberEntry entry;
        entry.thread = thread_id;

        // Generar un número aleatorio único
        int random_number;
        int is_even;
        do {
            random_number = rand() % 100000 + 1; // Generar un número aleatorio entre 1 y 100000
            is_even = (random_number % 2 == 0); // Determinar si es par
        } while (number_exists_in_list(thread_id, random_number, is_even)); // Verificar si el número ya existe

        entry.value = random_number; // Asignar el número aleatorio
        entry.index = i; // Asignar el índice de iteración

        // Obtener la fecha y hora actuales con microsegundos
        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *t = localtime(&tv.tv_sec);
        snprintf(entry.time, sizeof(entry.time), "%04d-%02d-%02d %02d:%02d:%02d.%06ld", 
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
                 t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec);

        // Añadir a la lista correspondiente
        if (is_even) {
            add_to_even_list(entry);
        } else {
            add_to_odd_list(entry);
        }
    }

    free(data); // Liberar la memoria de los datos del hilo
    pthread_exit(NULL); // Terminar el hilo
}

// Función para crear y gestionar los hilos
int create_and_manage_threads(int thread_num, int numbers_per_thread) {
    pthread_t threads[thread_num];
    int threads_created = 0;
    int success = 1;

    printf("Creando %d hilos...\n", thread_num);

    for (int i = 0; i < thread_num; i++) {
        printf("  Creando hilo %d\n", i);
        
        // Preparar datos para el hilo
        thread_data_t *data = malloc(sizeof(thread_data_t));
        if (data == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para los datos del hilo\n");
            success = 0;
            break;
        }
        data->thread_id = i;
        data->numbers_to_process = numbers_per_thread;

        if (pthread_create(&threads[i], NULL, thread_worker, (void *)data) != 0) {
            perror("Error al crear el hilo");
            free(data);
            success = 0;
            break;
        }
        threads_created++;
    }

    // Si hubo un error, esperar a que terminen los hilos ya creados
    if (!success) {
        printf("Error en la creación de hilos. Esperando a que terminen los hilos creados...\n");
        for (int i = 0; i < threads_created; i++) {
            if (pthread_join(threads[i], NULL) != 0) {
                perror("Error al esperar al hilo");
            }
        }
        return 0;
    }

    printf("Esperando a que los hilos terminen...\n");
    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar al hilo");
            success = 0;
        }
        printf("  Hilo %d terminado.\n", i);
    }

    printf("Todos los hilos han terminado.\n");
    return success;
}
