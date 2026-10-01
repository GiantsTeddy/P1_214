#include <stdlib.h>
#include "malloc.h"

#ifndef debug
    #define debug 0
#endif

#define MEMLENGTH 4096
static union {
    char bytes[MEMLENGTH];
    double not_used;
} heap;

struct header {
    int status;
    int length;
};

static int initialized=0;

void * mymalloc (size_t size, char *file, int line){
    int allocSize=size;
    if(size<sizeof(struct header)+8){
        //print TO SMALL
    }
    if(size%8!=0){
        allocSize=(size + 7) & ~7;
    }
    struct header *intial = (struct header *) heap.bytes;//points to start of heap
    if(initialized==0){
        intial->status=0;
        intial->length=MEMLENGTH-sizeof(struct header);
        initialized=1;
    }
    struct header *p = (struct header *) heap.bytes;
    for(struct header *p = (struct header *) heap.bytes;p<intial+MEMLENGTH;p+=p->length+sizeof(struct header)){
        if(p->status==0){
            if(p->length>=allocSize){
                p->status=1;
                int oldLength=p->length;
                p->length=allocSize;
                p+=p->length+sizeof(struct header);
                //Might have problems when we have a filled heap;
                p->status=0;
                p->length=oldLength-allocSize;
            }
        }
    }
}
void myfree (void *ptr, char *file, int line){

}


int main (int argc, char **argv){
    size_t size =16;
    mymalloc(size,*__FILE__,__LINE__);
}