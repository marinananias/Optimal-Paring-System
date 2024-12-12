/**
 * @file output_writer.c
 * @brief Implementation for writing match results to a CSV file.
 *
 * Outputs the results of the matching process to a CSV file. The results
 * include mentees or participants, their matched mentors or panels, and the
 * compatibility scores.
 *
 * Dependencies:
 * - `output_writer.h`: Declares the interface for this functionality.
 * - `input_parser.h`: Provides definitions for `DataSet` and `DataRow`.
 * - `solution_selector.h`: Provides the matching indices.
 * - `matching_engine.h`: Provides the `calculate_score` function to compute compatibility scores.
 *
 * Functionality:
 * - Outputs a header row in the CSV file.
 * - Writes each mentee or participant, their matched mentor or panel, and the compatibility score.
 * - Handles cases where no valid match is found.
 *
 * Example Output:
 * Mentee/Participant,Mentor/Panel,Compatibility Score
 * John Doe,Jane Smith,10
 * Alice Johnson,No Match,0
 */
#include <stdio.h>
#include <stdlib.h>
#include "output_writer.h"
#include "input_parser.h"
#include "solution_selector.h"
#include "matching_engine.h"

/**
 * @brief Writes mentee-to-mentor matches to the output file.
 *
 * Handles the `mentee_mentor` category, writing the best matches for each mentee
 * based on the provided matches array and compatibility scores.
 */
static void write_mentee_mentor_matches(FILE *file, DataSet *mentees, DataSet *mentors, int *matches) {
    fprintf(file, "Mentee,Mentor,Compatibility Score\n");

    for (int i = 0; i < mentees->row_count; i++) {
        int match_index = matches[i];
        if (match_index >= 0 && match_index < mentors->row_count) { // Validate mentor index
            fprintf(file, "%s,%s,%d\n",
                    mentees->rows[i].name,
                    mentors->rows[match_index].name,
                    calculate_score(&mentees->rows[i], &mentors->rows[match_index]));
        } else { // No valid match
            fprintf(file, "%s,No Match,0\n", mentees->rows[i].name);
        }
    }
} // write_mentee_mentor_matches

/**
 * @brief Writes all participant-to-panel matches with positive scores.
 *
 * Handles the `participant_panel` category, writing all matches where the compatibility
 * score is greater than zero, without restrictions.
 */
static void write_participant_panel_matches(FILE *file, DataSet *participants, DataSet *panels, int *compatibility_scores) {
    fprintf(file, "Participant,Panel,Compatibility Score\n");

    for (int i = 0; i < participants->row_count; i++) {
        for (int j = 0; j < panels->row_count; j++) {
            int score = compatibility_scores[i * panels->row_count + j];
            if (score > 0) {
                fprintf(file, "%s,%s,%d\n",
                        participants->rows[i].name,
                        panels->rows[j].name,
                        score);
            }
        }
    }
} // write_participant_panel_matches

/**
 * @brief Writes the matching results to a CSV file.
 *
 * This function writes mentee-to-mentor (or participant-to-panel) matching results,
 * including compatibility scores, to a specified CSV file. It handles both cases
 * (`mentee_mentor` and `participant_panel`) based on the matches array and compatibility
 * scores provided.
 *
 * @param filename Path to the output file where results will be written.
 * @param dataset1 Pointer to the dataset of mentees or participants.
 * @param dataset2 Pointer to the dataset of mentors or panels.
 * @param matches Array of indices representing the best matches for each mentee or participant (optional for `participant_panel`).
 * @param compatibility_scores Compatibility scores array for all mentee/panel pairings.
 * @param is_participant_panel Boolean flag to indicate `participant_panel` mode.
 * @return 1 on success, 0 on failure (e.g., file write error).
 */
int write_output_file(const char *filename, DataSet *dataset1, DataSet *dataset2, int *matches, int *compatibility_scores, bool is_participant_panel) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open output file");
        return 0;
    }

    if (is_participant_panel) {
        write_participant_panel_matches(file, dataset1, dataset2, compatibility_scores);
    } else {
        write_mentee_mentor_matches(file, dataset1, dataset2, matches);
    }

    fclose(file);
    return 1;
} // write_output_file

/**
 * @brief Analyzes panel popularity by counting matches for each panel.
 *
 * This function counts how many participants were matched to each panel
 * and writes the results to a CSV file.
 *
 * @param filename Path to the output file for panel popularity analysis.
 * @param mentors Dataset of panels/initiatives.
 * @param compatibility_scores Array of compatibility scores between participants and panels.
 * @param num_participants Number of participants.
 */
void analyze_panel_popularity(const char *filename, DataSet *mentors, int *compatibility_scores, int num_participants) {
    int *panel_counts = calloc(mentors->row_count, sizeof(int));

    // Count matches for each panel
    for (int i = 0; i < num_participants; i++) {
        for (int j = 0; j < mentors->row_count; j++) {
            int score = compatibility_scores[i * mentors->row_count + j];
            if (score > 0) {
                panel_counts[j]++;
            }
        }
    }

    // Write results to CSV
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open panel popularity file");
        free(panel_counts);
        return;
    }

    fprintf(file, "Panel,Number of Matches\n");
    for (int i = 0; i < mentors->row_count; i++) {
        fprintf(file, "%s,%d\n", mentors->rows[i].name, panel_counts[i]);
    }

    fclose(file);
    free(panel_counts);
} // analyze_panel_popularity
