/**
 * @file output_writer.h
 * @brief Header file for writing match results to an output CSV file.
 *
 * Dependencies:
 * - `input_parser.h`: Provides definitions for the `DataSet` structure.
 * - `solution_selector.h`: Provides the matching logic and the `matches` array.
 * - `utils.h`: Assumed to include utility functions for file handling and debugging.
 *
 * Notes:
 * - The caller is responsible for ensuring the datasets (`mentees` and `mentors`)
 *   and `matches` array are properly initialized.
 * - The function returns 1 on success and 0 on failure, allowing the caller to handle errors.
 */
#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H
#include "input_parser.h"
#include "solution_selector.h"

// Declare Function
int write_output_file(const char *filename, DataSet *mentees, DataSet *mentors, int *matches, int *compatibility_scores, bool is_participant_panel);
void analyze_panel_popularity(const char *filename, DataSet *mentors, int *compatibility_scores, int num_participants);

#endif