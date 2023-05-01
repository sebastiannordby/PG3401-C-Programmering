#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "integer-utils.h"
#include "integer-metadata.h"
#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"

char* int_to_binary(int num);
void print_meta_data(const INT_METADATA *st);
void write_to_file(const IntMetadataNode* meta_data);
IntMetadataNode* initialize_from_file();

int main(void) {
    // Read meta data from file
    IntMetadataNode* meta_data = initialize_from_file();

    // Could not read data
    if(meta_data == NULL){
        return 1;
    }

    // Output meta data to file
    write_to_file(meta_data);

    // Free memory allocated to store meta data
    IntMetadataNode* temp;
    while (meta_data != NULL) {
        temp = meta_data;
        meta_data = meta_data->next;
        free(temp);
    }

    return 0;
}

// Prints out the meta data formated.
// Index and number is spaced with 10 to fit all integers.
// Binary is spaced to 32 to fit all integers converted to binary.
void print_meta_data(const INT_METADATA *meta_data) {
    char *format = "Index: %-10d Number: %-10d Binary: %32s abun: %d cube: %d def: %d fib: %d odd: %d pert: %d prime: %d sqrt: %d\r\n";

    printf(format,
        meta_data->index, 
        meta_data->number,
        int_to_binary(meta_data->number), 
        meta_data->is_abdundant,  
        meta_data->is_cube, 
        meta_data->is_deficient, 
        meta_data->is_fibonacci, 
        meta_data->is_odd, 
        meta_data->is_perfect, 
        meta_data->is_prime, 
        meta_data->is_square);
}

/*
    Reads metadata from a file with path of INPUT_FILE_NAME.
    The input file can only contain numbers.
*/
IntMetadataNode* initialize_from_file() {
    FILE *input = fopen(INPUT_FILE_NAME, "r");
    int numbers_size, num;

    if(input == NULL) {
        printf("Could not find file %s\r\n", INPUT_FILE_NAME);
        return NULL;
    }

    IntMetadataNode* head = NULL;
    IntMetadataNode* current = NULL;

    printf("Initializing metadata:\r\n");

    while (fscanf(input, "%d", &num) > 0) {
        IntMetadataNode* node = (IntMetadataNode*) malloc(sizeof(IntMetadataNode));
        node->data.index = numbers_size;
        node->data.number = num;
        node->data.is_abdundant = is_abundant_number(num);
        node->data.is_cube = is_cube_number(num);
        node->data.is_deficient = is_deficient_number(num);
        node->data.is_fibonacci = is_fibonacci(num);
        node->data.is_odd = is_odd(num);
        node->data.is_perfect = is_perfect_number(num);
        node->data.is_prime = is_prime(num);
        node->data.is_square = is_square_number(num);
        node->next = NULL;

        if (head == NULL) { // First element must be head
            head = node;
            current = node;
        } else { // not first element, append to back.
            current->next = node;
            current = node;
        }

        print_meta_data(&node->data);

        numbers_size++;
    }

    fclose(input);

    printf("Metadata successfully created.\r\n");

    return head;
}

/*
    Converts any integer to a binary as a string.
    The biggest integer value INT_MAX when converted to binary can only be 32 characters long,
    hence the size of 33, because with need the NULL terminator.
    The downside with this function is that it converts all integers 
    into a 32(33) bit long string.

    This conversion work by shifting the number to the right by i,
    and this returns the bit in number at i position. Then the
    '&' (and) operator is used to subract the least significant.
    When the bit is extracted the bit is converted to ASCII character representation
    by adding character '0', which is 48 in ascii. 
    Since '1' is 49 in ASCII, and the bit is either 1 or 0, we get the correct representation.
*/
char* int_to_binary(int num) {
    char* binary = malloc(sizeof(char) * 33);
    memset(binary, '0', 32);
    binary[32] = '\0';

    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        binary[31 - i] = bit + '0';
    }

    return binary;
}

/*
    Outputs INT_METADATA provided in the linked list,
    to a file with the path of OUTPUT_FILE_NAME.
*/
void write_to_file(const IntMetadataNode* meta_data) {
    if(meta_data == NULL)
        return;

    FILE *output = fopen(OUTPUT_FILE_NAME, "w+");
    if(output == NULL) {
        printf("Could not open/create file %s\r\n", OUTPUT_FILE_NAME);
        return;
    }

    const IntMetadataNode* current = meta_data;

    // Iterate over the linked list
    while (current != NULL) {
        INT_METADATA data = current->data;

        char *format = "Index: %-10d Number: %-32s abun: %d cube: %d def: %d fib: %d odd: %d pert: %d prime: %d sqrt: %d\r\n";

        fprintf(output, format,
            data.index, 
            int_to_binary(data.number), 
            data.is_abdundant,  
            data.is_cube, 
            data.is_deficient, 
            data.is_fibonacci, 
            data.is_odd, 
            data.is_perfect, 
            data.is_prime, 
            data.is_square);

        current = current->next;
    }

    fclose(output);
    output = NULL;

    printf("Metadata successfully written to %s\r\n", OUTPUT_FILE_NAME);
}
