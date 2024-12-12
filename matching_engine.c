/**
 * @file matching_engine.c
 * @brief Multi-threaded engine for calculating compatibility scores and matching individuals or groups.
 *
 * Implements a matching engine designed to compute compatibility scores between
 * two datasets (e.g., mentees and mentors or participants and panels). The engine uses
 * multi-threading to speed up the computation of compatibility scores.
 */
#include "matching_engine.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Uncomment to enable debug logs
// #define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

/**
 * @brief Structure to pass arguments to threads.
 * 
 * Contains the data necessary for each thread to compute compatibility scores
 * between one individual and all members of a group. It also includes a mutex
 * for thread-safe updates to shared data.
 */
typedef struct {
    DataRow *individual;         // Pointer to the individual being matched
    DataSet *group;              // Pointer to the group dataset
    int *compatibility_scores;   // Shared array of compatibility scores
    int index;                   // Index of the individual in the first dataset
    pthread_mutex_t *mutex;      // Mutex for thread-safe updates
} ThreadArgs;

/**
 * @brief Calculate the compatibility score between two individuals.
 *
 * Compatibility is determined by the number of matching attributes between
 * the two `DataRow` objects. Matching attributes are identified through string comparisons.
 *
 * @param a Pointer to the first DataRow (mentee/participant).
 * @param b Pointer to the second DataRow (mentor/panel).
 * @return The compatibility score as an integer.
 */
int calculate_score(DataRow *a, DataRow *b) {
    if (!a || !b) return 0; // Null check

    int score = 0;
    for (int i = 0; i < a->attributes_count; i++) {
        for (int j = 0; j < b->attributes_count; j++) {
            if (strcmp(a->attributes[i], b->attributes[j]) == 0) {
                DEBUG_PRINT("Match found: %s\n", a->attributes[i]);
                score++;
            }
        }
    }
    return score;
} // calculate_score

/**
 * @brief Thread function to compute compatibility scores for an individual.
 *
 * Computes compatibility scores for one individual against all members of a group.
 * Updates the shared compatibility scores array.
 *
 * @param args Pointer to the `ThreadArgs` structure containing thread-specific data.
 * @return Always returns NULL.
 */
void *compute_scores(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;
    if (!thread_args || !thread_args->individual || !thread_args->group) return NULL;
    
    for (int i = 0; i < thread_args->group->row_count; i++) {
        int score = calculate_score(thread_args->individual, &thread_args->group->rows[i]);
        pthread_mutex_lock(thread_args->mutex);
        thread_args->compatibility_scores[thread_args->index * thread_args->group->row_count + i] = score;
        pthread_mutex_unlock(thread_args->mutex);
    }
    return NULL;
} // compute_scores

/**
 * @brief Matches individuals from one dataset to another by calculating compatibility scores.
 *
 * This function computes compatibility scores between two datasets using multi-threading.
 * Each thread handles the compatibility calculation for one individual from the first dataset
 * against all individuals in the second dataset.
 *
 * @param dataset1 Pointer to the first dataset (e.g., mentees or participants).
 * @param dataset2 Pointer to the second dataset (e.g., mentors or panels).
 * @param compatibility_scores Pointer to an array to store compatibility scores.
 *                             The array is dynamically allocated and must be freed by the caller.
 */
void match_datasets(DataSet *dataset1, DataSet *dataset2, int **compatibility_scores) {
    if (!dataset1 || !dataset2 || !compatibility_scores) {
        fprintf(stderr, "Invalid inputs to match_datasets.\n");
        return;
    }

    *compatibility_scores = malloc(dataset1->row_count * dataset2->row_count * sizeof(int));
    if (!*compatibility_scores) {
        perror("Failed to allocate memory for compatibility scores");
        return;
    }

    pthread_t threads[dataset1->row_count];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Thread arguments
    ThreadArgs args[dataset1->row_count];
    for (int i = 0; i < dataset1->row_count; i++) {
        args[i] = (ThreadArgs){.individual = &dataset1->rows[i],
                               .group = dataset2,
                               .compatibility_scores = *compatibility_scores,
                               .index = i,
                               .mutex = &mutex};
        if (pthread_create(&threads[i], NULL, compute_scores, &args[i]) != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
        }    
    }

    // Join threads
    for (int i = 0; i < dataset1->row_count; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d\n", i);
        }    
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
} // match_datasets