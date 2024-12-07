#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "matching_engine.h"

// Mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Struct for thread arguments
typedef struct {
    InputData data1;
    InputData data2;
    Match* matches;
    int* match_index;
    int start;
    int end;
} ThreadArgs;

// Helper function to compute the compatibility score
double compute_score(const char** row1, const char** row2, int columns1, int columns2) {
    if (row1 == NULL || row2 == NULL) {
        fprintf(stderr, "Invalid row data.\n");
        return 0.0;
    }

    if (columns1 == 0 || columns2 == 0) {
        fprintf(stderr, "Error: Invalid column count (columns1=%d, columns2=%d)\n", columns1, columns2);
        return 0.0;
    }
    
    printf("Computing score for rows: %s vs %s\n", row1[0], row2[0]);

    double score = 0.0;
    for (int i = 0; i < columns1 && i < columns2; i++) {
        if (row1[i] == NULL || row2[i] == NULL) {
            fprintf(stderr, "Error: NULL data in column %d\n", i);
            continue;
        }
        if (strcmp(row1[i], row2[i]) == 0) {
            score += 1.0; // Increment score for matching fields
        }
    }
    return score / (columns1 > columns2 ? columns1 : columns2); // Normalize
}

// Thread function to compute matches
void* thread_compute_matches(void* args) {
    ThreadArgs* thread_args = (ThreadArgs*)args;

    for (int i = thread_args->start; i < thread_args->end; i++) {
        for (int j = 0; j < thread_args->data2.rows; j++) {
            // Validate rows before processing
            if (thread_args->data1.data[i] == NULL || thread_args->data2.data[j] == NULL) {
                fprintf(stderr, "Error: Invalid row data at row1[%d] or row2[%d]\n", i, j);
                continue;
            }

            // Calculate the number of columns for each row
            int columns1 = 0, columns2 = 0;
            while (thread_args->data1.data[i][columns1] != NULL) columns1++;
            while (thread_args->data2.data[j][columns2] != NULL) columns2++;

            // Skip rows with no valid columns
            if (columns1 == 0 || columns2 == 0) {
                fprintf(stderr, "Error: Row %d or Row %d has zero columns\n", i, j);
                continue;
            }

            double score = compute_score(
                (const char**)thread_args->data1.data[i],
                (const char**)thread_args->data2.data[j],
                columns1,
                columns2
            );

            // Add the match to the shared matches array
            pthread_mutex_lock(&mutex);
            thread_args->matches[*thread_args->match_index] = (Match){i, j, score};
            (*thread_args->match_index)++;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}

// Calculate compatibility scores
Match* calculate_matches(InputData data1, InputData data2, int* match_count) {
    const int thread_count = 4; // Number of threads
    pthread_t threads[thread_count];
    ThreadArgs thread_args[thread_count];

    // Allocate memory for matches
    int max_matches = data1.rows * data2.rows;
    Match* matches = malloc(sizeof(Match) * max_matches);
    if (!matches) {
        perror("Failed to allocate memory for matches");
        exit(EXIT_FAILURE);
    }   
    *match_count = 0;

    int rows_per_thread = (data1.rows + thread_count - 1) / thread_count;

    // Initialize threads
    for (int i = 0; i < thread_count; i++) {
        printf("Thread %d: start = %d, end = %d\n", i, thread_args[i].start, thread_args[i].end);
        thread_args[i].data1 = data1;
        thread_args[i].data2 = data2;
        thread_args[i].matches = matches;
        thread_args[i].match_index = match_count;
        thread_args[i].start = i * rows_per_thread;
        thread_args[i].end = (i + 1) * rows_per_thread;

        // Ensure the last thread doesn't go out of bounds
        if (thread_args[i].end > data1.rows) {
            thread_args[i].end = data1.rows;
        }

        // Avoid empty ranges
        if (thread_args[i].start >= thread_args[i].end) {
            thread_args[i].start = thread_args[i].end = 0;
        }

        printf("Thread %d: start = %d, end = %d\n", i, thread_args[i].start, thread_args[i].end);

        pthread_create(&threads[i], NULL, thread_compute_matches, &thread_args[i]);    
    }

    // Wait for threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    if (matches == NULL) {
        fprintf(stderr, "Failed to allocate memory for matches.\n");
        return NULL;
    }

    return matches;
}

// Free memory for matches
void free_matches(Match* matches) {
    if (matches != NULL) {
        free(matches); // Ensure matches was allocated
    }
}
