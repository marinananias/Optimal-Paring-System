#include <stdio.h>
#include <stdlib.h>
#include "../input_parser.h"
#include "../matching_engine.h"

int main() {
    // Test input data for InputData structures
    InputData data1 = {
        .data = (char**[]) {
            (char*[]) {"John", "Math|Science", NULL},    // John needs Math or Science
            (char*[]) {"Jane", "Science|History", NULL}, // Jane needs Science or History
            (char*[]) {"Jack", "History", NULL},         // Jack needs History
            NULL
        },
        .rows = 3,
        .valid = 1
    };

    InputData data2 = {
        .data = (char**[]) {
            (char*[]) {"Alice", "Math|Science", NULL},   // Alice offers Math or Science
            (char*[]) {"Bob", "Math", NULL},             // Bob offers Math
            (char*[]) {"Charlie", "History|Science", NULL}, // Charlie offers History or Science
            NULL
        },
        .rows = 3,
        .valid = 1
    };

    if (!data1.valid || !data2.valid) {
        fprintf(stderr, "Failed to parse input data.\n");
        return EXIT_FAILURE;
    }

    if (data1.data == NULL || data2.data == NULL) {
        fprintf(stderr, "Parsed data is NULL.\n");
        return EXIT_FAILURE;
    }

    // Perform matching
    int match_count = 0;
    Match* matches = calculate_matches(data1, data2, &match_count);

    // Print the matches
    printf("Matches:\n");
    for (int i = 0; i < match_count; i++) {
        printf("Row1: %s (Needs: %s), Row2: %s (Expertise: %s), Score: %.2f\n",
               data1.data[matches[i].row1][0], data1.data[matches[i].row1][1],
               data2.data[matches[i].row2][0], data2.data[matches[i].row2][1],
               matches[i].score);
    }

    // Clean up
    free_matches(matches);

    return 0;
}
