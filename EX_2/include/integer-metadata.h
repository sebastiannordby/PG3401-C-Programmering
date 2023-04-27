#ifndef __INTEGER_METADATA_H__
#define __INTEGER_METADATA_H__

/*
    INT_METADATA holds information about a integer.
    This struct was provided in the exam text, but i
    changed it to a typedef struct for simpler use.
*/
typedef struct _INT_METADATA {
    int iIndex;
    int iNumber;
    bool bIsFibonacci;
    bool bIsPrimeNumber;
    bool bIsSquareNumber;
    bool bIsCubeNumber;
    bool bIsPerfectNumber;
    bool bIsAbdundantNumber;
    bool bIsDeficientNumber;
    bool bIsOddNumber;
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