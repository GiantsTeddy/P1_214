#include <stdio.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main() {
    char *p = malloc(16);
    for (int i = 0; i < 16; i++) p[i] = i;

    // Allocate and free other blocks
    void *q = malloc(32);
    free(q);

    // Check that p still contains the original data
    int ok = 1;
    for (int i = 0; i < 16; i++) {
        if (p[i] != i) ok = 0;
    }

    printf("Allocator writes-to-payload test: %s\n", ok ? "PASS" : "FAIL");

    free(p);
}