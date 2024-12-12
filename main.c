#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "input_parser.h"
#include "matching_engine.h"
#include "solution_selector.h"
#include "output_writer.h"

// Function to display usage instructions
void print_usage(const char *program_name) {
    printf("Usage: %s <category> <file1> <file2>\n", program_name);
    printf("Categories:\n");
    printf("  mentee_mentor     Match mentors and mentees (with capacity constraints)\n");
    printf("  participant_panel Match participants and panels/initiatives (no constraints)\n");
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return 1;
    }

    const char *category = argv[1];
    const char *file1 = argv[2];
    const char *file2 = argv[3];
    bool success1;
    bool success2;

    // Parse the first input file
    printf("Parsing input file: %s\n", file1);
    DataSet *dataset1 = parse_csv(file1, &success1);
    if (!success1) {
        printf("Error: Failed to parse input file: %s\n", file1);
        return 1;
    }

    // Parse the second input file
    printf("Parsing input file: %s\n", file2);
    DataSet *dataset2 = parse_csv(file2, &success2);
    if (!success2) {
        printf("Error: Failed to parse input file: %s\n", file2);
        free_dataset(dataset1);
        return 1;
    }

    //////////////////////////////////////////////////////
    // printf("Parsed %d mentees:\n", dataset1->row_count);
    // for (int i = 0; i < dataset1->row_count; i++) {
    //     printf("Mentee: %s, Attributes: ", dataset1->rows[i].name);
    //     for (int j = 0; j < dataset1->rows[i].attributes_count; j++) {
    //         printf("%s ", dataset1->rows[i].attributes[j]);
    //     }
    //     printf("\n");
    // }

    // printf("Parsed %d mentors:\n", dataset2->row_count);
    // for (int i = 0; i < dataset2->row_count; i++) {
    //     printf("Mentor: %s, with Capacity: %d; Attributes: ", dataset2->rows[i].name, dataset2->rows[i].capacity);
    //     for (int j = 0; j < dataset2->rows[i].attributes_count; j++) {
    //         printf("%s ", dataset2->rows[i].attributes[j]);
    //     }
    //     printf("\n");
    // }
    //////////////////////////////////////////////////////

    int *compatibility_scores = NULL;
    int *matches = NULL;

    // Handle matching based on category
    if (strcmp(category, "mentee_mentor") == 0) {
        printf("Starting matching process for mentee_mentor...\n");
        match_mentees_to_mentors(dataset1, dataset2, &compatibility_scores);
        select_optimal_matches(dataset1, dataset2, compatibility_scores, &matches);
        printf("Matching completed.\n");
    } else if (strcmp(category, "participant_panel") == 0) {
        printf("Starting matching process for participant_panel...\n");
        match_participants_to_panels(dataset1, dataset2, &compatibility_scores);
        matches = NULL; // Not used for participant_panel
        printf("Matching completed.\n");
    } else {
        print_usage(argv[0]);
        free_dataset(dataset1);
        free_dataset(dataset2);
        return EXIT_FAILURE;
    }

    // Write the output
    if (!write_output_file("output.csv", dataset1, dataset2, matches, compatibility_scores, (strcmp(category, "participant_panel") == 0))) {
        fprintf(stderr, "Error writing output file.\n");
        free_dataset(dataset1);
        free_dataset(dataset2);
        free(compatibility_scores);
        free(matches);
        return EXIT_FAILURE;
    }

    // Free allocated memory
    free(compatibility_scores);
    free(matches);
    free_dataset(dataset1);
    free_dataset(dataset2);

    printf("Program completed successfully.\n");
    return 0;
}
