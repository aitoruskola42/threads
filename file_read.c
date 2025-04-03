#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "main.h"

/**
 * Reads and processes a text file
 * 
 * @param filename Path to the file to be read
 * @return Pointer to the file contents or NULL if error
 * 
 * Operations:
 * - Opens and validates file existence
 * - Gets file size using fstat
 * - Allocates memory for content
 * - Reads entire file content
 * - Ensures proper line ending with \n
 * - Handles empty files
 * 
 * Returns NULL if:
 * - File cannot be opened
 * - File is empty
 * - Memory allocation fails
 */
char* read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "%s\n", FILE_NOT_FOUND_ERROR);
        return NULL;
    }

    struct stat file_stat;
    if (fstat(fileno(file), &file_stat) != 0) {
        fprintf(stderr, "%s\n", FILE_NOT_FOUND_ERROR);
        fclose(file);
        return NULL;
    }

    long file_size = file_stat.st_size;

    if (file_size > 0) {
        char *content = (char *)malloc(file_size + 2);
        fread(content, 1, file_size, file);
        content[file_size] = '\0';
        fclose(file);

        if (content[file_size - 1] != '\n') {
            content[file_size] = '\n';
            content[file_size + 1] = '\0';
        } else {
            content[file_size] = '\0';
        }
        return content;
    } else {
        fclose(file);
        fprintf(stderr, "%s\n", FILE_EMPTY_MESSAGE);
        return NULL;
    }
}