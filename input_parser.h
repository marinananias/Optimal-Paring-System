/**
 * @file input_parser.h
 * @brief Header file for the CSV parsing utility.
 *
 * Declares the data structures and function prototypes used
 * for parsing a CSV file and managing the resulting data.
 */
#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H
#include <stdbool.h>

// Structure to store individual data rows
typedef struct {
    char *name;            // Mentor or Mentee name
    char **attributes;     // List of attributes
    int attributes_count;  // Number of attributes
    int capacity;          // Maximum number of mentees a mentor can take
} DataRow;

// Structure to store the entire dataset
typedef struct {
    DataRow *rows; // Array of rows
    int row_count; // Total number of rows
} DataSet;

// Function Declarations
DataSet *parse_csv(const char *file_path, bool *success);
void free_dataset(DataSet *dataset);

#endif // INPUT_PARSER_H
