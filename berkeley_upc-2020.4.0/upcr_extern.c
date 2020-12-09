/*
 * UPC runtime functions for external C/C++/MPI code
 *
 * Jason Duell <jcduell@lbl.gov>
 * $Source: bitbucket.org:berkeleylab/upc-runtime.git/upcr_extern.c $
 *
 */

#include <upcr_internal.h>

static void 
upcri_too_early(const char *func)
{
    fflush(stdout);
    /* NOTE: the test harness scans for "UPC Runtime error:" */
    fprintf(stderr, 
	    "UPC Runtime error: %s called before UPC runtime initialized\n", 
	    func);
    fflush(stderr);

    exit(-1);
}

// Error if called too soon
// Should PRECEDE call to UPCR_BEGIN_FUNCTION(), to ensure desired error diagnostic
// behavior for an early init in DEBUG/-pthreads mode.
#if UPCR_DEBUG
  #define UPCRI_DIE_IF_TOO_EARLY(func)  \
   if (upcri_startup_lvl == upcri_startup_init)	\
       upcri_too_early(func);
#else
  #define UPCRI_DIE_IF_TOO_EARLY(func)  
#endif

int bupc_extern_mythread()
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_mythread");
    UPCR_BEGIN_FUNCTION();
    return (int) upcr_mythread();
}

int bupc_extern_threads()
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_threads");
    UPCR_BEGIN_FUNCTION();
    return (int) upcr_threads();
}

char * bupc_extern_getenv(const char *env_name)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_getenv");
    UPCR_BEGIN_FUNCTION();
    return upcr_getenv(env_name);
}

void bupc_extern_notify(int barrier_id)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_notify");
    UPCR_BEGIN_FUNCTION();
    upcr_notify(barrier_id, 0);
}

void bupc_extern_wait(int barrier_id)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_wait");
    UPCR_BEGIN_FUNCTION();
    upcr_wait(barrier_id, 0);
}

void bupc_extern_barrier(int barrier_id)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_barrier");
    UPCR_BEGIN_FUNCTION();
    upcr_notify(barrier_id, 0);
    upcr_wait(barrier_id, 0);
}

void * bupc_extern_alloc(size_t bytes)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_alloc");
    UPCR_BEGIN_FUNCTION();
    upcr_shared_ptr_t sptr = upcr_alloc(bytes);
    if (upcr_isnull_shared(sptr))
	return NULL;
    return upcr_shared_to_local(sptr);
}

void * bupc_extern_all_alloc(size_t nblocks, size_t blocksz)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_all_alloc");
    UPCR_BEGIN_FUNCTION();
    upcr_shared_ptr_t sptr = upcr_all_alloc(nblocks, blocksz);
    if (upcr_isnull_shared(sptr))
	return NULL;
    /* Performs equivalent of "(void *)sptr[MYTHREAD]", 
     * but w/o pointer arithmetic */
    return upcri_shared_remote_to_mylocal(sptr);
}


void bupc_extern_free(void *ptr, int thread)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_free");
    UPCR_BEGIN_FUNCTION();
    if (ptr == NULL)
	return;
    upcr_shared_ptr_t sptr;
    if (thread == -1) {
      sptr = _bupc_inverse_cast(ptr);
    } else {
      sptr = _bupc_local_to_shared(ptr, thread, 0);
    }
    upcri_assert(!upcr_isnull_shared(sptr));
    upcr_free(sptr);
}

void bupc_extern_all_free(void *ptr, int thread)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_all_free");
    UPCR_BEGIN_FUNCTION();
    if (ptr == NULL)
	return;
    upcr_all_free(_bupc_local_to_shared(ptr, thread, 0));
}

int bupc_extern_threadof(void *ptr)
{
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_threadof");
    UPCR_BEGIN_FUNCTION();

    if_pf (!ptr) return -1;

    upcr_shared_ptr_t sptr = _bupc_inverse_cast(ptr);
    if_pf (upcr_isnull_shared(sptr)) return -1;
    else return upcr_threadof_shared(sptr);
}

void bupc_extern_process_thread_layout(int *upcthreads_in_process,
                                       int *first_upcthread_in_process) 
{    
    UPCRI_DIE_IF_TOO_EARLY("bupc_extern_process_thread_layout");
    // Do NOT call UPCR_BEGIN_FUNCTION() - this is defined to work for any pthread
    if (upcthreads_in_process) {
      #if UPCRI_UPC_PTHREADS
        *upcthreads_in_process = upcri_mypthreads();
      #else
        *upcthreads_in_process = 1;
      #endif
    }
    if (first_upcthread_in_process) {
      #if UPCRI_UPC_PTHREADS
        upcr_thread_t mynode = upcr_mynode();
        *first_upcthread_in_process = upcri_1stthread(mynode);
      #else
        *first_upcthread_in_process = upcr_mythread();
      #endif
    }
}

void bupc_extern_config_info(const char **upcr_config_str, 
                             const char **gasnet_config_str,
                             const char **upcr_version_str,
                             int    *upcr_runtime_spec_major,
                             int    *upcr_runtime_spec_minor,
                             int    *upcr_debug,
                             int    *upcr_pthreads,
                             size_t *upcr_pagesize) {
  // This function MAY be called pre-init
  // do NOT invoke UPCRI_DIE_IF_TOO_EARLY or UPCR_BEGIN_FUNCTION
  // do NOT add any dependencies on non-const runtime state
  if (upcr_config_str)   *upcr_config_str = UPCR_CONFIG_STRING;
  if (gasnet_config_str) *gasnet_config_str = GASNET_CONFIG_STRING;
  if (upcr_version_str)  *upcr_version_str = UPCR_VERSION; 
  if (upcr_runtime_spec_major) *upcr_runtime_spec_major = UPCR_RUNTIME_SPEC_MAJOR;
  if (upcr_runtime_spec_minor) *upcr_runtime_spec_minor = UPCR_RUNTIME_SPEC_MINOR;
  #if UPCR_DEBUG
    if (upcr_debug) *upcr_debug = 1;
  #else
    if (upcr_debug) *upcr_debug = 0;
  #endif
  #if UPCRI_UPC_PTHREADS
    if (upcr_pthreads) *upcr_pthreads = 1;
  #else
    if (upcr_pthreads) *upcr_pthreads = 0;
  #endif
  if (upcr_pagesize) *upcr_pagesize = UPCR_PAGESIZE;
}

//-------------------------------------------------------------------------------
// support for bupc_tentative API
#ifdef __BUPC_TENTATIVE_H
#error header inclusion ordering problem
#endif
#define BUPC_TENTATIVE_SPECIFIER extern // we are providing the definitions in this TU
#include <bupc_tentative.h>

int bupc_tentative_version_major = BUPC_TENTATIVE_VERSION_MAJOR;
int bupc_tentative_version_minor = BUPC_TENTATIVE_VERSION_MINOR;

void (*bupc_tentative_init)(int *argc, char ***argv) = bupc_init;

void (*bupc_tentative_init_reentrant)(int *argc, char ***argv,
                         int (*pmain_func)(int, char **) ) = bupc_init_reentrant;

void (*bupc_tentative_exit)(int exitcode) = bupc_exit;

int (*bupc_tentative_mythread)(void) = bupc_extern_mythread;
int (*bupc_tentative_threads)(void) = bupc_extern_threads;

char * (*bupc_tentative_getenv)(const char *env_name) = bupc_extern_getenv;

void (*bupc_tentative_notify)(int barrier_id) = bupc_extern_notify;
void (*bupc_tentative_wait)(int barrier_id) = bupc_extern_wait;
void (*bupc_tentative_barrier)(int barrier_id) = bupc_extern_barrier;

void * (*bupc_tentative_alloc)(size_t bytes) = bupc_extern_alloc;
void * (*bupc_tentative_all_alloc)(size_t nblocks, size_t blocksz) = bupc_extern_all_alloc;
void (*bupc_tentative_free)(void *ptr, int thread) = bupc_extern_free;
void (*bupc_tentative_all_free)(void *ptr, int thread) = bupc_extern_all_free;

int (*bupc_tentative_threadof)(void *ptr) = bupc_extern_threadof;

void (*bupc_tentative_process_thread_layout)(int *upcthreads_in_process,
                                             int *first_upcthread_in_process) = 
                                             bupc_extern_process_thread_layout;

void (*bupc_tentative_config_info)(const char **upcr_config_str,
                                   const char **gasnet_config_str,
                                   const char **upcr_version_str,
                                   int    *upcr_runtime_spec_major,
                                   int    *upcr_runtime_spec_minor,
                                   int    *upcr_debug,
                                   int    *upcr_pthreads,
                                   size_t *upcr_pagesize) = bupc_extern_config_info;

extern void upcri_init_extern(void) {
  // this function is called by init to ensure upcr_extern.o is always present in the link,
  // which is a requirement for correct operation of the bupc_tentative_* API
  #define CHECK_TENT(sym) if (!(sym)) upcri_err("upcri_init_extern: missing " #sym)
  CHECK_TENT(bupc_tentative_version_major);
  CHECK_TENT(bupc_tentative_version_minor);
  CHECK_TENT(bupc_tentative_init);
  CHECK_TENT(bupc_tentative_init_reentrant);
  CHECK_TENT(bupc_tentative_exit);
  CHECK_TENT(bupc_tentative_mythread);
  CHECK_TENT(bupc_tentative_threads);
  CHECK_TENT(bupc_tentative_getenv);
  CHECK_TENT(bupc_tentative_notify);
  CHECK_TENT(bupc_tentative_wait);
  CHECK_TENT(bupc_tentative_barrier);
  CHECK_TENT(bupc_tentative_alloc);
  CHECK_TENT(bupc_tentative_all_alloc);
  CHECK_TENT(bupc_tentative_free);
  CHECK_TENT(bupc_tentative_all_free);
  CHECK_TENT(bupc_tentative_threadof);
  CHECK_TENT(bupc_tentative_process_thread_layout);
  CHECK_TENT(bupc_tentative_config_info);
}
