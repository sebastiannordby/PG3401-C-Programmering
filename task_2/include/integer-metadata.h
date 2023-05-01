#ifndef __INTEGER_METADATA_H__
#define __INTEGER_METADATA_H__
#include <stdbool.h>
/*
    INT_METADATA holds information about a integer.
    This struct was provided in the exam text, but i
    changed it to a typedef struct for simpler use.

    This was the original struct provided in the exam text.
    I did some modifications to fit my conventions.
    struct OPPGAVE2_TALL_METADATA {
        int iIndex; // Rekkefølge i filen, første tall = 1
        int iNumber; // Tallet, som lest fra input filen
        bool bIsFibonacci;
        bool bIsPrimeNumber;
        bool bIsSquareNumber;
        bool bIsCubeNumber;
        bool bIsPerfectNumber;
        bool bIsAbundantNumber;
        bool bIsDeficientNumber;
        bool bIsOddNumber;
    }
*/
typedef struct _INT_METADATA {
    int index;
    int number;
    bool is_fibonacci;
    bool is_prime;
    bool is_square;
    bool is_cube;
    bool is_perfect;
    bool is_abdundant;
    bool is_deficient;
    bool is_odd;
} INT_METADATA;

/*
    IntMetadataNode is a node to create a linked list with.
    This way there is no need to keep track of how many records
    of INT_METADATA is created, and you can simply go through the next node
    as long as it's not NULL.
*/
typedef struct IntMetadataNode {
    INT_METADATA data;
    struct IntMetadataNode *next;
} IntMetadataNode;
#endif