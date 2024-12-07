#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_parser.h"
#include "utils.h"

// Helper function to count rows in a file
int count_rows(FILE* file) {
    int rows = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        rows++;
    }
    rewind(file); // Reset the file pointer to the beginning
    return rows;
}

// Helper function to parse a line into columns, supporting multi-value fields
char** parse_line(const char* line, int* columns) {
    char** result = NULL; // Array to store parsed columns
    char* token = NULL;   // Current token
    char* line_copy = strdup(line); // Create a modifiable copy of the line
    int col_count = 0;    // Counter for the number of columns

    // Tokenize the line based on ',' or '\n'
    token = strtok(line_copy, ",\n");
    while (token != NULL) {
        // Check if the field contains multi-values
        if (strchr(token, '|')) {
            // Split the multi-value field
            int sub_count = 0;
            char** split_values = split_field(token, &sub_count);

            // Convert the split values back into a single string for storage
            char* combined = malloc(1024); // Ensure enough space
            combined[0] = '\0';
            for (int i = 0; i < sub_count; i++) {
                strcat(combined, split_values[i]);
                if (i < sub_count - 1) {
                    strcat(combined, "|"); // Rejoin with '|'
                }
                free(split_values[i]);
            }
            free(split_values);

            // Store the combined string
            result = realloc(result, sizeof(char*) * (col_count + 1));
            if (!result) {
                perror("Failed to allocate memory for columns");
                exit(EXIT_FAILURE);
            }
            result[col_count] = combined;
        } else {
            // Single-value field
            result = realloc(result, sizeof(char*) * (col_count + 1));
            if (!result) {
                perror("Failed to allocate memory for columns");
                exit(EXIT_FAILURE);
            }

            // Duplicate the token and store it in the result
            result[col_count] = strdup(token);
            if (!result[col_count]) {
                perror("Failed to allocate memory for a column");
                exit(EXIT_FAILURE);
            }
        }

        col_count++;
        token = strtok(NULL, ",\n");
    }

    // Add a null terminator to the array for safety
    result = realloc(result, sizeof(char*) * (col_count + 1));
    if (!result) {
        perror("Failed to allocate memory for null terminator");
        exit(EXIT_FAILURE);
    }
    result[col_count] = NULL;

    // Set the column count
    *columns = col_count;

    // Free the line copy and return the result
    free(line_copy);
    return result;
}


// Parse input files and populate InputData structure
InputData parse_input_file(const char* file_name) {
    // Try opening the files
    FILE* file = fopen(file_name, "r");
    if (!file) {
        perror("Error opening input files");
        exit(EXIT_FAILURE);
    }

    // Declare InputData object
    InputData data = {NULL, 0, 0};

    // Count rows and skip headers
    data.rows = count_rows(file) - 1;

    // Allocate memory for data arrays
    data.data = malloc(sizeof(char**) * data.rows);
    if (!data.data) {
        perror("Failed to allocate memory for rows");
        exit(EXIT_FAILURE);
    }

    // Initialize line buffer
    char line[1024];

    // Skip header row for file
    fgets(line, sizeof(line), file);

    // Parse rows from file
    int row_index = 0;
    while (fgets(line, sizeof(line), file)) {
        int columns = 0;
        data.data[row_index] = parse_line(line, &columns);
        printf("Row %d has %d columns\n", row_index, columns);
        row_index++;
    }

    data.valid = 1; // Set to 1 if parsing is successful
    fclose(file);
    return data;
}

// Free dynamically allocated memory in InputData
void free_input_data(InputData data) {
    for (int i = 0; i < data.rows; i++) {
        int j = 0;
        while (data.data[i][j] != NULL) {
            free(data.data[i][j]);
            j++;
        }
        free(data.data[i]);
    }
    free(data.data);
}

