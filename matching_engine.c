/**
 * @file matching_engine.c
 * @brief Multi-threaded engine for calculating compatibility scores and matching individuals or groups.
 *
 * Implements a matching engine designed to compute compatibility scores between
 * two datasets (e.g., mentees and mentors or participants and panels). The engine uses
 * multi-threading to speed up the computation of compatibility scores.
 *
 * Key Features:
 * - Compatibility Score Calculation
 * - Multi-threading for performance
 * - Mutex for synchronization
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
 * @param a Pointer to the first DataRow (e.g., mentee).
 * @param b Pointer to the second DataRow (e.g., mentor).
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
 * Updates the shared compatibility scores array in a thread-safe manner.
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
 * @brief Matches participants to panels by calculating compatibility scores.
 *
 * Similar to `match_mentees_to_mentors`, this function computes compatibility scores
 * for participants against panels using multi-threading.
 *
 * @param participants Pointer to the dataset of participants.
 * @param panels Pointer to the dataset of panels.
 * @param compatibility_scores Pointer to an array to store compatibility scores.
 *                             The array is dynamically allocated and must be freed by the caller.
 */
void match_participants_to_panels(DataSet *participants, DataSet *panels, int **compatibility_scores) {
    if (!participants || !panels || !compatibility_scores) {
        fprintf(stderr, "Invalid inputs to match_participants_to_panels.\n");
        return;
    }

    // Similar logic as match_mentees_to_mentors
    *compatibility_scores = malloc(participants->row_count * panels->row_count * sizeof(int));
    if (!*compatibility_scores) {
        perror("Failed to allocate memory for compatibility scores");
        return;
    }

    pthread_t threads[participants->row_count];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Thread arguments
    ThreadArgs args[participants->row_count];
    for (int i = 0; i < participants->row_count; i++) {
        args[i] = (ThreadArgs){.individual = &participants->rows[i],
                               .group = panels,
                               .compatibility_scores = *compatibility_scores,
                               .index = i,
                               .mutex = &mutex};
        pthread_create(&threads[i], NULL, compute_scores, &args[i]);
    }

    // Join threads
    for (int i = 0; i < participants->row_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
} // match_participants_to_panels

/**
 * @brief Matches mentees to mentors by calculating compatibility scores.
 *
 * Creates one thread per mentee to compute compatibility scores against all mentors.
 * The results are stored in a shared compatibility scores array.
 *
 * @param mentees Pointer to the dataset of mentees.
 * @param mentors Pointer to the dataset of mentors.
 * @param compatibility_scores Pointer to an array to store compatibility scores.
 *        The array is dynamically allocated and must be freed by the caller.
 */
void match_mentees_to_mentors(DataSet *mentees, DataSet *mentors, int **compatibility_scores) {
    if (!mentees || !mentors || !compatibility_scores) {
        fprintf(stderr, "Invalid inputs to match_mentees_to_mentors.\n");
        return;
    }
    *compatibility_scores = malloc(mentees->row_count * mentors->row_count * sizeof(int));
    if (!*compatibility_scores) {
        perror("Failed to allocate memory for compatibility scores");
        return;
    }

    pthread_t threads[mentees->row_count];
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    // Thread arguments
    ThreadArgs args[mentees->row_count];
    for (int i = 0; i < mentees->row_count; i++) {
        args[i] = (ThreadArgs){.individual = &mentees->rows[i],
                               .group = mentors,
                               .compatibility_scores = *compatibility_scores,
                               .index = i,
                               .mutex = &mutex}; // Pass the mutex for synchronization
        pthread_create(&threads[i], NULL, compute_scores, &args[i]);
    }

    // Join threads
    for (int i = 0; i < mentees->row_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);
} // match_mentees_to_mentors