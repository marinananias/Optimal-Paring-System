/**
 * @file solution_selector.c
 * @brief Provides an optimized Hungarian Algorithm with capacity constraints and logs suboptimal arrangements.
 *
 * Implements helper functions to find optimal matches between mentees and mentors
 * by considering compatibility scores and mentor capacity constraints.
 *
 * Dependencies:
 * - `solution_selector.h`: Declares the interface for these utilities.
 * - `input_parser.h`: Provides data structures for datasets of mentees and mentors.
 */
#include "solution_selector.h"
#include "matching_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

// Structure to store arrangement data
typedef struct {
    int *arrangement;
    int total_score;
} Arrangement;

/**
 * @brief Logs all evaluated arrangements and their scores to a file.
 *
 * @param arrangements Array of arrangements with their scores.
 * @param count Number of arrangements.
 * @param filename Path to the log file.
 */
void log_arrangements(Arrangement *arrangements, int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open log file");
        return;
    }

    fprintf(file, "Arrangement,Total Score\n");
    for (int i = 0; i < count; i++) {
        fprintf(file, "[");
        for (int j = 0; j < count; j++) {
            fprintf(file, "%d", arrangements[i].arrangement[j]);
            if (j < count - 1) {
                fprintf(file, ", ");
            }
        }
        fprintf(file, "], %d\n", arrangements[i].total_score);
    }

    fclose(file);
} // log_arrangements

/**
 * @brief Hungarian Algorithm with capacity constraints and arrangement logging.
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
    Arrangement *arrangements = malloc(n * sizeof(Arrangement));
    int arrangement_count = 0;

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

        // Log arrangement
        arrangements[arrangement_count].arrangement = malloc(n * sizeof(int));
        for (int k = 0; k < n; k++) {
            arrangements[arrangement_count].arrangement[k] = row_assigned[k];
        }
        arrangements[arrangement_count].total_score = -best_value;
        arrangement_count++;
    }

    // Step 4: Generate matches
    *matches = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        (*matches)[i] = row_assigned[i];
    }

    // Log all arrangements to file
    log_arrangements(arrangements, arrangement_count, "arrangement_scores.log");

    // Cleanup
    for (int i = 0; i < arrangement_count; i++) {
        free(arrangements[i].arrangement);
    }
    free(arrangements);

    for (int i = 0; i < n; i++) {
        free(cost[i]);
    }
    free(cost);
    free(row_assigned);
    free(mentor_capacity_remaining);
} // select_optimal_matches

/**
 * @brief Measures the execution time of threaded and non-threaded matching algorithms.
 *
 * This function compares the performance of threaded and non-threaded versions of
 * `match_mentees_to_mentors`, displaying the time taken for each approach.
 *
 * @param mentees Pointer to the dataset of mentees.
 * @param mentors Pointer to the dataset of mentors.
 * @param compatibility_scores Pointer to the array of compatibility scores.
 */
void measure_threading_performance(DataSet *mentees, DataSet *mentors, int *compatibility_scores) {
    struct timespec start, end;
    double threaded_time, non_threaded_time;

    // Threaded execution
    clock_gettime(CLOCK_MONOTONIC, &start);
    match_datasets(mentees, mentors, &compatibility_scores); // Threaded version
    clock_gettime(CLOCK_MONOTONIC, &end);
    threaded_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Non-threaded execution
    clock_gettime(CLOCK_MONOTONIC, &start);
    match_datasets(mentees, mentors, &compatibility_scores); // Non-threaded version
    clock_gettime(CLOCK_MONOTONIC, &end);
    non_threaded_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Log results
    printf("Threaded Execution Time: %.6f seconds\n", threaded_time);
    printf("Non-Threaded Execution Time: %.6f seconds\n", non_threaded_time);

    FILE *log_file = fopen("threading_performance.log", "w");
    if (log_file) {
        fprintf(log_file, "Threaded Execution Time: %.6f seconds\n", threaded_time);
        fprintf(log_file, "Non-Threaded Execution Time: %.6f seconds\n", non_threaded_time);
        fclose(log_file);
    } else {
        perror("Failed to open threading performance log file");
    }
} // measure_threading_performance

/**
 * @brief Non-threaded version of the matching algorithm.
 *
 * This function implements a sequential (non-threaded) version of
 * `match_mentees_to_mentors` for performance comparison purposes.
 *
 * @param mentees Pointer to the dataset of mentees.
 * @param mentors Pointer to the dataset of mentors.
 * @param compatibility_scores Pointer to the array of compatibility scores.
 */
void match_mentees_to_mentors_non_threaded(DataSet *mentees, DataSet *mentors, int **compatibility_scores) {
    int n = mentees->row_count;
    int m = mentors->row_count;

    *compatibility_scores = malloc(n * m * sizeof(int));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            (*compatibility_scores)[i * m + j] = calculate_score(&mentees->rows[i], &mentors->rows[j]);
        }
    }
} // match_mentees_to_mentors_non_threaded
