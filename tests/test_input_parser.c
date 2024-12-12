#include <stdio.h>
#include <stdbool.h>
#include "../input_parser.h"

void display_dataset(const DataSet *dataset) {
    for (int i = 0; i < dataset->row_count; i++) {
        DataRow row = dataset->rows[i];
        printf("Name: %s\n", row.name);
        printf("Attributes: ");
        for (int j = 0; j < row.attributes_count; j++) {
            printf("%s", row.attributes[j]);
            if (j < row.attributes_count - 1) {
                printf(", ");
            }
        }
        if (row.capacity > 0) {
            printf("\nCapacity: %d", row.capacity);
        }
        printf("\n\n");
    }
}

int main() {
    const char *mentee_file = "../inputs/mentees1.csv";
    const char *mentor_file = "../inputs/mentors1.csv";
    bool success;

    // Parse mentees CSV
    printf("Parsing Mentees CSV file: %s\n", mentee_file);
    DataSet *mentees = parse_csv(mentee_file, &success);
    if (!success) {
        printf("Failed to parse the Mentees CSV file.\n");
        return 1;
    }

    printf("\nParsed Mentees Data:\n");
    display_dataset(mentees);

    // Parse mentors CSV
    printf("Parsing Mentors CSV file: %s\n", mentor_file);
    DataSet *mentors = parse_csv(mentor_file, &success);
    if (!success) {
        printf("Failed to parse the Mentors CSV file.\n");
        free_dataset(mentees);
        return 1;
    }

    printf("\nParsed Mentors Data:\n");
    display_dataset(mentors);

    // Free the allocated memory
    free_dataset(mentees);
    free_dataset(mentors);
    printf("Memory successfully freed.\n");

    return 0;
}
