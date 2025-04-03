#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include "main.h"


typedef struct {
    int thread_id;
    int numbers_to_process;
} thread_data_t;


/**
 * Checks if a number already exists in the specified list
 * 
 * @param thread_id ID of the thread checking
 * @param number Number to check
 * @param is_even Flag indicating which list to check (1 for even, 0 for odd)
 * @return 1 if number exists, 0 otherwise
 * 
 * Thread-safe operations:
 * - Locks appropriate mutex
 * - Searches for number in list
 * - Unlocks mutex
 */
int number_exists_in_list(int thread_id, int number, int is_even) {
    int exists = 0;
    if (is_even) {
        pthread_mutex_lock(&even_mutex);
        for (int i = 0; i < even_count; i++) {
            if (even_list[i].thread == thread_id && even_list[i].value == number) {
                exists = 1; 
                break;
            }
        }
        pthread_mutex_unlock(&even_mutex);
    } else {
        pthread_mutex_lock(&odd_mutex);
        for (int i = 0; i < odd_count; i++) {
            if (odd_list[i].thread == thread_id && odd_list[i].value == number) {
                exists = 1; 
                break;
            }
        }
        pthread_mutex_unlock(&odd_mutex);
    }
    return exists; 
}


/**
 * Worker thread function that generates and processes random numbers
 * 
 * @param arg Pointer to thread_data_t structure
 * 
 * Operations:
 * - Generates unique random numbers
 * - Adds timestamp to each entry
 * - Classifies numbers as even/odd
 * - Adds numbers to appropriate lists
 * - Handles thread-safe operations
 */
void *thread_worker(void *arg) {
    thread_data_t *data = (thread_data_t *)arg; 
    int thread_id = data->thread_id;
    int numbers_to_process = data->numbers_to_process;


    for (int i = 0; i < numbers_to_process; i++) {
        NumberEntry entry;
        entry.thread = thread_id;


        int random_number;
        int is_even;
        do {
            random_number = rand() % 100000 + 1; 
            is_even = (random_number % 2 == 0); 
        } while (number_exists_in_list(thread_id, random_number, is_even)); 

        entry.value = random_number; 
        entry.index = i; 

        struct timeval tv;
        gettimeofday(&tv, NULL);
        struct tm *t = localtime(&tv.tv_sec);
        snprintf(entry.time, sizeof(entry.time), "%04d-%02d-%02d %02d:%02d:%02d.%06ld", 
                 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, 
                 t->tm_hour, t->tm_min, t->tm_sec, tv.tv_usec);

        if (is_even) {
            add_to_even_list(entry);
        } else {
            add_to_odd_list(entry);
        }
    }

    free(data); 
    pthread_exit(NULL); 
}

/**
 * Creates and manages multiple worker threads
 * 
 * @param thread_num Number of threads to create
 * @param numbers_per_thread Numbers each thread should process
 * @return 1 if successful, 0 if error
 * 
 * Operations:
 * - Creates specified number of threads
 * - Allocates thread data structures
 * - Manages thread creation/joining
 * - Handles thread creation errors
 */
int create_and_manage_threads(int thread_num, int numbers_per_thread) {
    pthread_t threads[thread_num];
    int threads_created = 0;
    int success = 1;

    for (int i = 0; i < thread_num; i++) {
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


    if (!success) {
        for (int i = 0; i < threads_created; i++) {
            if (pthread_join(threads[i], NULL) != 0) {
                perror("Error al esperar al hilo");
            }
        }
        return 0;
    }

    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar al hilo");
            success = 0;
        }
    }

    return success;
}
