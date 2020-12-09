/*
 upc version of guppie.c
 001222 mhm

 to compile with bill and jessie's t3e compiler:
 upc -O2 -fshared -fthreads-16 -X16 -o guppie.upc guppie.upc.c
 upc -O2 -fstrength-reduce -funroll-loops -fshared -fthreads-16 -X16 -o guppie.upc guppie.upc.c
 upc -O2 -fstrength-reduce -funroll-all--loops -fshared -fthreads-16 -X16 -o guppie.upc guppie.upc.c

 upc -O2 -fstrength-reduce -funroll-loops -fshared -fthreads-128 -X128 -o guppie.upc guppie.upc.c
 setlabel -l Hbigdev guppie.upc
 pagesize -p 8M guppie.upc
 ./guppie.upc

*/

#include <upc_relaxed.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <time.h>
#include <unistd.h>
#include <inttypes.h>

/* Types used by program (should be 64 bits) */
#define uint64 uint64_t
#define int64 int64_t

/* Macros for timing */
#if __UPC_TICK__
#include <upc_tick.h>
#define WSEC() (upc_ticks_to_ns(upc_ticks_now()) / 1.0E9)
#elif defined __BERKELEY_UPC_RUNTIME__
#define WSEC() (bupc_ticks_to_ns(bupc_ticks_now()) / 1.0E9)
#else
static struct tms t;
#define WSEC() (times(&t) / (double)sysconf(_SC_CLK_TCK))
#endif
#define CPUSEC() (clock() / (double)CLOCKS_PER_SEC)

/* Random number generator */
#define POLY 0x0000000000000007UL
#define PERIOD 1317624576693539401L

/* Log size of main table (suggested: half of global memory) */
#ifndef LTABSIZE
#define LTABSIZE 25L
#endif
#define TABSIZE (1L << LTABSIZE)

/* Number of updates to table (suggested: 4x number of table entries) */
#define NUPDATE (4L * TABSIZE)

/* PGI TPR 19327: libpgc in 13.0 to 13.5 contains a conflicting "table" symbol! -PHH */
#if defined(__PGI) && (__PGIC__ == 13) && (__PGIC_MINOR__ < 6)
#define table guppie_table
#endif

/* Allocate main table in shared memory */
shared uint64 table[TABSIZE];

/*
  Utility routine to start random number generator at Nth step
*/
uint64 startr(int64 n)
{
    int i, j;
    uint64 m2[64];
    uint64 temp, ran;

    while (n < 0) n += PERIOD;
    while (n > PERIOD) n -= PERIOD;
    if (n == 0) return 0x1;

    temp = 0x1;
    for (i=0; i<64; i++)
    {
	m2[i] = temp;
	temp = (temp << 1) ^ ((int64) temp < 0 ? POLY : 0);
	temp = (temp << 1) ^ ((int64) temp < 0 ? POLY : 0);
    }
    
    for (i=62; i>=0; i--)
	if ((n >> i) & 1)
	    break;

    ran = 0x2;
    while (i > 0)
    {
	temp = 0;
	for (j=0; j<64; j++)
	    if ((ran >> j) & 1)
		temp ^= m2[j];
	ran = temp;
	i -= 1;
	if ((n >> i) & 1)
	    ran = (ran << 1) ^ ((int64) ran < 0 ? POLY : 0);
    }
  
    return ran;
}    

/********************************
 divide up total size (loop iters or space amount) in a blocked way
*********************************/
void Block(int mype,
           int npes,
           int64 totalsize,
           int64 *start,
           int64 *stop,
           int64 *size)
{
    int64 div;
    int64 rem;

    div = totalsize / npes;
    rem = totalsize % npes;

    if (mype < rem)
    {
        *start = mype * (div + 1);
        *stop   = *start + div;
        *size  = div + 1;
    }
    else
    {
        *start = mype * div + rem;
        *stop  = *start + div - 1;
        *size  = div;
    }
}


/*
  update_table -- multi-threaded update_table for 2 or more threads
*/
void update_table(int64 ltabsize,
		  int64 tabsize,
		  int64 nupdate)
{
#ifndef VLEN
#define VLEN 32
#endif
    uint64 ran[VLEN];              /* Current random numbers */
    uint64 t1[VLEN];
    uint64 temp;
    uint64 nerrors;
    double icputime;               /* CPU time to init table */
    double is;
    double cputime;               /* CPU time to update table */
    double s;
    int64 start, stop, size;
    uint64 *local_table;
    int64 i;
    int64 j;

    upc_barrier;

    /* Begin init timing here */
    icputime = -CPUSEC();
    is = -WSEC();

    /* Initialize main table */
    for(i = MYTHREAD; i < tabsize; i+= THREADS)
	table[i] = i;

    upc_barrier;

    /* End init timing here */
    icputime += CPUSEC();
    is += WSEC();

    upc_barrier;

    /* Begin update timing here */
    cputime = -CPUSEC();
    s = -WSEC();
    
    Block(MYTHREAD, THREADS, nupdate, &start, &stop, &size);

    uint64 rng = startr(start);

    for (i=0; i<size/VLEN; i++)
    {
	for (j=0; j<VLEN; j++)
	{
            rng = (rng << 1) ^ ((int64) rng < 0 ? POLY : 0);
            t1[j] = table[rng & (tabsize-1)];
            ran[j] = rng;
	}
	for (j=0; j<VLEN; j++)
	{
	    t1[j] ^= ran[j];
	}
	for (j=0; j<VLEN; j++)
	{
	    table[ran[j] & (tabsize-1)] = t1[j];
	}
    }

    upc_barrier;

    /* End update timed section */
    cputime += CPUSEC();
    s += WSEC();

    /* Verification of results (in parallel).  */
    temp = 0x1;
    for (i=0; i<nupdate; i++) {
      temp = (temp << 1) ^ (((int64) temp < 0) ? POLY : 0);
      shared uint64 *p = & table[temp & (tabsize-1)];
      if (upc_threadof(p) == MYTHREAD)
        *p ^= temp;
    }
	
    nerrors = 0;
    for (i = MYTHREAD; i < tabsize; i += THREADS)
      if (table[i] != i)
        nerrors++;
 
    upc_barrier;

    /* Save error count in table.
       (We can re-use table here.)  */
    table[MYTHREAD] = nerrors;

    upc_barrier;
	
    if (MYTHREAD == 0) {
      /* Print timing results */
      printf ("init(c= %.4f w= %.4f) up(c= %.4f w= %.4f) rate= %.4f Mup/s\n",
	      icputime, is, cputime, s, ((double) nupdate * 1e-6 / s));
      for (i = 1; i < THREADS; ++i)
        nerrors += table[i];
	printf ("Found %d errors in %d locations (%.1f%%, %s).\n",
	      (int) nerrors, (int) tabsize, 100.0*nerrors/tabsize,
	      (nerrors <= tabsize / 10) ? "passed" : "failed");
    }

}

/*
  main routine
*/
int main(int argc, char *argv[])
{
    int64 ltabsize = LTABSIZE;
    int64 tabsize = TABSIZE;	
    int64 nupdate = NUPDATE;	

    if(argc >= 2)
    {
	ltabsize = (atoi(argv[1]));
	tabsize = 1L << ltabsize;
      nupdate = 4L * tabsize;
    }
    
    if(argc >= 3)
    {
	nupdate = 1L << (atoi(argv[2]));	
    }
    
    if(MYTHREAD == 0)
    {
	/* Print parameters for run */
	printf("nThreads = %d\n", THREADS);
	printf("Main table size = 2^%ld = %ld words\n", (long)ltabsize, (long)tabsize);
	printf("Number of updates = %ld\n", (long)nupdate);
    }
    
    update_table(ltabsize, tabsize, nupdate);
    return(0);
}




