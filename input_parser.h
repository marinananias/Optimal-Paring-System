#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H

typedef struct {
    char*** data; // 2D array for file (rows of string arrays)
    int rows;     // Number of rows in file
    int valid;    // Indicates if the parsing was successful
} InputData;

InputData parse_input_file(const char* file1_name);
void free_input_data(InputData data);

#endif
