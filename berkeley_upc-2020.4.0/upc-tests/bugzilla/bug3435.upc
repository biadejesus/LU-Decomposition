#include <upc.h>
#include <stdio.h>

int main() {
  upc_barrier;
  if (MYTHREAD == 0) {
      printf("testing: thread 0 calling upc_global_exit(0)\n");
      upc_global_exit(0);
  } else return 0;
}

