#include <stdio.h>
#include <stdlib.h>
#include "../input_parser.h"
#include "../matching_engine.h"
#include "../solution_selector.h"

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

    // Find optimal matches
    int optimal_match_count = 0;
    Match* optimal_matches = find_optimal_matches(data1, data2, &optimal_match_count);

    // Print optimal matches
    printf("Optimal Matches:\n");
    for (int i = 0; i < optimal_match_count; i++) {
        printf("Row1: %s (Needs: %s), Row2: %s (Expertise: %s), Score: %.2f\n",
               data1.data[optimal_matches[i].row1][0], data1.data[optimal_matches[i].row1][1],
               data2.data[optimal_matches[i].row2][0], data2.data[optimal_matches[i].row2][1],
               optimal_matches[i].score);
    }

    // Clean up
    free(optimal_matches);

    return EXIT_SUCCESS;
}
