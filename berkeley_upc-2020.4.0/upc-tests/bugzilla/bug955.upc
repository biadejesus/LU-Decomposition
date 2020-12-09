#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#include <upc.h> 

typedef struct{
  int64_t dest;
  int64_t payload[3];
} packet_t;


int main(void){

  shared packet_t *sh_buf;
  packet_t *pr_buf;
  sh_buf = upc_all_alloc(9000*THREADS,sizeof(packet_t));
  pr_buf = (packet_t *)(&sh_buf[MYTHREAD]);

  // generate lots of packets with random dest and correct src
  int64_t i;
  int64_t n_packet;

  if((100%THREADS) != 0){
    if(MYTHREAD==0) printf("we require threads to divide 100\n");
    return 1;
  }
  
  int64_t idx=0;
  for(n_packet=7000; n_packet<9000; n_packet+=100){
    idx++;

    if(MYTHREAD==0) {
      printf("starting n_packet = %"PRId64"\n",n_packet);
      fflush(stdout);
    }

    // the bug only happens when we calloc each time
    packet_t *packet = calloc(n_packet,sizeof(packet_t));

    // populate packets with destination and current idx
    int64_t width = n_packet/THREADS;
    for(i=0; i<n_packet; i++){
      packet[i].dest = i/width;
      packet[i].payload[0] = idx;
    }
    upc_barrier;


    // send them
    int thr;
    for(thr = 0; thr<THREADS; thr++){
      upc_memput((shared void *)&sh_buf[width*MYTHREAD*THREADS + thr],(char *)&packet[width*thr],width*sizeof(packet_t));
    }
    upc_barrier;

    // check the received packets
    for(i=0; i<n_packet; i++){
      if(pr_buf[i].dest != MYTHREAD || pr_buf[i].payload[0] != idx){
	printf("thread %d, dest %"PRId64" : ERROR OCCURRED at position %"PRId64" (idx %"PRId64", payload %"PRId64")\n",
	       (int)MYTHREAD,pr_buf[i].dest,i,idx,pr_buf[i].payload[0]);
	break;
      }
    }
    upc_barrier;

    // this free matters in making the bug
    free(packet);

    fflush(stdout);
  }

  upc_all_free(sh_buf);

  if (!MYTHREAD) printf("done.\n");
  return 0;
}

