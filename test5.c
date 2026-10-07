#include <stdio.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main() {
    char *p = malloc(24);

    // Write right up to the boundary
    for (int i = 0; i < 24; i++) p[i] = 0xAA;

    // Now allocate another block — if metadata was corrupted, allocator will misbehave
    void *q = malloc(24);

    printf("Header integrity test completed.\n");

    free(p);
    free(q);
}