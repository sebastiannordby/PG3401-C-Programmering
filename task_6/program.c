#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH_LINE_CODE 2048
#define MAX_OUTPUT_FILE_NAME_LEN 256

void convert_line_to_hungarian_notation(char *line);
void convert_line_spaces_with_tab(char *line);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Invalid number of arguments.\r\n");
        printf("Usage: %s <filename.c>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input_file_name = argv[1];
    if(strlen(input_file_name) < 3) {
        printf("Specified filename is to short to be a file.\r\n");
    }

    char output_file_name[MAX_OUTPUT_FILE_NAME_LEN];

    // New file name will be modified_<originalname>
    sprintf(output_file_name, "modified_%s", input_file_name);

    FILE *input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        perror("Error opening the provided file");
        return EXIT_FAILURE;
    }

    FILE *output_file = fopen(output_file_name, "w");
    if (output_file == NULL) {
        perror("Error creating output file");
        fclose(input_file);
        return EXIT_FAILURE;
    }

    char line[MAX_LENGTH_LINE_CODE];
    while (fgets(line, MAX_LENGTH_LINE_CODE, input_file) != NULL) {
        convert_line_spaces_with_tab(line); // Insert tab where applicable
        fputs(line, output_file); // Output result to file
    }

    fclose(input_file);
    fclose(output_file);
    printf("Code beautification complete.\n");

    return EXIT_SUCCESS;
}

// Convert three spaces with tab on a given line.
void convert_line_spaces_with_tab(char *line) {
    int current_char, curr_with_replace;
    int len = strlen(line);

    // Loop over the line.
    for(current_char = 0, curr_with_replace = 0; current_char < len; current_char++, curr_with_replace++)
    {
        // If the current char is whitespace, and its not the last on the line,
        // check if there is two more spaces where is three spaces in a row. 
        if(line[current_char] == ' ' && current_char + 1 < len && line[current_char + 1] == ' ' && 
            current_char + 2 < len && line[current_char + 2] == ' ')
        {
            line[curr_with_replace] = '\t';
            current_char += 2; // Skip past the two extra whitespaces
        }
        else
        {
            // If not we can just keep the character as is.
            line[curr_with_replace] = line[current_char];
        }
    }

    // End the line at j, j should be the counter when spaces replaced 
    // with a tab character
    line[curr_with_replace] = '\0';
}

