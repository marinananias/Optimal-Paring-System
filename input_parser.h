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

/**
 * @brief Represents an individual data row (e.g., Mentor or Mentee).
 *
 * Stores the name, a list of attributes (e.g., skills or topics),
 * the number of attributes, and an optional capacity (for mentors).
 */
typedef struct {
    char *name;            ///< Name of the entity (Mentor or Mentee)
    char **attributes;     ///< List of attributes (pipe-separated in CSV)
    int attributes_count;  ///< Number of attributes in the list
    int capacity;          ///< Capacity (only applicable for mentors)
} DataRow;

/**
 * @brief Represents the entire dataset parsed from a CSV file.
 *
 * Contains an array of `DataRow` objects and the total number of rows.
 */
typedef struct {
    DataRow *rows;         ///< Array of rows (each representing a Mentor/Mentee)
    int row_count;         ///< Total number of rows in the dataset
} DataSet;

// Function Declarations
DataSet *parse_csv(const char *file_path, bool *success);
void free_dataset(DataSet *dataset);

#endif // INPUT_PARSER_H
