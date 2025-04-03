#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "main.h"

// Estructura para pasar datos a cada hilo (si es necesario)
// typedef struct {
//     int thread_id;
//     int numbers_to_process;
//     // Otros datos que el hilo pueda necesitar...
// } thread_data_t;

// Función que ejecutará cada hilo
void *thread_worker(void *arg) {
    // Aquí irá la lógica de cada hilo:
    // 1. Recibir sus datos (ej: ID de hilo, rango de números a procesar).
    // 2. Procesar los números asignados.
    // 3. Determinar si cada número es par o impar.
    // 4. Crear una estructura NumberEntry con los detalles (index, thread_id, time, value).
    // 5. Añadir la entrada a la lista correspondiente (even_list o odd_list).
    //    ¡¡IMPORTANTE!!: Se necesitará un mutex para proteger el acceso a 
    //    even_list, odd_list, even_count y odd_count cuando se añadan elementos.
    
    printf("Hilo ejecutándose (argumento: %p)...\n", arg); // Mensaje temporal
    
    // Simulación de añadir un número (¡esto necesita la lógica real y mutex!)
    /*
    if (/* número es par */ /*) {
        NumberEntry entry = { ... }; 
        // pthread_mutex_lock(&list_mutex); // Bloquear mutex
        add_to_even_list(entry);
        // pthread_mutex_unlock(&list_mutex); // Desbloquear mutex
    } else {
        NumberEntry entry = { ... };
        // pthread_mutex_lock(&list_mutex); // Bloquear mutex
        add_to_odd_list(entry);
        // pthread_mutex_unlock(&list_mutex); // Desbloquear mutex
    }
    */

    pthread_exit(NULL); // Terminar el hilo
}

// Función para crear y gestionar los hilos
int create_and_manage_threads(int thread_num, int numbers_per_thread) {
    pthread_t threads[thread_num];
    // thread_data_t thread_data_array[thread_num]; // Array para los datos de cada hilo si se usa la estructura

    printf("Creando %d hilos...\n", thread_num);

    for (int i = 0; i < thread_num; i++) {
        printf("  Creando hilo %d\n", i);
        // Preparar datos para el hilo i (ej: thread_data_array[i] = ...)
        // Pasar el ID de hilo o la estructura de datos como argumento a thread_worker
        // if (pthread_create(&threads[i], NULL, thread_worker, /* (void *)&thread_data_array[i] */ (void *)(intptr_t)i) != 0) {
        if (pthread_create(&threads[i], NULL, thread_worker, (void *)(intptr_t)i) != 0) { // Ejemplo pasando solo el ID
             perror("Error al crear el hilo");
             // Manejar el error (quizás terminar los hilos ya creados)
             return 0; // Indicar fallo
        }
    }

    printf("Esperando a que los hilos terminen...\n");
    for (int i = 0; i < thread_num; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error al esperar al hilo");
            // Manejar el error
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


