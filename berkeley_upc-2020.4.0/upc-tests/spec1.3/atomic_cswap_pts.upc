#include <upc.h>
#include <upc_atomic.h>
#include <stdio.h>

#ifndef MIN
#define MIN(x,y)  ((x)<(y)?(x):(y))
#endif

#define B MIN(10,UPC_MAX_BLOCK_SIZE)

#define ASSERT(expr) do { if (!(expr)) { printf("ERROR at %i: %s\n", __LINE__, #expr); errors++; } } while(0)

int errors = 0;

// create a B-blocked pointer with a given thread and phase
// for any given thread arg, the location is identical
shared [B] char *makeptr(int thread, int phase) {
  static shared [B] char a[B*2*THREADS];
  ASSERT(phase >= 0 && phase < B);
  ASSERT(thread < THREADS);
  shared [B] char *myelem = &a[THREADS*B + thread*B]; 
  shared [] char *p = (shared [] char *)myelem;
  p -= phase;
  shared [B] char *p2 = (shared [B] char *)p;
  p2 += phase;
  ASSERT(p2 == (shared void *)myelem);
  ASSERT(upc_threadof(p2) == thread);
  ASSERT(upc_phaseof(p2) == phase);
  return p2;
}

int main() {
  upc_atomicdomain_t *d = upc_all_atomicdomain_alloc(UPC_PTS, UPC_CSWAP|UPC_GET|UPC_SET, 0);

  upc_barrier;

  if (!MYTHREAD) { // this is a serial test
    static shared void * shared target;
    shared void *oldval = NULL;
    shared void *newval;
    shared void *fetch;

    for (int th=0; th < THREADS; th++) {
      for (int ph=0; ph < B; ph++) {
        newval = makeptr(th, ph); // create next pointer in sequence

        if (ph == 0) { // this pointer is to a new location
          shared void *tmp = newval;
          ASSERT(tmp != oldval);

          // this CAS must "fail"
          upc_atomic_relaxed(d, &fetch, UPC_CSWAP, &target, &tmp, &newval);

          ASSERT(fetch == oldval); // verify original value
          ASSERT(upc_phaseof(fetch) == upc_phaseof(oldval));

          upc_atomic_relaxed(d, &fetch, UPC_GET, &target, NULL, NULL);

          ASSERT(fetch == oldval); // verify non-replaced value
          ASSERT(upc_phaseof(fetch) == upc_phaseof(oldval));
        } else {
          ASSERT(newval == oldval);
          ASSERT(upc_phaseof(newval) != upc_phaseof(oldval));
        }
      
        // this CAS must "succeed"
        upc_atomic_relaxed(d, &fetch, UPC_CSWAP, &target, &oldval, &newval);

        ASSERT(fetch == oldval); // verify original value
        ASSERT(upc_phaseof(fetch) == upc_phaseof(oldval));

        upc_atomic_relaxed(d, &fetch, UPC_GET, &target, NULL, NULL);

        ASSERT(fetch == newval); // verify replaced value
        ASSERT(upc_phaseof(fetch) == upc_phaseof(newval));

        oldval = newval;
      }
    }

  }

  upc_barrier;

  if (!MYTHREAD) {
    if (!errors) printf("SUCCESS\n");
  }

  return 0;
}
