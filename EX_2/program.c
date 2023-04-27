#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "integer-utils.h"
#include "integer-metadata.h"
#define INPUT_FILE_NAME "input.txt"
#define OUTPUT_FILE_NAME "output.txt"

void print_struct(INT_METADATA *st) {
    printf("Index: %d\r\n", st->iIndex);
    printf("Number: %d\r\n", st->iNumber);
    printf("bIsFibonacci: %d\r\n", st->bIsFibonacci);
    printf("bIsPrimeNumber: %d\r\n", st->bIsPrimeNumber);
    printf("bIsSquareNumber: %d\r\n", st->bIsSquareNumber);
    printf("bIsCubeNumber: %d\r\n", st->bIsCubeNumber);
    printf("bIsPerfectNumber: %d\r\n", st->bIsPerfectNumber);
    printf("bIsAbdundantNumber: %d\r\n", st->bIsAbdundantNumber);
    printf("bIsDeficientNumber: %d\r\n", st->bIsDeficientNumber);
    printf("bIsOddNumber: %d\r\n", st->bIsOddNumber);
    printf("\r\n");
}

/*
    Reads metadata from a file with path of INPUT_FILE_NAME.
    The input file can only contain numbers.
*/
IntMetadataNode* readMetaData() {
    FILE *input = fopen(INPUT_FILE_NAME, "r");
    int numbers_size, num;

    if(input == NULL) {
        printf("Could not find file %s\r\n", INPUT_FILE_NAME);
        return NULL;
    }

    IntMetadataNode* head = NULL;
    IntMetadataNode* current = NULL;

    while (fscanf(input, "%d", &num) > 0) {
        IntMetadataNode* node = (IntMetadataNode*) malloc(sizeof(IntMetadataNode));
        node->data.iIndex = numbers_size;
        node->data.iNumber = num;
        node->data.bIsAbdundantNumber = isAbundantNumber(num);
        node->data.bIsCubeNumber = isCubeNumber(num);
        node->data.bIsDeficientNumber = isDeficientNumber(num);
        node->data.bIsFibonacci = isFibonacci(num);
        node->data.bIsOddNumber = isOdd(num);
        node->data.bIsPerfectNumber = isPerfectNumber(num);
        node->data.bIsPrimeNumber = isPrime(num);
        node->data.bIsSquareNumber = isSquareNumber(num);
        node->next = NULL;

        if (head == NULL) { // First element must be head
            head = node;
            current = node;
        } else { // not first element, append to back.
            current->next = node;
            current = node;
        }

        numbers_size++;
    }

    fclose(input);

    return head;
}

/*
    Converts any integer to a binary as a string.
    INT_MAX when converted to binary can only be 32 characters long,
    hence the size of 33, because with need the NULL terminator.
    The downside with this function is that it converts all integers 
    into a 32(33) bit long string.
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
void write_meta_data(IntMetadataNode* metaData) {
    if(metaData == NULL)
        return;

    FILE *output = fopen(OUTPUT_FILE_NAME, "w+");
    if(output == NULL) {
        printf("Could not open/create file %s\r\n", OUTPUT_FILE_NAME);
        return;
    }

    IntMetadataNode* current = metaData;

    // Iterate over the linked list
    while (current != NULL) {
        INT_METADATA data = current->data;
        printf("Index: %d, Number: %d, Is Abundant Number: %d\n", 
            data.iIndex, data.iNumber, data.bIsAbdundantNumber);
        // Access other metadata fields here

        char *format = "Index: %03d\tNumber: %32s\tabun: %d\tcube: %d\tdef: %d\tfib: %d\todd: %d\t pert: %d\tprime: %d\tsqrt: %d\r\n";

        fprintf(output, format,
            data.iIndex, int_to_binary(data.iNumber), data.bIsAbdundantNumber,  
            data.bIsCubeNumber, data.bIsDeficientNumber, data.bIsFibonacci, 
            data.bIsOddNumber, data.bIsPerfectNumber, data.bIsPrimeNumber, data.bIsSquareNumber);

        current = current->next;
    }

    fclose(output);
    output = NULL;
}

int main(void) {
    // Read meta data from file
    IntMetadataNode* metaData = readMetaData();

    // Could not read data
    if(metaData == NULL){
        return 1;
    }

    // Output meta data to file
    write_meta_data(metaData);

    // Free memory allocated to store meta data
    IntMetadataNode* temp;
    while (metaData != NULL) {
        temp = metaData;
        metaData = metaData->next;
        free(temp);
    }

    return 0;
}