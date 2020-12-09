// UPC locktest by Dan Bonachea
//
// A simple tester to detect upc_lock implementations that don't correctly 
// enforce mutual exclusion for short critical sections.
//
// In each round, the test performs a series of conflicting read-modify-writes (increments) 
// from each thread to a centralized shared data area in a critical section protected by a upc_lock, 
// after which the result is verified. The test is repeated while varying the number of locations 
// accessed within each critical section, as well as the affinity of the data area and the
// affinity of the upc_lock controlling the critical section.
//
// Any errors generally indicate the upc_lock implementation is not correctly enforcing
// mutual exclusion with respect to small gets and puts in a shared region.
//
// Several defines below may be specified on the command line to alter behavior.
//
// Note the test is mostly intended to be run with small thread counts --
// the total amount of work per thread grows quadratically with thread count, 
// so you may need to scale down iterations at larger scales for reasonable run time.

// All data accesses are properly synchronized, so everything should work in both
// strict *and* relaxed mode. 
// We test relaxed because UPC compilers are more likely to mess that up.
#include <upc_relaxed.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif

#ifndef MAXLOC // max blocksize (in elements) for access chunks
#define MAXLOC MIN(4096,UPC_MAX_BLOCK_SIZE)
#endif

#ifndef ITERS // default number of iterations per round
#define ITERS 100
#endif

#ifndef T // should be an unsigned integral type
#define T uint64_t
#endif

shared [MAXLOC] T data[THREADS*MAXLOC];

upc_lock_t * shared slock[THREADS]; // one lock with affinity to each thread

int main(int argc, char **argv) {
  int iters = 0;
  size_t maxloc = 0;

  if (argc > 1 && argv[1][0] == '-') {
    if (!MYTHREAD) { 
      printf("Usage: %s (iters) (chunksz)\n", argv[0]);
      printf("  chunksz is specified as number of %ib elements: [1..%i]\n", 
             (int)sizeof(T), (int)MAXLOC);
    }
    exit(1);
  }

  if (argc > 1) iters = atoi(argv[1]);
  if (!iters) iters = ITERS;

  if (argc > 2) maxloc = atoi(argv[2]);
  if (!maxloc) maxloc = 128;
  if (maxloc > MAXLOC) maxloc = MAXLOC;

  // setup data structures

  slock[MYTHREAD] = upc_global_lock_alloc();
  shared [] T *mydata = (shared [] T *)&data[MYTHREAD * MAXLOC];
  assert(upc_threadof(mydata) == MYTHREAD);

  upc_barrier;
  if (!MYTHREAD) printf("Running locktest on %i threads, %i iters, %ib accesses, max chunk %ib\n",
                        THREADS, iters, (int)sizeof(T), (int)(maxloc * sizeof(T)));

  // test loop
  for (size_t sz = 1; sz <= maxloc; sz *= 2) { // payload chunk size for this round
    upc_barrier;
    for (int dtarget = 0; dtarget < THREADS; dtarget++) { // thread hosting the data for this round
      upc_barrier;
      if (!MYTHREAD) printf("%8ib chunk, datathread=%i\n",(int)(sz * sizeof(T)), dtarget);
      upc_barrier;
      shared [] T *pdata = (shared [] T *)&data[dtarget * MAXLOC];
      assert(upc_threadof(pdata) == dtarget);
      for (int ltarget = 0; ltarget < THREADS; ltarget++) {  // thread hosting the lock for this round
        upc_lock_t *lock = slock[ltarget];

        upc_barrier;
        for (size_t j=0; j < MAXLOC; j++) mydata[j] = 0; // init data area
        upc_barrier;

        for (int i=0; i < iters; i++) { // the contention loop, increment each value in the chunk iters times
          size_t init = 0;
          size_t limit = sz-1;
          int inc = +1;
          if (i&0x1) { // toggle chunk scanning direction each iteration
            init = sz-1;
            limit = 0;
            inc = -1;
          }
          upc_lock(lock);
            T first;
            for (size_t j = init; 1; j += inc) {
              T tmp = pdata[j]; // shared read
              if (j == init) first = tmp;
              else if (tmp != first) { // detect when values within a chunk differ
                printf("%i: ERROR: Inconsistent value within a chunk: chunk[%i]=0x%016llx chunk[%i]=0x%016llx datathread=%i lockthread=%i\n",
                       (int)MYTHREAD, 
                       (int)init, (unsigned long long)first, 
                       (int)j, (unsigned long long)tmp,
                       dtarget, ltarget);
              }
              tmp++; // value increment
              pdata[j] = tmp; // shared write

              if (j == limit) break;
            }
          upc_unlock(lock);
        }
        upc_barrier; // complete contention loop

        // final value check
        if (MYTHREAD == dtarget) { 
          T expect = (T)(THREADS * iters); // may truncate, with well-defined semantics
          for (size_t j=0; j < sz; j++) {
            T tmp = mydata[j];
            if (tmp != expect) {
              printf("%i: ERROR: Wrong value at final check: expect=0x%016llx actual[%i]=0x%016llx datathread=%i lockthread=%i\n",
                       (int)MYTHREAD, (unsigned long long)expect, (int)j, (unsigned long long)tmp, dtarget, ltarget);
            }
          }
        }
      }
    }
  }
  
  upc_barrier;

  // (optional) cleanup
  upc_lock_free(slock[MYTHREAD]);
   
  upc_barrier;
  if (!MYTHREAD) printf("done.\n");

  return 0;
}
