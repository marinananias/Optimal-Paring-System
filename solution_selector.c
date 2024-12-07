#include <stdio.h>
#include <stdlib.h>
#include "solution_selector.h"
#include "matching_engine.h"

// Helper function to initialize a cost matrix from compatibility scores
double** initialize_cost_matrix(InputData data1, InputData data2, int* size) {
    *size = data1.rows > data2.rows ? data1.rows : data2.rows;
    double** cost_matrix = malloc(sizeof(double*) * (*size));

    for (int i = 0; i < *size; i++) {
        cost_matrix[i] = malloc(sizeof(double) * (*size));
        for (int j = 0; j < *size; j++) {
            if (i < data1.rows && j < data2.rows) {
                // Calculate cost as (1 - compatibility score)
                cost_matrix[i][j] = 1.0 - compute_score(
                    (const char**)data1.data[i],
                    (const char**)data2.data[j],
                    2, 2 // Assuming 2 columns, adjust as needed
                );
            } else {
                // Assign high cost for "dummy" matches
                cost_matrix[i][j] = 1.0;
            }
        }
    }
    return cost_matrix;
}

// Helper function to free the cost matrix
void free_cost_matrix(double** matrix, int size) {
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Helper function to find the minimum cost in a row
int find_min_index(double* row, int size, int* matched) {
    int min_index = -1;
    double min_cost = 1.0; // Initialize with the highest cost

    for (int j = 0; j < size; j++) {
        if (!matched[j] && row[j] < min_cost) {
            min_cost = row[j];
            min_index = j;
        }
    }

    return min_index;
}

// Function to find optimal matches ensuring everyone gets matched
Match* find_optimal_matches(InputData data1, InputData data2, int* match_count) {
    int size;
    double** cost_matrix = initialize_cost_matrix(data1, data2, &size);

    // To keep track of which participants and panels are already matched
    int* matched_rows = calloc(size, sizeof(int)); // Tracks matches for data1
    int* matched_cols = calloc(size, sizeof(int)); // Tracks matches for data2

    Match* matches = malloc(sizeof(Match) * data1.rows);
    *match_count = 0;

    // Find matches greedily (optimal if costs represent true preferences)
    for (int i = 0; i < data1.rows; i++) {
        int best_j = find_min_index(cost_matrix[i], size, matched_cols);
        if (best_j != -1) {
            matched_rows[i] = 1;
            matched_cols[best_j] = 1;
            matches[*match_count] = (Match){i, best_j, 1.0 - cost_matrix[i][best_j]};
            (*match_count)++;
        }
    }

    // Clean up
    free_cost_matrix(cost_matrix, size);
    free(matched_rows);
    free(matched_cols);

    return matches;
}
