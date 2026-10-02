#include <stdlib.h>
#include "malloc.h"

#ifndef debug
    #define debug 0
#endif

#define DEBUGPRINT(...) if(debug) printf(__VA_ARGS__);

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
    int headersize= sizeof(struct header);
    if(size<headersize+8){
        //print TO SMALL
    }
    if(size%8!=0){
        allocSize=(size + 7) & ~7;
    }
    
    DEBUGPRINT("%u\n",allocSize);
    struct header *intial = (struct header *) heap.bytes;//points to start of heap
    if(initialized==0){
        intial->status=0;
        intial->length=MEMLENGTH-headersize;
        initialized=1;
    }
    
    struct header *EndofList= (struct header *)((char *)intial + MEMLENGTH);
    int interations=1;
    for(struct header *p = (struct header *) heap.bytes;p<EndofList;p= (struct header *) ((char * )p+ p->length+headersize)){
        DEBUGPRINT("%u\n",interations);
        interations++;
        
        if(p->status==0){
            if(p->length>=allocSize){
                p->status=1;
                int oldLength=p->length;
                p->length=allocSize;

                struct header *next=(struct header *) ((char * )p+ p->length+headersize);
                if(next>EndofList){
                    DEBUGPRINT("next is end");
                }else if (oldLength-allocSize<=headersize+8){
                    DEBUGPRINT("can't split");
                }else{
                    next->status=0;
                    next->length=oldLength-allocSize-headersize;
                }
                return (void *)((char *)p + headersize);;
            }
        }
    }
    DEBUGPRINT("it was full\n")
}


void myfree (void *ptr, char *file, int line){

}


int main (int argc, char **argv){
    mymalloc(16,__FILE__,__LINE__);
    mymalloc(300,__FILE__,__LINE__);
    int fill = 4096-(16+300)-30;
    mymalloc(fill,__FILE__,__LINE__);//fills the rest of the heap
}