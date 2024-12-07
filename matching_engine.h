#ifndef MATCHING_ENGINE_H
#define MATCHING_ENGINE_H

#include "input_parser.h"

// Struct to represent a compatibility score
typedef struct {
    int row1;     // Index of the first entity in the pair
    int row2;     // Index of the second entity in the pair
    double score; // Compatibility score
} Match;

// Function to calculate compatibility scores between two datasets
Match* calculate_matches(InputData data1, InputData data2, int* match_count);

// Function to free memory allocated for matches
void free_matches(Match* matches);

#endif