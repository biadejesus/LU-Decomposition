#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "upc.h"

typedef shared char *Buffer;

shared[1] Buffer *buffers;

#ifndef LARGE
#define LARGE 50000
#endif

void allocImbalanced()
{

    for(int i = 0; i < LARGE; i++) {
        buffers[MYTHREAD * LARGE + i] = upc_alloc(log(rand()/2+1)*32 + 32);
    }
    upc_barrier;
    if (!MYTHREAD) printf("All threads allocated\n");
    upc_barrier;
}

int main(int argc, char **argv) {

    buffers = upc_all_alloc(LARGE*THREADS, sizeof(Buffer));

    allocImbalanced();

    for(int i = MYTHREAD; i < LARGE*THREADS; i+=THREADS) {
        upc_free(buffers[i]);
    }
    printf("Thread %d finished\n", MYTHREAD); fflush(stdout);
    upc_barrier;
    if (!MYTHREAD) printf("All threads finished\n");
    upc_barrier;

    upc_all_free(buffers);
    return 0;
}
