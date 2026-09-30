#include mymalloc.h;

#define MEMLENGTH 4096
static union {
char bytes[MEMLENGTH];
double not_used;
} heap;

struct header (){
    int status;
    int memlength;
}

void * mymalloc (size_t size, char *file, int line){
    struct header *p = (struct header *) heap->bytes;
}

void   myfree (void *ptr, char *file, int line){

}