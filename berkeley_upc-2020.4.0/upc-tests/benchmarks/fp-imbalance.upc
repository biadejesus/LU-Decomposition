#include <upc.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>
#include <float.h>
#include <upc_tick.h>
#include <bupc_collectivev.h>

void usage(const char *p) {
  if (!MYTHREAD) {
    fprintf(stderr,"Usage: %s <Miters> <worker_threads>\n",p);
  }
  upc_barrier;
  exit(1);
}

int main(int argc, char **argv) {
  uint64_t Miters = 100;
  int worker_threads = MAX(1,THREADS/2);

  if (argc > 1) {
    long long v = atoll(argv[1]);
    if (v <= 0) usage(argv[0]);
    Miters = (uint64_t)v;
  }
  uint64_t iters = Miters*1000000;
  if (argc > 2) {
    worker_threads = atoi(argv[2]);
  }
  if (argc > 3 || !iters || worker_threads < 0 || worker_threads > THREADS) usage(argv[0]);

  int idle_threads = THREADS - worker_threads;
  int iamworker = 0;
  if (worker_threads > 0) {
    int R = (THREADS+worker_threads-1) / worker_threads;
    iamworker = !(MYTHREAD % R);
  }
  int check = bupc_allv_reduce_all(int, iamworker, UPC_ADD); 
  assert(check == worker_threads);
  if (!MYTHREAD) {
    printf("Running %s with %"PRIu64" Miters, THREADS=%i workers=%i idlers=%i ...\n",
           argv[0], Miters, THREADS, worker_threads, idle_threads);
  }
  upc_barrier;
  static volatile double result = 0;
  double mysecs = 0;
  upc_tick_t start = upc_ticks_now();
  if (iamworker) {
    const double eps = DBL_EPSILON;
    const double factor = 1.0 + eps;
    double accum = factor;
    for (uint64_t i = 0; i < iters; i++) {
      accum = accum * factor + eps;
    }
    result = accum;
    mysecs = upc_ticks_to_ns(upc_ticks_now() - start)/1.0e9;
  }
  upc_barrier; // idle threads wait in this barrier
  double totalsecs = upc_ticks_to_ns(upc_ticks_now() - start)/1.0e9;

  // collect statistics
  double tmp; 
  result = (tmp=result, bupc_allv_reduce_all(double, tmp, UPC_ADD)); // defeat optimizer
  double secs_min = (tmp=(iamworker?mysecs:1.0e100), bupc_allv_reduce_all(double, tmp, UPC_MIN));
  double secs_max = bupc_allv_reduce_all(double, mysecs, UPC_MAX);
  double secs_sum = bupc_allv_reduce_all(double, mysecs, UPC_ADD);
  double secs_avg = secs_sum/worker_threads;
  double workerMflops = 2 * iters / 1.0e6;

  if (!MYTHREAD) {
    printf("Overall runtime: %0.3f sec\n",totalsecs);
    printf("Overall speed:   %0.3f MFlops\n",workerMflops*worker_threads/totalsecs);
    printf("\n");
    printf("                         MIN       MAX       AVG     TOTAL\n");
    printf("                    --------------------------------------\n");
    printf("worker time (secs): %8.3f  %8.3f  %8.3f  %8.3f\n",
          secs_min, secs_max, secs_avg, secs_sum);
    printf("worker MFlops:      %8.3f  %8.3f  %8.3f  %8.3f\n",
          workerMflops/secs_max, workerMflops/secs_min, 
          workerMflops/secs_avg, workerMflops*worker_threads/secs_avg);
  }

  return 0;
}
