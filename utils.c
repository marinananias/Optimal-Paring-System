#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

char** split_field(const char* field, int* count) {
    char* copy = strdup(field); // Create a modifiable copy
    char** result = NULL;
    int token_count = 0;

    char* token = strtok(copy, "|");
    while (token != NULL) {
        result = realloc(result, sizeof(char*) * (token_count + 1));
        if (!result) {
            perror("Failed to allocate memory for split values");
            exit(EXIT_FAILURE);
        }
        result[token_count] = strdup(token);
        if (!result[token_count]) {
            perror("Failed to allocate memory for a token");
            exit(EXIT_FAILURE);
        }
        token_count++;
        token = strtok(NULL, "|");
    }

    *count = token_count;
    free(copy);
    return result;
}
