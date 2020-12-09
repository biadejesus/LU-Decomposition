// test bupc_ptradd
#include <upc.h>
#include <stdio.h>

#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif

int main() {
  shared void *p = upc_all_alloc(10*THREADS, 100*sizeof(double));  

  #define _TEST_SZ(blocksz) do {                                                  \
    int bsz = (blocksz);                                                          \
    int count = bsz ? (1000/bsz)*bsz*THREADS : 1000;                              \
    shared [blocksz] double *arr = p;                                             \
    for (int i = 0; i < count; i++) {                                             \
      shared [blocksz] double *arrp = &(arr[i]);                                  \
      shared [blocksz] double *fnp = bupc_ptradd(p, bsz, sizeof(double), i);      \
      shared [blocksz] double *basep = bupc_ptradd(fnp, bsz, sizeof(double), -i); \
      if (arrp != fnp || upc_phaseof(arrp) != upc_phaseof(fnp))                   \
        printf("ERROR (arrp != fnp) at blocksz=%i and i=%i\n",bsz,i);             \
      if (basep != arr || upc_phaseof(basep) != upc_phaseof(arr))                 \
        printf("ERROR (basep != arr) at blocksz=%i and i=%i\n",bsz,i);            \
    }                                                                             \
  } while (0)
  #define TEST_SZ(b) _TEST_SZ(MIN(b,UPC_MAX_BLOCK_SIZE))

  TEST_SZ(0);
  TEST_SZ(1);
  TEST_SZ(2);
  TEST_SZ(10);
  TEST_SZ(1000);
  #ifdef __UPC_STATIC_THREADS__
    TEST_SZ(THREADS);
    TEST_SZ(10*THREADS);
  #endif
  
  upc_barrier;
  printf("done.\n");
  return 0;
}

