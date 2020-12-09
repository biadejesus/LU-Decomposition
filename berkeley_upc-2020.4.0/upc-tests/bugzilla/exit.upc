#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

int main(int argc, char **argv) {
  printf("Hello from %i/%i\n",MYTHREAD,THREADS);
 
  upc_barrier;
  int test = 0;
  if (argc > 1) test = atoi(argv[1]);
  const char *testdesc[] = {
    "Return from main",
    "exit(0)",
    "upc_global_exit(0)",
    "abort()",
  };
  int numtests = sizeof(testdesc)/sizeof(testdesc[0]);
  if (!MYTHREAD) {
    if (test >= numtests) {
      fprintf(stderr,"Usage: %s [0..%i]\n",argv[0],numtests);
      upc_global_exit(1);
    }
    printf("Running test %i: %s\n",test,testdesc[test]); fflush(0);
  }
  upc_barrier;
  switch (test) {
    case 0:
      return 0; 
    break;
    case 1:
      exit(0); 
    break;
    case 2:
      upc_global_exit(0); 
    break;
    case 3:
      abort(); 
    break;
    default: abort();
  }
  upc_barrier;
  fprintf(stderr,"TH%i did not die!\n",MYTHREAD);
  abort();
  return 0;
}
