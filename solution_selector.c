/**
 * @file solution_selector.c
 * @brief Provides an optimized Hungarian Algorithm with capacity constraints.
 *
 * Implements helper functions to find optimal matches between mentees and mentors
 * by considering compatibility scores and mentor capacity constraints.
 *
 * Dependencies:
 * - `solution_selector.h`: Declares the interface for these utilities.
 * - `input_parser.h`: Provides data structures for datasets of mentees and mentors.
 * - `stdlib.h`, `stdio.h`, `limits.h`: Standard libraries for memory allocation, input/output, and integer limits.
 */
#include "solution_selector.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * @brief Optimized Hungarian Algorithm with capacity constraints.
 *
 * Solves the assignment problem for mentees and mentors by maximizing compatibility scores
 * while respecting the capacity constraints of each mentor.
 *
 * @param mentees Pointer to the dataset of mentees.
 * @param mentors Pointer to the dataset of mentors.
 * @param compatibility_scores Pointer to the array of compatibility scores.
 * @param matches Pointer to an array where the optimal matches will be stored.
 *
 * @note The `matches` array is dynamically allocated and must be freed by the caller.
 *       Each index of `matches` corresponds to a mentee, and its value indicates the
 *       index of the matched mentor. If no match is found, the value is -1.
 */
void select_optimal_matches(DataSet *mentees, DataSet *mentors, int *compatibility_scores, int **matches) {
    int n = mentees->row_count; // Number of mentees
    int m = mentors->row_count; // Number of mentors

    // Step 1: Initialize the cost matrix
    int **cost = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++) {
        cost[i] = malloc(m * sizeof(int));
        for (int j = 0; j < m; j++) {
            cost[i][j] = -compatibility_scores[i * m + j]; // Convert to maximization problem
        }
    }

    // Step 2: Initialize assignments and capacity tracking
    int *row_assigned = malloc(n * sizeof(int)); // Mentee to mentor
    int *mentor_capacity_remaining = malloc(m * sizeof(int)); // Track remaining capacity for each mentor
    for (int i = 0; i < n; i++) row_assigned[i] = -1;
    for (int j = 0; j < m; j++) mentor_capacity_remaining[j] = mentors->rows[j].capacity;

    // Step 3: Solve the assignment problem while respecting capacities
    for (int i = 0; i < n; i++) {
        int best_col = -1;
        int best_value = INT_MAX;

        for (int j = 0; j < m; j++) {
            if (cost[i][j] < best_value && mentor_capacity_remaining[j] > 0) {
                best_value = cost[i][j];
                best_col = j;
            }
        }

        if (best_col != -1) {
            row_assigned[i] = best_col; // Assign the mentor to the mentee
            mentor_capacity_remaining[best_col]--; // Decrease the mentor's remaining capacity
        }
    }

    // Step 4: Generate matches
    *matches = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        (*matches)[i] = row_assigned[i];
    }

    // Cleanup
    for (int i = 0; i < n; i++) {
        free(cost[i]);
    }
    free(cost);
    free(row_assigned);
    free(mentor_capacity_remaining);
} // select_optimal_matches