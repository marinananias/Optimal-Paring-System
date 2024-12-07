#include <stdio.h>
#include <stdlib.h>
#include "../input_parser.h"
#include "../matching_engine.h"
#include "../output_writer.h"

int main() {
    // Test input data for InputData structures
    InputData data1 = {
        .data = (char**[]) {
            (char*[]) {"John", "Math", NULL},
            (char*[]) {"Jane", "Science", NULL},
            (char*[]) {"Jack", "History", NULL},
            NULL
        },
        .rows = 3,
        .valid = 1
    };

    InputData data2 = {
        .data = (char**[]) {
            (char*[]) {"Alice", "Math", NULL},
            (char*[]) {"Bob", "Science", NULL},
            (char*[]) {"Charlie", "History", NULL},
            NULL
        },
        .rows = 3,
        .valid = 1
    };

    // Test matches
    Match matches[] = {
        {0, 0, 0.50}, // John vs Alice
        {1, 1, 0.50}, // Jane vs Bob
        {2, 2, 0.50}  // Jack vs Charlie
    };
    int match_count = 3;

    // Output file name
    const char* output_file = "test_output.csv";

    // Write matches to CSV
    write_matches_to_csv(output_file, matches, match_count, data1, data2);

    // Notify user to check the output file
    printf("Test complete. Check %s for results.\n", output_file);

    return EXIT_SUCCESS;
}
