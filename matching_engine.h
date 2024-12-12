/**
 * @file matching_engine.h
 * @brief Header file for the multi-threaded matching engine.
 *
 * Declares functions and structures used for matching individuals
 * (e.g., mentees to mentors or participants to panels) based on compatibility scores.
 * The implementation leverages multi-threading to compute compatibility scores efficiently.
 *
 * Dependencies:
 * - `input_parser.h`: Defines the `DataSet` and `DataRow` structures used for data representation.
 * - `synchronization.h`: Provides functionality for mutexes and shared resource synchronization.
 */
#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include "input_parser.h"
#include "synchronization.h"

// Function Declarations
int calculate_score(DataRow *a, DataRow *b);
void match_datasets(DataSet *dataset1, DataSet *dataset2, int **compatibility_scores);

#endif // MATCHING_ENGINE_H