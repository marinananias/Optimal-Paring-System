#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_parser.h"
#include "matching_engine.h"
#include "output_writer.h"

int main(int argc, char** argv) {
	// Make sure we have all the arguments we need
	if (argc != 3) {
		fprintf(stderr, "Expected input format: ./main file-name1.csv file-name2.csv\n");
		return EXIT_FAILURE;
	}

	const char *file1 = argv[1];
	const char *file2 = argv[2];

	// Parse input data from the first file
	InputData data1 = parse_input_file(file1);
	if (!data1.valid) {
		fprintf(stderr, "Error: Invalid input data in file '%s'.\n", file1);
		return EXIT_FAILURE;
	}

	// Parse input data from the second file
	InputData data2 = parse_input_file(file2);
	if (!data2.valid) {
		fprintf(stderr, "Error: Invalid input data in file '%s'.\n", file2);
		return EXIT_FAILURE;
	}

	// Perform matching
	int match_count = 0;
	Match* matches = calculate_matches(data1, data2, &match_count);
	if (!matches) {
		fprintf(stderr, "Error: Failed to calculate matches.\n");
		free_input_data(data1);
		free_input_data(data2);
		return EXIT_FAILURE;
	}

	// Print matches
	printf("Matches:\n");
	for (int i = 0; i < match_count; i++) {
		printf("Row1: %s (Needs: %s), Row2: %s (Expertise: %s), Score: %.2f\n",
						data1.data[matches[i].row1][0], data1.data[matches[i].row1][1],
						data2.data[matches[i].row2][0], data2.data[matches[i].row2][1],
						matches[i].score);
	}

	// Write matches to a CSV file
	const char* output_file = "matches_output.csv";
	write_matches_to_csv(output_file, matches, match_count, data1, data2);

	// Clean up
	free_matches(matches);
	free_input_data(data1);
	free_input_data(data2);

	return EXIT_SUCCESS;
}
