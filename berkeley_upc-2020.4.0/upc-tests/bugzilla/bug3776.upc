#include <upc.h>
#include <stdio.h>
#include <stdint.h>

#ifndef SZ
#define SZ 64*1024*1024
#endif

// 64 MB on thread 0, zero on other threads
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
extern shared [] char bigarray[SZ];
       shared [] char bigarray[SZ];

#ifndef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
#endif

#ifndef BSZ
#define BSZ MIN(1024*1024, UPC_MAX_BLOCK_SIZE)
#endif

// up to 8MB on each thread
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
extern shared [BSZ] double distarray[BSZ*THREADS];
       shared [BSZ] double distarray[BSZ*THREADS];

int main() {
  if (!MYTHREAD) printf("done.\n");
  return 0;
}
