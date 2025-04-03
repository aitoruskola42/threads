#ifndef MAIN_H
#define MAIN_H
typedef struct {
    int index;
    int thread;
    char time[20]; // Formato de fecha con segundos
    int value;     // Valor par o impar
} NumberEntry;

#define ARG_ERROR "Argumentos incorrectos, ponga -h para ver argumentos disponibles"
#define HELP_MESSAGE "Uso: ./a.exe -f <nombre_fichero> o --file <nombre_fichero>"
#define EXT_ERROR "Se requiere que el fichero tenga la extensión .txt"
#define FILE_NOT_FOUND_ERROR "El fichero no existe o no se puede abrir"
#define FILE_EMPTY_MESSAGE "El fichero está vacío."
#define FILE_ERROR "Error al introducir el fichero"

// Declaraciones de funciones
void handle_arguments(int argc, char *argv[], char **filename);
char* read_file(const char *filename);
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num);
void print_content(const char *content, int numbers_per_thread, int thread_num);

// Funciones para manejar las listas
int initialize_lists(int size);
void add_to_even_list(NumberEntry entry);
void add_to_odd_list(NumberEntry entry);
void print_even_list();
void print_odd_list();
void free_lists();


#endif
