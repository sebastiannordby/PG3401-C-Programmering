#include <stdio.h>
#include <stdlib.h>
#include "integer-utils.h"
#define FILE_NAME "input.txt"
#define MAX_NUMBERS_IN_FILE 1000

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

int main() {
    FILE *fp = fopen(FILE_NAME, "r");
    int numbers[MAX_NUMBERS_IN_FILE];
    int numbers_size, num;

    if(fp == NULL) {
        printf("Could not find file %s\r\n", FILE_NAME);
        return 1;
    }

    INT_METADATA *metaData = malloc(sizeof(INT_METADATA) * MAX_NUMBERS_IN_FILE);

    while (fscanf(fp, "%d", &num) > 0) {
        numbers[numbers_size] = num;
        INT_METADATA *p = (metaData + numbers_size);
        p->iIndex = numbers_size;
        p->iNumber = num;
        p->bIsAbdundantNumber = isAbundantNumber(num);
        p->bIsCubeNumber = isCubeNumber(num);
        p->bIsDeficientNumber = isDeficientNumber(num);
        p->bIsFibonacci = isFibonacci(num);
        p->bIsOddNumber = isOdd(num);
        p->bIsPerfectNumber = isPerfectNumber(num);
        p->bIsPrimeNumber = isPrime(num);
        p->bIsSquareNumber = isSquareNumber(num);

        numbers_size++;

        print_struct(p);
    }

    for(int i = 0; i < numbers_size; i++) {
        printf("Number at line: %d = %d\r\n", i, numbers[i]);
        print_struct(metaData + i);
    }

    return 0;
}