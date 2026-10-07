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
    int allocatingSize=16;
    void *ptrs[allocatingSize];

    // Initialize array
    for (int i = 0; i < allocatingSize; i++){
        ptrs[i] = malloc(128);
    }
    for( int i=1;i<allocatingSize;i+=2)
        free(ptrs[i]);// every odd block is freed

    free(ptrs[4]);
    free(ptrs[6]);
    free(ptrs[12]);
    ptrs[4]=malloc(128*4);
    ptrs[12]=malloc(128+1);
    ptrs[6]=malloc(1);// just so it can be freed without any issue
    for (int i = 0; i < allocatingSize; i+=2) {
        free(ptrs[i]);
    }
}

void workload5(){
    int allocatingSize=16;
    void *ptrs[allocatingSize];

    // Initialize array
    for (int i = 0; i < allocatingSize; i++){
        int size=128;

        switch (i%4)
        {
        case 0:
            size=32;
            //printf("0? | ");
            break;
        case 1:
            size=128;
            //printf("1? | ");
            break;
        case 2:
            size=129;
            //printf("2? | ");
            break;
        case 3:
            size=256;
            //printf("3? | ");
            break;
        }
        ptrs[i] = malloc(size);
        //printf("ptr %i: %p\n",i,ptrs[i]);
    }

    free(ptrs[2]);
    ptrs[2]=NULL;
    free(ptrs[7]);
    ptrs[7]=NULL;
    free(ptrs[4]);
    ptrs[4]=NULL;
    free(ptrs[11]);
    ptrs[11]=NULL;
    free(ptrs[1]);
    ptrs[1]=NULL;
    ptrs[2]=malloc(200);
    ptrs[7]=malloc(64);
    ptrs[4]=malloc(150);

    free(ptrs[7]);
    ptrs[7]=NULL;
    free(ptrs[3]);
    ptrs[3]=NULL;
    ptrs[7]=malloc(300);

    free(ptrs[6]);
    ptrs[6]=NULL;
    free(ptrs[4]);
    ptrs[4]=NULL;

    ptrs[4]=malloc(350);

    for (int i = 0; i < allocatingSize; i++) 
        if(ptrs[i]!=NULL)
            free(ptrs[i]);
}


int main(int agrc, char** argv){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for(int i = 0; i < 50; i++){
        

        // Record the start time
        

        workload1();
        workload2();
        workload3();
        workload4();
        workload5();

        // Record the end time
        

        // Calculate total seconds and microseconds
        
        
        //printf("Run %d: %ld sec %ld microsec \n", i, seconds, microseconds);
    }
    gettimeofday(&end, NULL);
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    long avgMicro =((seconds*1000000)+microseconds)/50.0;
    long avgSecs = avgMicro / 1000000;
    avgMicro = avgMicro % 1000000;
    printf("Average time over 50 runs: %ld seconds %ld micro seconds\n",avgSecs,avgMicro);
}