/*
 * Broadcast function for collective allocations and a few other startup tasks.
 */

#include <upcr_internal.h>

GASNETT_THREADKEY_DEFINE(_upcri_broadcast_key);

/* Broadcasts data from one UPC thread to all others.
 *
 * Does not guarantee that all threads have received the data when any one of
 * them returns from this function--use a barrier after the function if you
 * need that (like UPC's MY/MY or MPI).
 *
 */
void 
_upcri_broadcast(upcr_thread_t from_thread, void *addr, size_t len UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
#if 1
    /* Work around a behavior in which root thread just sends AMs to others
       so fast that they expend unreasonable amounts of memory to store the
       "eager state" associated with collectives they have not yet reached.
       This is an issue with many back-to-back broadcasts as will happen
       in things like benchmarks of upc_all_alloc() and upc_all_lock_alloc().
       XXX: Real fix belongs in GASNet!
    */
    const unsigned int sync_spacing = 64; /* Must be a power of 2 */
    uintptr_t counter = (uintptr_t)gasnett_threadkey_get(_upcri_broadcast_key);
    const int flags = GASNET_COLL_LOCAL | GASNET_COLL_IN_MYSYNC |
                      ((++counter & (sync_spacing-1)) ? GASNET_COLL_OUT_MYSYNC : GASNET_COLL_OUT_ALLSYNC);

    gasnett_threadkey_set(_upcri_broadcast_key, (void*)counter);
    upcri_assert(UPCRI_IS_POWER_OF_TWO(sync_spacing));
#else
    const int flags = GASNET_COLL_LOCAL | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC;
#endif

#if UPCRI_UPC_PTHREADS // TODO-EX: this a temporary work-around for lack of multi-image support in GEX
    // Step 0: distinguish one pthread per proccess
    gex_Rank_t from_node = upcri_thread2node[from_thread];
    int lead_thread = (from_node == upcri_mynode) ? upcri_thread2pthread[from_thread] : 0;
    int i_am_leader = upcri_mypthread() == lead_thread;
    // Step 1: one thread per process performs the bcast and publishes the result
    static void *ptr;
    if (i_am_leader) {
      gasnet_coll_broadcast(GASNET_TEAM_ALL, addr, from_node, addr, len, flags | GASNET_COLL_NO_IMAGES);
      ptr = addr;
    }
    upcri_pthread_barrier();
    // Step 2: remaining threads copy from the local leader
    if (!i_am_leader) {
      memcpy(addr, ptr, len);
    }
    upcri_pthread_barrier();
#else
    gasnet_coll_broadcast(GASNET_TEAM_ALL, addr, from_thread, addr, len, flags);
#endif
}
