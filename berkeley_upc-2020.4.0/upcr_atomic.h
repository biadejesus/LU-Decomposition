/*   $Source: bitbucket.org:berkeleylab/upc-runtime.git/upcr_atomic.h $
 * Description: Berkeley UPC atomics support - types and prototypes 
 *
 * Copyright 2006, Dan Bonachea <bonachea@cs.berkeley.edu>
 */


#ifndef UPCR_ATOMIC_H
#define UPCR_ATOMIC_H

#include <upcr_preinclude/upc_types.h>
#include <upcr_preinclude/upc_atomic.h>

// AM-based algorithm to handle 128-bit AMOs not yet natively supported in GASNet
#ifndef UPCRI_USE_AM_PTS_AMO
#define UPCRI_USE_AM_PTS_AMO !UPCRI_PACKED_SPTR
#endif

#endif /* UPCR_ATOMIC_H */
