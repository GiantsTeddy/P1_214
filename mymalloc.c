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

struct header { //size 8
    int status;
    int length;
};

static int initialized=0;

void printHeaders(){
    struct header* a = (struct header *) heap.bytes;
    while(1){
        printf("Header: \n\tSize: %d \n\tStatus: %d\n", a->length, a->status);

        if((void *)a + a->length+sizeof(struct header) >= (void*)(heap.bytes)+MEMLENGTH){ break; } //Quit ts before we get outside of array
        a = (struct header *) ((char * )a + a->length+sizeof(struct header)); //iterates the penis
    }
    printf("\n\n");
}

void detectLeaks(){
    int leakedObjs = 0;
    int leakedBytes = 0;
    struct header* a = (struct header *) heap.bytes;
    while(1){
        if(a->status == 1){
            leakedObjs+=1;
            leakedBytes+=a->length;
        }

        if((void *)a + a->length+sizeof(struct header) >= (void*)(heap.bytes)+MEMLENGTH){ break; } //Quit ts before we get outside of array
        a = (struct header *) ((char * )a + a->length+sizeof(struct header)); //iterates the penis
    }
    printf("%d bytes leaked in %d objects.\n", leakedBytes, leakedObjs);
}

void * mymalloc (size_t size, char *file, int line){
    int allocSize=size;
    int headersize= sizeof(struct header);
    if(size<headersize+8){
        //print TOO SMALL
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
        atexit(detectLeaks);
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
                    DEBUGPRINT("next is end\n");
                }else if (oldLength-allocSize<=headersize+8){
                    DEBUGPRINT("can't split\n");
                }else{
                    next->status=0;
                    next->length=oldLength-allocSize-headersize;
                }
                return (void *)((char *)p + headersize);
            }
        }
    }
    DEBUGPRINT("it was full\n")
    return NULL;
}

void coallese(void* ptr){

}

void myfree (void *ptr, char *file, int line){
    //printf("entered function\n");
    //Error check: free before shit is initialized
    if(initialized==0){
        fprintf(stderr, "Free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
    }
    struct header* curr = (struct header *) heap.bytes;

    //printf("heap.bytes      = %p\n", (void *)heap.bytes); //Mem addy for start of heap
    //printf("header pointer  = %p\n", (void *)curr + sizeof(struct header)); //Mem addy for header
    //printf("&heap           = %p\n", (void *)&heap); //Mem addy for start of heap
    //printf("curr block = %p\n", (void *)curr + sizeof(struct header));
    //printf("ptr = %p\n", ptr);
    
    //Check each header to see if the current chunk stores the ptr
    while((void *)curr + sizeof(struct header) != ptr){
        //printf("curr block = %p\n", (void *)curr + sizeof(struct header));
        //printf("ptr = %p\n", ptr);

        if((void *)curr + curr->length+sizeof(struct header) >= (void*)(heap.bytes)+MEMLENGTH){ break; } //Quit ts before we get outside of array
        curr = (struct header *) ((char * )curr + curr->length+sizeof(struct header)); //iterates the penis
    }
    if((void *)curr + sizeof(struct header) == ptr){
        //Error check
        if(curr->status == 0){
            fprintf(stderr, "Free: Inappropriate pointer (%s:%d)\n", file, line);
            exit(2);
        }
        //Deallocate bitches
        curr->status = 0;
        //printf("free success\n");
        //Coallese
        struct header* a = (struct header*) heap.bytes;
        while(1){
            if(a->status == 0){
                struct header* b = (void*)a + a->length + sizeof(struct header); //b is the header next to a
                
                while(1){
                    if(b->status == 0){
                        a->length += b->length + sizeof(struct header);
                    } else { break; }

                    b = (struct header *) ((char * )b + b->length+sizeof(struct header)); //iterates the penis
                }
            }
            if((void *)a + a->length+sizeof(struct header) >= (void*)(heap.bytes)+MEMLENGTH){ break; } //Quit ts before we get outside of array
            a = (struct header *) ((char * )a + a->length+sizeof(struct header)); //iterates the penis
        }
    } else {
        //Free: Inappropriate pointer (file.c:line)
        fprintf(stderr, "Free: Inappropriate pointer (%s:%d)\n", file, line);
        exit(2);
        //printf("fucked it up\n");
    }
    
}


int main (int argc, char **argv){
    void* fuck = mymalloc(16,__FILE__,__LINE__);
    void* a = mymalloc(300,__FILE__,__LINE__);
    int fill = 4096-(16+300)-30;
    void* b = mymalloc(fill,__FILE__,__LINE__);//fills the rest of the heap

    printHeaders();

    //free test
    myfree(fuck, __FILE__, __LINE__);
    myfree(a, __FILE__, __LINE__);
    myfree(b, __FILE__, __LINE__);

    printHeaders();

    void* x = mymalloc(26,__FILE__,__LINE__);
    void* y = mymalloc(44,__FILE__,__LINE__);
    void* z = mymalloc(5,__FILE__,__LINE__);

    //myfree(z, __FILE__, __LINE__);
    //myfree(y, __FILE__, __LINE__);



    int l = 90;
    int* m = &l;

    
    //int *p = mymalloc(sizeof(int)*2, __FILE__, __LINE__); //error check
    //myfree(p + 1, __FILE__, __LINE__);

    /*int *r = mymalloc(sizeof(int)*100, __FILE__, __LINE__); //error check
    int *q = r;
    myfree(r, __FILE__, __LINE__);
    myfree(q, __FILE__, __LINE__);*/

    printHeaders();
}