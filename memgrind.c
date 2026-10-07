#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include "mymalloc.h"

void workload1() {
    int sizes[] = {8, 16, 32, 64, 128, 512, 1024};
    void *ptrs[7];

    // Allocate
    for (int i = 0; i < 7; i++) {
        ptrs[i] = malloc(sizes[i]);
        if (!ptrs[i]) {
            fprintf(stderr, "workload1: malloc failed for size %d\n", sizes[i]);
            return;
        }
    }

    // Free in reverse order
    for (int i = 6; i >= 0; i--) {
        free(ptrs[i]);
    }
}


void workload2() {
    void *ptrs[120];

    // Allocate 120 small objects (1 byte each)
    for (int i = 0; i < 120; i++) {
        ptrs[i] = malloc(1);
        if (!ptrs[i]) {
            fprintf(stderr, "workload2: malloc failed at index %d\n", i);
            return;
        }
    }

    // Free in same order
    for (int i = 0; i < 120; i++) {
        free(ptrs[i]);
    }
}

void workload3() {
    void *ptrs[120];
    int allocated = 0;   // number of currently allocated objects
    int total_allocs = 0;

    // Initialize array
    for (int i = 0; i < 120; i++) ptrs[i] = NULL;

    while (total_allocs < 120) {
        int action = rand() % 2;  // 0 = allocate, 1 = free

        if (action == 0) {
            // Allocate if we haven't hit 120 total allocations
            void *p = malloc(1);
            if (!p) {
                fprintf(stderr, "workload3: malloc failed\n");
                return;
            }

            ptrs[allocated++] = p;
            total_allocs++;

        } else if (allocated > 0) {
            // Free a random allocated object
            int idx = rand() % allocated;

            free(ptrs[idx]);

            // Move last pointer into freed slot
            ptrs[idx] = ptrs[allocated - 1];
            ptrs[allocated - 1] = NULL;

            allocated--;
        }
    }

    // Free any remaining allocated objects
    for (int i = 0; i < allocated; i++) {
        free(ptrs[i]);
    }
}

void workload4(){
    void *ptrs[8];
    //int allocated = 0;   // number of currently allocated objects
    //int total_allocs = 0;
    // Initialize array
    for (int i = 0; i < 8; i++){
        ptrs[i] = malloc(128);
    }
    for( int i=1;i<8;i+=2)
        free(ptrs[i]);// every odd block is freed

    free(ptrs[4]);
    ptrs[4]=malloc(129);
    for (int i = 0; i < 8; i+=2) {
        free(ptrs[i]);
    }
}


int main(int agrc, char** argv){
    for(int i = 0; i < 50; i++){
        struct timeval start, end;

        // Record the start time
        gettimeofday(&start, NULL);

        workload1();
        workload2();
        workload3();
        workload4();

        // Record the end time
        gettimeofday(&end, NULL);

        // Calculate total seconds and microseconds
        long seconds = end.tv_sec - start.tv_sec;
        long microseconds = end.tv_usec - start.tv_usec;
        
        printf("Run %d: %ld sec %ld microsec \n", i, seconds, microseconds);
    }
}