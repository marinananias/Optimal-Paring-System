#include <stdio.h>
#include "../input_parser.h"

int main() {
    // Test files
    const char* file1 = "../inputs/file1.csv";
    const char* file2 = "../inputs/file2.csv";

    // Parse input files
    InputData data1 = parse_input_file(file1);
    InputData data2 = parse_input_file(file2);

    // Print data from file1
    printf("Data from %s:\n", file1);
    for (int i = 0; i < data1.rows; i++) {
        printf("Row %d:\n", i + 1);
        for (int j = 0; data1.data[i][j] != NULL; j++) {
            printf("  Column %d: %s\n", j + 1, data1.data[i][j]);
        }
    }

    // Print data from file2
    printf("\nData from %s:\n", file2);
    for (int i = 0; i < data2.rows; i++) {
        printf("Row %d:\n", i + 1);
        for (int j = 0; data2.data[i][j] != NULL; j++) {
            printf("  Column %d: %s\n", j + 1, data2.data[i][j]);
        }
    }

    // Free allocated memory
    free_input_data(data1);
    free_input_data(data2);

    return 0;
}
