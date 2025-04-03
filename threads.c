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

    printf("Creando %d hilos...\n", thread_num);

    for (int i = 0; i < thread_num; i++) {
        printf("  Creando hilo %d\n", i);
        
        // Preparar datos para el hilo
        thread_data_t *data = malloc(sizeof(thread_data_t));
        data->thread_id = i;
        data->numbers_to_process = numbers_per_thread;

        if (pthread_create(&threads[i], NULL, thread_worker, (void *)data) != 0) {
            perror("Error al crear el hilo");
            return 0; // Indicar fallo
        }
    }

    printf("Esperando a que los hilos terminen...\n");
    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar al hilo");
        }
        printf("  Hilo %d terminado.\n", i);
    }

    printf("Todos los hilos han terminado.\n");
    return 1; // Indicar éxito
}

// --- Consideraciones adicionales ---
// 1. Mutex: Necesitarás inicializar un mutex (pthread_mutex_t) globalmente o en main,
//    y pasarlo o hacerlo accesible a las funciones add_to_even_list/add_to_odd_list,
//    o usarlo directamente en thread_worker antes y después de llamar a dichas funciones.
// 2. Distribución del trabajo: Decide cómo se distribuirán los números entre los hilos.
//    ¿Cada hilo procesa 'numbers_per_thread' números? ¿Cómo sabe cada hilo *cuáles* números procesar?
//    La estructura thread_data_t puede ser útil para esto.
// 3. Gestión de errores: Añadir una gestión de errores más robusta.
// 4. Inclusión en main: Necesitarás declarar `create_and_manage_threads` en `main.h` (o directamente en `main.c` si prefieres no crear `threads.h`)
//    e incluir `<pthread.h>`. Luego, llama a `create_and_manage_threads` desde `main` en lugar del bucle de ejemplo.
// 5. Compilación: Recuerda enlazar con la librería pthread al compilar (usando `-pthread`). Ejemplo: gcc main.c list_management.c threads.c read_file.c arg_check.c -o nemer -pthread


