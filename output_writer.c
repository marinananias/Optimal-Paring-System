#include <stdio.h>
#include <stdlib.h>
#include "output_writer.h"

void write_matches_to_csv(const char* filename, Match* matches, int match_count, InputData data1, InputData data2) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Write header
    fprintf(file, "Name,Name,Match,Score\n");

    // Write matches
    for (int i = 0; i < match_count; i++) {
        fprintf(file, "%s,%s,%s,%.2f\n",
                data1.data[matches[i].row1][0], data2.data[matches[i].row2][0], 
                data2.data[matches[i].row2][1], matches[i].score);
    }

    fclose(file);
    printf("Results written to %s\n", filename);
}
