#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILENAME_LEN 256
#define MAX_LINE_LEN 1024

void replace_while_with_for(char *line);
void apply_hungarian_notation(char *line);
void replace_three_spaces_with_tab(char *line);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename.c>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char output_filename[MAX_FILENAME_LEN];
    sprintf(output_filename, "%s_beautified.c", filename);

    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    FILE *output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        return 1;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, input_file) != NULL) {
        replace_while_with_for(line);
        apply_hungarian_notation(line);
        replace_three_spaces_with_tab(line);
        fputs(line, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    printf("Code beautification complete.\n");

    return 0;
}

void replace_while_with_for(char *line) {
    char *while_ptr = strstr(line, "while ");
    if (while_ptr != NULL && isspace(*(while_ptr - 1))) {
        char *open_paren_ptr = strchr(while_ptr, '(');
        if (open_paren_ptr != NULL) {
            char *close_paren_ptr = strrchr(line, ')');
            if (close_paren_ptr != NULL && close_paren_ptr > open_paren_ptr) {
                char *open_brace_ptr = strchr(close_paren_ptr, '{');
                if (open_brace_ptr != NULL) {
                    *while_ptr = 'f';
                    *(while_ptr + 1) = 'o';
                    *(while_ptr + 2) = 'r';
                    memmove(open_paren_ptr + 1, open_paren_ptr, close_paren_ptr - open_paren_ptr + 1);
                    sprintf(open_paren_ptr, "%s;", line);
                    *(open_brace_ptr - 1) = ')';
                    memmove(open_brace_ptr, open_brace_ptr + 1, strlen(open_brace_ptr));
                }
            }
        }
    }
}

void apply_hungarian_notation(char *line) {
    char *unsigned_int_ptr = strstr(line, "unsigned int ");
    if (unsigned_int_ptr != NULL && isspace(*(unsigned_int_ptr - 1))) {
        char *space_ptr = strchr(unsigned_int_ptr, ' ');
        if (space_ptr != NULL) {
            int name_len = strlen(space_ptr + 1);
            char *new_name = malloc(name_len + 3);
            sprintf(new_name, "ui%s", space_ptr + 1);
            strncpy(space_ptr + 1, new_name, name_len);
            free(new_name);
        }
    }
}

void replace_three_spaces_with_tab(char *line) {
    int i, j;
    int len = strlen(line);

    for(i = 0, j = 0; i < len; i++, j++)
    {
        if(line[i] == ' ' && i+1 < len && line[i+1] == ' ' && i+2 < len && line[i+2] == ' ')
        {
            line[j] = '\t';
            i += 2;
        }
        else
        {
            line[j] = line[i];
        }
    }

    line[j] = '\0';
}
