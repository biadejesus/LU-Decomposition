#ifndef _UPC_COLLECTIVE_BITS_H_
#define _UPC_COLLECTIVE_BITS_H_

/* This file contains typedefs and constants needed by both the library
 * build and by application builds.  This file contains only C.
 */

/* upc_flag_t and upc_op_t defined under upc_types.h, as per 1.3 UPC Spec */
#include <upcr_preinclude/upc_bits.h>

/* upc_op_t values specific to the collectives library, must be >= 65536 */
#define UPC_FUNC          GEX_OP_USER
#define UPC_NONCOMM_FUNC  GEX_OP_USER_NC

#if (PLATFORM_OS_CATAMOUNT || PLATFORM_OS_CNL) && PLATFORM_COMPILER_GNU
 /* Note: platform macros will not be defined for BUPC first preprocess,
    which prevents folding the following .._LT() into the line above */
 #if PLATFORM_COMPILER_VERSION_LT(4,0,0)
  /* HACK: workaround the fact that gcc and gccupc-3 on Cray XT catamount and CNL crash 
     while building libupcr if long double is used for the collectives. 
     Therefore, long double collectives are treated as having type double for these configs.
   */
  #define UPCRI_COLL_LONG_DOUBLE double
 #endif
#endif
#ifndef UPCRI_COLL_LONG_DOUBLE
  #define UPCRI_COLL_LONG_DOUBLE long double
#endif

#endif
