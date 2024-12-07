#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

typedef struct {
    char*** data; // 2D array for file (rows of string arrays)
    int rows;     // Number of rows in file
    int valid;    // Indicates if the parsing was successful
} InputData;

// Parses a CSV file and returns an InputData structure
InputData parse_input_file(const char* file_name);

// Frees dynamically allocated memory in an InputData structure
void free_input_data(InputData data);

#endif
