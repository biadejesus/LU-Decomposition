/* This tests exactly 1 erroneous case from bug1997, to ensure they EACH get a warning */
#include "upc.h"

static int matmult10(shared [10] double *x) { return 0; } 

int main( int argc, char **argv ) {
  shared [] double *xI = 0;
  matmult10(xI);
  return 0;
}