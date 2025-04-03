#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Eliminar MAX_ENTRIES
// #define MAX_ENTRIES 100 // Comentado o eliminado

// Cambiar a punteros y añadir tamaño máximo
NumberEntry *even_list = NULL;
NumberEntry *odd_list = NULL;
static int max_entries_per_list = 0; // Tamaño máximo por lista
int even_count = 0;
int odd_count = 0;

// Nueva función para inicializar las listas
int initialize_lists(int size) {
    max_entries_per_list = size;
    even_list = (NumberEntry *)malloc(max_entries_per_list * sizeof(NumberEntry));
    odd_list = (NumberEntry *)malloc(max_entries_per_list * sizeof(NumberEntry));

    if (even_list == NULL || odd_list == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para las listas.\n");
        // Liberar cualquier memoria que se haya podido asignar
        free(even_list); 
        free(odd_list);
        even_list = NULL;
        odd_list = NULL;
        max_entries_per_list = 0;
        return 0; // Indicar fallo
    }
    even_count = 0;
    odd_count = 0;
    return 1; // Indicar éxito
}

// Nueva función para liberar memoria
void free_lists() {
    free(even_list);
    free(odd_list);
    even_list = NULL;
    odd_list = NULL;
    max_entries_per_list = 0;
    even_count = 0;
    odd_count = 0;
}


void add_to_even_list(NumberEntry entry) {
    // Usar max_entries_per_list en lugar de MAX_ENTRIES
    if (even_list != NULL && even_count < max_entries_per_list) {
        even_list[even_count++] = entry;
    } else if (even_list == NULL) {
         fprintf(stderr, "Error: La lista de pares no ha sido inicializada.\n");
    } else {
         fprintf(stderr, "Error: La lista de pares está llena.\n");
    }
}

void add_to_odd_list(NumberEntry entry) {
    // Usar max_entries_per_list en lugar de MAX_ENTRIES
    if (odd_list != NULL && odd_count < max_entries_per_list) {
        odd_list[odd_count++] = entry;
    } else if (odd_list == NULL) {
         fprintf(stderr, "Error: La lista de impares no ha sido inicializada.\n");
    } else {
         fprintf(stderr, "Error: La lista de impares está llena.\n");
    }
}

void print_even_list() {
    // Añadir comprobación por si la lista no está inicializada
    if (even_list == NULL) {
        printf("La lista de pares no está inicializada.\n");
        return;
    }
    printf("Lista de numeros pares (%d/%d):\n", even_count, max_entries_per_list);
    for (int i = 0; i < even_count; i++) {
        printf("Index: %d, Thread: %d, Time: %s, Value: %d\n",
               even_list[i].index, even_list[i].thread, even_list[i].time, even_list[i].value);
    }
}

void print_odd_list() {
    // Añadir comprobación por si la lista no está inicializada
    if (odd_list == NULL) {
        printf("La lista de impares no está inicializada.\n");
        return;
    }
    printf("Lista de numeros impares (%d/%d):\n", odd_count, max_entries_per_list);
    for (int i = 0; i < odd_count; i++) {
        printf("Index: %d, Thread: %d, Time: %s, Value: %d\n",
               odd_list[i].index, odd_list[i].thread, odd_list[i].time, odd_list[i].value);
    }
}