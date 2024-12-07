#ifndef OUTPUT_WRITER_H
#define OUTPUT_WRITER_H

#include "matching_engine.h"

// Function to write matches to a CSV file
void write_matches_to_csv(const char* filename, Match* matches, int match_count, InputData data1, InputData data2);

#endif
