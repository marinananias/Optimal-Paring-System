#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "input_parser.h"
#include "matching_engine.h"
#include "solution_selector.h"
#include "output_writer.h"

/**
 * @brief Displays usage instructions
 * 
 * @param program_name
 */
void print_usage(const char *program_name) {
    printf("Usage: %s <category> <file1> <file2>\n", program_name);
    printf("Categories:\n");
    printf("  mentee_mentor     Match mentors and mentees (with capacity constraints)\n");
    printf("  participant_panel Match participants and panels/initiatives (no constraints)\n");
} // print_usage

/**
 * @brief Parses a dataset from a given file and handles errors.
 * 
 * @param file_path Path to the input file.
 * @param success Pointer to a boolean to indicate parsing success.
 * @return Pointer to the parsed dataset.
 */
DataSet *parse_dataset(const char *file_path, bool *success) {
    printf("Parsing input file: %s\n", file_path);
    DataSet *dataset = parse_csv(file_path, success);
    if (!*success) {
        fprintf(stderr, "Error: Failed to parse input file: %s\n", file_path);
    }
    return dataset;
} // parse_dataset

/**
 * @brief Frees allocated datasets and compatibility scores.
 *
 * @param dataset1 Pointer to the first dataset.
 * @param dataset2 Pointer to the second dataset.
 * @param compatibility_scores Pointer to the compatibility scores array.
 * @param matches Pointer to the matches array.
 */
void cleanup_resources(DataSet *dataset1, DataSet *dataset2, int *compatibility_scores, int *matches) {
    free(compatibility_scores);
    free(matches);
    free_dataset(dataset1);
    free_dataset(dataset2);
} // cleanup_resources

int main(int argc, char *argv[]) {
    if (argc != 4) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *category = argv[1];
    const char *file1 = argv[2];
    const char *file2 = argv[3];
    bool success1, success2;

    DataSet *dataset1 = parse_dataset(file1, &success1);
    if (!success1) return EXIT_FAILURE;

    DataSet *dataset2 = parse_dataset(file2, &success2);
    if (!success2) {
        free_dataset(dataset1);
        return EXIT_FAILURE;
    }

    int *compatibility_scores = NULL;
    int *matches = NULL;

    if (strcmp(category, "mentee_mentor") == 0) {
        // Measure threading performance
        printf("Measuring threading performance...\n");
        measure_threading_performance(dataset1, dataset2, compatibility_scores);
        printf("Threading performance measured.\n\n");

        // Run the matching process
        printf("Starting matching process for mentee_mentor...\n");
        match_datasets(dataset1, dataset2, &compatibility_scores);
        select_optimal_matches(dataset1, dataset2, compatibility_scores, &matches);
        printf("Matching completed.\n\n");
    } else if (strcmp(category, "participant_panel") == 0) {
        printf("Starting matching process for participant_panel...\n");
        match_datasets(dataset1, dataset2, &compatibility_scores);
        printf("Matching completed.\n\n");

        // Analyze panel popularity
        analyze_panel_popularity("panel_popularity.csv", dataset2, compatibility_scores, dataset1->row_count);
        printf("Panel popularity analysis written to panel_popularity.csv.\n");
    } else {
        print_usage(argv[0]);
        cleanup_resources(dataset1, dataset2, compatibility_scores, matches);
        return EXIT_FAILURE;
    }

    // Write results to the output file
    if (!write_output_file("output.csv", dataset1, dataset2, matches, compatibility_scores, strcmp(category, "participant_panel") == 0)) {
        fprintf(stderr, "Error writing output file.\n");
        cleanup_resources(dataset1, dataset2, compatibility_scores, matches);
        return EXIT_FAILURE;
    }

    cleanup_resources(dataset1, dataset2, compatibility_scores, matches);

    printf("Program completed successfully.\n");
    return EXIT_SUCCESS;
} // main
