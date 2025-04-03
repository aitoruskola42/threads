#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "main.h"


pthread_mutex_t even_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t odd_mutex = PTHREAD_MUTEX_INITIALIZER;
NumberEntry *even_list = NULL;
NumberEntry *odd_list = NULL;
static int max_entries_per_list = 0; 
int even_count = 0;
int odd_count = 0;


int initialize_lists(int size) {
    max_entries_per_list = size;
    even_list = (NumberEntry *)malloc(max_entries_per_list * sizeof(NumberEntry));
    odd_list = (NumberEntry *)malloc(max_entries_per_list * sizeof(NumberEntry));

    if (even_list == NULL || odd_list == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para las listas.\n");
        free(even_list); 
        free(odd_list);
        even_list = NULL;
        odd_list = NULL;
        max_entries_per_list = 0;
        return 0; 
    }
    even_count = 0;
    odd_count = 0;
    
    pthread_mutex_init(&even_mutex, NULL);
    pthread_mutex_init(&odd_mutex, NULL);
    
    return 1;
}


void free_lists() {

    pthread_mutex_destroy(&even_mutex);
    pthread_mutex_destroy(&odd_mutex);
    
    free(even_list);
    free(odd_list);
    even_list = NULL;
    odd_list = NULL;
    max_entries_per_list = 0;
    even_count = 0;
    odd_count = 0;
}

void add_to_even_list(NumberEntry entry) {
    if (pthread_mutex_lock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista par\n");
        return;
    }
    if (even_list != NULL && even_count < max_entries_per_list) {
        even_list[even_count++] = entry;
    } else if (even_list == NULL) {
        fprintf(stderr, "Error: La lista de pares no ha sido inicializada.\n");
    } else {
        fprintf(stderr, "Error: La lista de pares está llena.\n");
    }
    
    if (pthread_mutex_unlock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista par\n");
    }
}

void add_to_odd_list(NumberEntry entry) {
    if (pthread_mutex_lock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista impar\n");
        return;
    }
    
    if (odd_list != NULL && odd_count < max_entries_per_list) {
        odd_list[odd_count++] = entry;
    } else if (odd_list == NULL) {
        fprintf(stderr, "Error: La lista de impares no ha sido inicializada.\n");
    } else {
        fprintf(stderr, "Error: La lista de impares está llena.\n");
    }
    
    if (pthread_mutex_unlock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista impar\n");
    }
}

void print_even_list() {
    if (pthread_mutex_lock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista par\n");
        return;
    }

    if (even_list == NULL) {
        if (pthread_mutex_unlock(&even_mutex) != 0) {
            fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista par\n");
        }
        return;
    }
    for (int i = 0; i < even_count; i++) {
        printf("%d\n", even_list[i].value);
    }
    
    if (pthread_mutex_unlock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista par\n");
    }
}

void print_odd_list() {
    if (pthread_mutex_lock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista impar\n");
        return;
    }
    if (odd_list == NULL) {
        if (pthread_mutex_unlock(&odd_mutex) != 0) {
            fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista impar\n");
        }
        return;
    }
    for (int i = 0; i < odd_count; i++) {
        printf("%d\n", odd_list[i].value);
    }
    
    if (pthread_mutex_unlock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista impar\n");
    }
}

static int compare_entries(const void *a, const void *b) {
    return ((NumberEntry *)a)->value - ((NumberEntry *)b)->value;
}

void sort_even_list() {
    if (pthread_mutex_lock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista par\n");
        return;
    }

    if (even_list == NULL || even_count == 0) {
        fprintf(stderr, "La lista de pares está vacía o no inicializada\n");
        pthread_mutex_unlock(&even_mutex);
        return;
    }

    qsort(even_list, even_count, sizeof(NumberEntry), compare_entries);

    if (pthread_mutex_unlock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista par\n");
    }
}

void sort_odd_list() {
    if (pthread_mutex_lock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista impar\n");
        return;
    }

    if (odd_list == NULL || odd_count == 0) {
        fprintf(stderr, "La lista de impares está vacía o no inicializada\n");
        pthread_mutex_unlock(&odd_mutex);
        return;
    }

    qsort(odd_list, odd_count, sizeof(NumberEntry), compare_entries);

    if (pthread_mutex_unlock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista impar\n");
    }
}

void print_sorted_lists() {
    sort_even_list();
    printf("\nLista de números pares ordenada:\n");
    if (pthread_mutex_lock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista par\n");
        return;
    }
    
    if (even_list != NULL && even_count > 0) {
        for (int i = 0; i < even_count; i++) {
            printf("%d\n", even_list[i].value);
        }
    }
    
    if (pthread_mutex_unlock(&even_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista par\n");
        return;
    }
    sort_odd_list();
    printf("\nLista de números impares ordenada:\n");
    if (pthread_mutex_lock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo bloquear el mutex de la lista impar\n");
        return;
    }
    
    if (odd_list != NULL && odd_count > 0) {
        for (int i = 0; i < odd_count; i++) {
            printf("%d\n", odd_list[i].value);
        }
    }
    
    if (pthread_mutex_unlock(&odd_mutex) != 0) {
        fprintf(stderr, "Error: No se pudo desbloquear el mutex de la lista impar\n");
    }
}