#include <stdio.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main() {
    

    void* a = malloc(20);   // should allocate 24 bytes
    void* b = malloc(20);   // should allocate another 24 bytes



    // Fill each block with a unique pattern
    memset(a, 0xAA, 24);
    memset(b, 0xBB, 24);

    // Check that writing to a does not affect b
    int ok = 1;
    for (int i = 0; i < 24; i++) {
        if (((unsigned char*)b)[i] != 0xBB) ok = 0;
    }

    printf("Overlap test: %s\n", ok ? "PASS" : "FAIL");

    free(a);
    free(b);
}