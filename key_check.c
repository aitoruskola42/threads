#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

/**
 * Validates the key-value format in the configuration file
 * 
 * @param content File content to validate
 * @param numbers_per_thread Pointer to store numbers per thread value
 * @param thread_num Pointer to store number of threads value
 * @return 1 if valid format, 0 otherwise
 * 
 * Validates:
 * - Presence of required keys (numbers_per_thread, thread_num)
 * - Valid numeric values for each key
 * - Proper key=value format
 * 
 * Extracts and stores:
 * - Number of items per thread
 * - Total number of threads
 */
int validate_key_value_format(const char *content, int *numbers_per_thread, int *thread_num) {
    const char *required_keys[] = {"numbers_per_thread", "thread_num"};
    int keys_found[2] = {0, 0};

    char *content_copy = strdup(content);
    char *line = strtok(content_copy, "\n");
    while (line != NULL) {
        for (int i = 0; i < 2; i++) {
            if (strncmp(line, required_keys[i], strlen(required_keys[i])) == 0) {
                char *equals_sign = strchr(line, '=');
                if (equals_sign && *(equals_sign + 1) != '\0') {
                    keys_found[i] = 1;
                    if (i == 0) {
                        *numbers_per_thread = atoi(equals_sign + 1);
                    } else if (i == 1) {
                        *thread_num = atoi(equals_sign + 1);
                    }
                }
                break;
            }
        }
        line = strtok(NULL, "\n");
    }

    free(content_copy);
    return keys_found[0] && keys_found[1];
}
