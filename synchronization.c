/**
 * @file synchronization.c
 * @brief Provides utilities for managing thread synchronization primitives.
 *
 * Implements helper functions to initialize and destroy mutexes,
 * ensuring thread-safe operations in multi-threaded applications.
 *
 * Dependencies:
 * - `synchronization.h`: Declares the interface for these utilities.
 * - `pthread.h`: Provides the POSIX threading API.
 */

#include "synchronization.h"
#include <stdio.h>

/**
 * @brief Initializes a mutex for thread synchronization.
 *
 * This function wraps the `pthread_mutex_init` function to initialize a mutex
 * and provides error handling.
 *
 * @param mutex Pointer to the mutex to be initialized.
 *
 * @note If the mutex initialization fails, an error message is printed to `stderr`.
 */
void init_mutex(pthread_mutex_t *mutex) {
    if (pthread_mutex_init(mutex, NULL) != 0) {
        perror("Failed to initialize mutex");
    }
} // init_mutex

/**
 * @brief Destroys a mutex and releases associated resources.
 *
 * This function wraps the `pthread_mutex_destroy` function to clean up a mutex
 * and provides error handling.
 *
 * @param mutex Pointer to the mutex to be destroyed.
 *
 * @note If the mutex destruction fails, an error message is printed to `stderr`.
 */
void destroy_mutex(pthread_mutex_t *mutex) {
    if (pthread_mutex_destroy(mutex) != 0) {
        perror("Failed to destroy mutex");
    }
} // destroy_mutex