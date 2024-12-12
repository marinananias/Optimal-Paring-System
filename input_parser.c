/**
 * @file input_parser.c
 * @brief CSV Parser for extracting data into memory.
 *
 * Contains the implementation of a CSV parser designed to process
 * files where each line contains a name, optional attributes (pipe-separated),
 * and an optional numeric capacity value. The parsed data is stored in dynamically 
 * allocated `DataSet` and `DataRow` structures for further use in applications.
 *
 * Key Features:
 * - Reads CSV files and parses lines into structured rows.
 * - Supports attributes as a pipe-separated list.
 * - Dynamically allocates memory for rows and attributes.
 * - Trims leading and trailing whitespace for clean data.
 * - Handles errors during file reading, memory allocation, and format validation.
 *
 * Example CSV Format:
 * Name,Attributes,Capacity
 * Item1,attr1|attr2|attr3,100
 * Item2,attrA|attrB,50
 */
#include "input_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024 // Maximum length of a line in the input CSV file
#define MAX_ATTRIBUTES 10 // Maximum number of attributes allowed per row

/**
 * Helper function to trim leading and trailing whitespace from a string.
 * Modifies the string in-place by adjusting the null terminator.
 *
 * @param str Pointer to the string to be trimmed.
 */
static void trim_whitespace(char *str) {
    if (str == NULL) return;

    // Remove trailing whitespace
    size_t len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }

    // Remove leading whitespace
    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // Shift the trimmed string to the beginning
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
} // trim_whitespace

/**
 * Splits a pipe-separated string of attributes and stores them in a DataRow object.
 *
 * @param attribute_str String containing pipe-separated attributes.
 * @param row Pointer to the DataRow where attributes will be stored.
 */
static void split_attributes(char *attribute_str, DataRow *row) {
    char *token = strtok(attribute_str, "|");
    while (token != NULL && row->attributes_count < MAX_ATTRIBUTES) {
        trim_whitespace(token);
        row->attributes[row->attributes_count++] = strdup(token);
        token = strtok(NULL, "|");
    }
} // split_attributes

/**
 * Parses a CSV file and populates a DataSet object with its contents.
 * Each row in the CSV file is represented as a DataRow in the DataSet.
 * The function expects the CSV file to follow a specific format:
 * - Each line contains a name, attributes (optional, pipe-separated), 
 * and an optional capacity.
 *
 * @param file_path Path to the CSV file to be parsed.
 * @param success Pointer to a boolean variable to indicate success or failure.
 * @return Pointer to the populated DataSet object, or NULL on failure.
 */
DataSet *parse_csv(const char *file_path, bool *success) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Error opening file");
        *success = false;
        return NULL;
    }

    // Allocate memory for the DataSet
    DataSet *dataset = (DataSet *)malloc(sizeof(DataSet));
    if (!dataset) {
        perror("Error allocating memory for dataset");
        *success = false;
        fclose(file);
        return NULL;
    }

    dataset->rows = NULL;
    dataset->row_count = 0;

    char line[MAX_LINE_LENGTH];

    // Read the first line (header or empty check)
    if (fgets(line, MAX_LINE_LENGTH, file) == NULL) {
        printf("Error: File is empty or invalid format.\n");
        *success = false;
        fclose(file);
        free(dataset);
        return NULL;
    }

    // Read and process each subsequent line
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        DataRow row;
        row.attributes = (char **)malloc(MAX_ATTRIBUTES * sizeof(char *));
        row.attributes_count = 0;

        if (!row.attributes) {
            perror("Error allocating memory for attributes");
            free_dataset(dataset);
            *success = false;
            fclose(file);
            return NULL;
        }
        
        // Tokenize the line by commas
        char *token = strtok(line, ",");
        if (token) {
            trim_whitespace(token);
            row.name = strdup(token);
        }

        // Process the remaining tokens for attributes and capacity
        while ((token = strtok(NULL, ",")) != NULL) {
            trim_whitespace(token);
            
            for (size_t i = 0; i < strlen(token)+3; i++) {
                if (isdigit(token[i])) {
                    row.capacity = atoi(&token[i]);
                }
            }
            
           if (row.attributes_count < MAX_ATTRIBUTES) {
                split_attributes(token, &row);
            }
        }

        // Resize the rows array in the dataset
        dataset->rows = (DataRow *)realloc(dataset->rows, (dataset->row_count + 1) * sizeof(DataRow));
        if (!dataset->rows) {
            perror("Error reallocating memory for dataset rows");
            free_dataset(dataset);
            *success = false;
            fclose(file);
            return NULL;
        }

        // Add the parsed row to the dataset
        dataset->rows[dataset->row_count++] = row;
    }

    fclose(file);
    *success = true;
    return dataset;
} // parse_csv

/**
 * Frees the memory allocated for a DataSet object, including its rows and attributes.
 *
 * @param dataset Pointer to the DataSet to be freed.
 */
void free_dataset(DataSet *dataset) {
    if (!dataset) return;

    // Free each row and its attributes
    for (int i = 0; i < dataset->row_count; i++) {
        free(dataset->rows[i].name);
        for (int j = 0; j < dataset->rows[i].attributes_count; j++) {
            free(dataset->rows[i].attributes[j]);
        }
        free(dataset->rows[i].attributes);
    }

    // Free the rows array and the dataset itself
    free(dataset->rows);
    free(dataset);
} // free_dataset
