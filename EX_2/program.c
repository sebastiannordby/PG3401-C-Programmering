#include <stdio.h>
#include "integer-utils.h"

struct OPPGAVE2_TALL_METADATA {
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
} OPPGAVE2_TALL_METADATA;

int main() {
    struct OPPGAVE2_TALL_METADATA *d = malloc(sizeof(OPPGAVE2_TALL_METADATA));

    d->bIsAbdundantNumber = 0;

    free(d);
    d = NULL;

    return 0;
}