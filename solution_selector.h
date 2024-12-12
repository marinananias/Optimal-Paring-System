/**
 * @file solution_selector.h
 * @brief Header file for the optimal solution selector module.
 *
 * Defines interface for selecting the optimal matches between
 * two datasets (e.g., mentees and mentors) based on compatibility scores. The
 * implementation uses permutation to evaluate all possible matchings
 * and find the combination that maximizes the total compatibility score.
 *
 * Dependencies:
 * - `input_parser.h`: Defines the `DataSet` and `DataRow` structures used for input data.
 * - `utils.h`: Assumed to contain utility functions for operations like memory management and debugging.
 *
 */
#ifndef SOLUTION_SELECTOR_H
#define SOLUTION_SELECTOR_H
#include "input_parser.h"

// Function Declarations
void select_optimal_matches(DataSet *mentees, DataSet *mentors, int *compatibility_scores, int **matches);
void permute(int *array, int start, int end, int **best_matches, int *best_score, 
             DataSet *mentees, DataSet *mentors, int *compatibility_scores);
void match_mentees_to_mentors(DataSet *mentees, DataSet *mentors, int **compatibility_scores);
void match_mentees_to_mentors_non_threaded(DataSet *mentees, DataSet *mentors, int **compatibility_scores);
void measure_threading_performance(DataSet *mentees, DataSet *mentors, int *compatibility_scores);

#endif // SOLUTION_SELECTOR_H