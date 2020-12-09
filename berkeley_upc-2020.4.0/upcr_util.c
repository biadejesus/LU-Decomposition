#include <upcr_internal.h>

/* thread-safe rand wrappers - for use by application code ONLY */
extern int _upcri_rand(UPCRI_PT_ARG_ALONE) {
  uint64_t lresult;
  int result;
  lresult = nrand48((void*)upcri_auxdata()->rand_state);
  lresult = lresult % (((uint64_t)RAND_MAX)+1);
  result = (int)lresult;
  upcri_assert(result >= 0 && result <= RAND_MAX);
  return result;
}

extern void _upcri_srand(unsigned int _seed UPCRI_PT_ARG) {
  memset(upcri_auxdata()->rand_state, 0, 3*sizeof(short));
  memcpy(upcri_auxdata()->rand_state, &_seed, MIN(sizeof(_seed), 3*sizeof(short)));
}

extern void _upcri_rand_init(UPCRI_PT_ARG_ALONE) {
  /* At least Linux/glibc has been observed to race when setting
     the 'a' and 'c' parameters on the first call to nrand48().
     So, this nrand48() call is a throw-away to ensure the
     parameters are initialized.
     We do this on all platforms since it will not change results.
   */
  if (upcri_mypthread() == 0) {
    memset(upcri_auxdata()->rand_state, 0xaa, 3*sizeof(short));
    (void)nrand48((void*)upcri_auxdata()->rand_state);
  }

  _upcri_srand(1 UPCRI_PT_PASS); /* C99 mandates default srand(1) */

#if PLATFORM_OS_CYGWIN
  /* Bug 1475: Cywgin needs a srand48() call or nrand48() will use a
   * multiplier of zero!!  It also wants it called from *every* thread.
   */
  srand48(1);
#endif
}

/* replacement for clock() on systems where it's lacking */
static gasnett_tick_t tickbegin = GASNETT_TICK_MIN;
extern void upcri_clock_init(void) {
  tickbegin = gasnett_ticks_now();
}
extern clock_t upcri_clock(void) {
  upcri_assert(CLOCKS_PER_SEC == 1000000);
  return (clock_t)gasnett_ticks_to_us(gasnett_ticks_now()-tickbegin);
}

/* replacement for system() on systems where RMA makes it unsafe (bug 3909) */
int bupc_system(const char *cmd) {
  #if HAVE_MMAP
    // gasnet_config.h has result of probe for HAVE_MAP_ANON(YMOUS)
    #if defined(MAP_ANONYMOUS) || defined(HAVE_MAP_ANONYMOUS)
      #define UPCRI_MAP_ANONYMOUS MAP_ANONYMOUS
    #elif defined(MAP_ANON) || defined(HAVE_MAP_ANON)
      #define UPCRI_MAP_ANONYMOUS MAP_ANON
    #else
      #define UPCRI_MAP_ANONYMOUS 0 /* probably fails but just want to compile */
    #endif

    // Copy cmd to freshly allocated page(s)
    size_t len = strlen(cmd) + 1;
    char *ptr = mmap(NULL, len, PROT_READ|PROT_WRITE,
                                MAP_SHARED|UPCRI_MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) return -1;
    strcpy(ptr, cmd);

    int result = system(ptr);

    int save_errno = errno;
    munmap(ptr,len);
    errno = save_errno;

    return result;
  #else
    return system(cmd);
  #endif
}
