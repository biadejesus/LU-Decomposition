#include <upc.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
  void *p1 = malloc(4);
  (free)(p1);
  upc_barrier;
  if (!MYTHREAD) printf("done.\n");
  return 0;
}
