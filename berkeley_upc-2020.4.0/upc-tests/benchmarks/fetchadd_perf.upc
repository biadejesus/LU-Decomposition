#include <upc_relaxed.h>
#include <upc_tick.h>
#include <upc_atomic.h>
#include <inttypes.h>
#include <stdio.h>

shared uint64_t X = 0;


void test(int threads, int iters, upc_lock_t *L) {
  uint64_t const one = 1;

  // ---------------------------------------------------------------------------
  upc_barrier;
  upc_atomichint_t hint1 = 
    #ifdef UPC_ATOMIC_HINT_FAVOR_NEAR
           UPC_ATOMIC_HINT_FAVOR_NEAR;
    #else
           0;
    #endif
  upc_atomicdomain_t *ad1 = upc_all_atomicdomain_alloc(UPC_UINT64, UPC_ADD, hint1);

  for (int i = 0; i < 10; ++i) {
    uint64_t result;
    upc_atomic_relaxed(ad1, &result, UPC_ADD, &X, &one, 0); /* warmup */
  }
  upc_barrier;
  upc_tick_t T1 = upc_ticks_now();
  if (MYTHREAD < threads) {
    for (int i = 0; i < iters; ++i) {
      uint64_t result;
      upc_atomic_relaxed(ad1, &result, UPC_ADD, &X, &one, 0);
    }
  }
  upc_barrier;
  T1 = upc_ticks_now() - T1;
  upc_all_atomicdomain_free(ad1);


  // ---------------------------------------------------------------------------
  upc_barrier;
  upc_atomichint_t hint2 = 
    #ifdef UPC_ATOMIC_HINT_FAVOR_FAR
           UPC_ATOMIC_HINT_FAVOR_FAR;
    #else
           0;
    #endif
  upc_atomicdomain_t *ad2 = upc_all_atomicdomain_alloc(UPC_UINT64, UPC_ADD, hint2);

  for (int i = 0; i < 10; ++i) {
    uint64_t result;
    upc_atomic_relaxed(ad2, &result, UPC_ADD, &X, &one, 0); /* warmup */
  }
  upc_barrier;
  upc_tick_t T2 = upc_ticks_now();
  if (MYTHREAD < threads) {
    for (int i = 0; i < iters; ++i) {
      uint64_t result;
      upc_atomic_relaxed(ad2, &result, UPC_ADD, &X, &one, 0);
    }
  }
  upc_barrier;
  T2 = upc_ticks_now() - T2;
  upc_all_atomicdomain_free(ad2);


  // ---------------------------------------------------------------------------
  upc_barrier;
  for (int i = 0; i < 10; ++i) { /* warmup */
    upc_lock(L); 
      uint64_t result = X;
      X = result + 1; 
    upc_unlock(L); 
  }
  upc_barrier;
  upc_tick_t T3 = upc_ticks_now();
  if (MYTHREAD < threads) {
    for (int i = 0; i < iters; ++i) {
      upc_lock(L); 
        uint64_t result = X;
        X = result + 1; 
      upc_unlock(L); 
    }
  }
  upc_barrier;
  T3 = upc_ticks_now() - T3;

  if (!MYTHREAD) printf("%i \t%g \t%g \t%g\n", threads, 
        (1e3*iters)/upc_ticks_to_ns(T1), 
        (1e3*iters)/upc_ticks_to_ns(T2), 
        (1e3*iters)/upc_ticks_to_ns(T3));
}

int main(int argc, char **argv) {
  upc_lock_t *L;
  int iters = 100;
  int i;

  if (argc > 1) {
    iters = atoi(argv[1]);
  }
  L = upc_all_lock_alloc();

  if (!MYTHREAD) printf("Update rate in Mop/s/thread\nTHREADS\tAMO/near\tAMO/far \tLOCKs\n");
  for (i = 1; i <= THREADS; ++i) test(i, iters, L);

  if (!MYTHREAD) printf("done.\n");
  upc_barrier;

  return 0;
}
