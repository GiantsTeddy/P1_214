#include <stdio.h>
#include <string.h>

#ifndef REALMALLOC
#include "mymalloc.h"
#endif

int main() {
    char *p = malloc(32);
    char *original = p;

    // Perform many allocations and frees
    for (int i = 0; i < 200; i++) {
        void *x = malloc(8);
        free(x);
    }

    // Check pointer identity
    printf("Pointer stability: %s\n", (p == original) ? "PASS" : "FAIL");

    free(p);
}
