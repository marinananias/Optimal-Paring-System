/**
 * @file synchronization.h
 * @brief Header file for thread synchronization utilities.
 *
 * Declares functions for managing mutexes in multi-threaded applications.
 *
 * Dependencies:
 * - `pthread.h`: Provides the POSIX threading API for working with mutexes.
 *
 * Notes:
 * - The caller is responsible for declaring and allocating the mutex.
 * - Mutexes must be destroyed after use to avoid resource leaks.
 * - Ensure no threads are using the mutex before calling `destroy_mutex`.
 */
#ifndef SYNCHRONIZATION_H
#define SYNCHRONIZATION_H

#include <pthread.h>

// Declare Functions
void init_mutex(pthread_mutex_t *mutex);
void destroy_mutex(pthread_mutex_t *mutex);

#endif
