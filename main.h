#ifndef MAIN_H
#define MAIN_H
#include <pthread.h>

typedef struct {
    int index;
    int thread;
    char time[50];
    int value;    
} NumberEntry;

#define ARG_ERROR "Argumentos incorrectos, ponga -h para ver argumentos disponibles"
#define HELP_MESSAGE "Uso: ./a.exe -f <nombre_fichero> o --file <nombre_fichero>"
#define EXT_ERROR "Se requiere que el fichero tenga la extensión .txt"
#define FILE_NOT_FOUND_ERROR "El fichero no existe o no se puede abrir"
#define FILE_EMPTY_MESSAGE "El fichero está vacío."
#define FILE_ERROR "Error al introducir el fichero"


void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num);


int initialize_lists(int size);
void add_to_even_list(NumberEntry entry);
void add_to_odd_list(NumberEntry entry);
void print_even_list();
void print_odd_list();
void free_lists();

void sort_even_list();
void sort_odd_list();
void print_sorted_lists();

void *thread_worker(void *arg);
int create_and_manage_threads(int thread_num, int numbers_per_thread);

extern NumberEntry *even_list; 
extern NumberEntry *odd_list;  
extern int even_count;         
extern int odd_count;          


extern pthread_mutex_t even_mutex;
extern pthread_mutex_t odd_mutex;

#endif
