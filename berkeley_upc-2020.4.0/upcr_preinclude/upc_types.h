/* Reference implementation of upc_types.h 
 * Copyright 2012, The Regents of the University of California 
 * This code is under BSD license: https://upc.lbl.gov/download/dist/LICENSE.TXT
 */

#ifndef _UPC_TYPES_H_
#define _UPC_TYPES_H_

#include <stdint.h>
#include <gasnet_fwd.h> // for op definitions

typedef unsigned long upc_op_t;

#define UPC_ADD         GEX_OP_ADD
#define UPC_MULT        GEX_OP_MULT
#define UPC_AND         GEX_OP_AND
#define UPC_OR          GEX_OP_OR
#define UPC_XOR         GEX_OP_XOR
#define UPC_MIN         GEX_OP_MIN
#define UPC_MAX         GEX_OP_MAX

// TODO-EX: GEX currently lacks LOG(AND,OR), so use these two values chosen to avoid meaningful collision
#define UPC_LOGAND      (1<<12)
#define UPC_LOGOR       (1<<13)

typedef int upc_type_t;

#define UPC_CHAR        1
#define UPC_UCHAR       2 
#define UPC_SHORT       3  
#define UPC_USHORT      4 
#define UPC_INT         5 
#define UPC_UINT        6 
#define UPC_LONG        7 
#define UPC_ULONG       8 
#define UPC_LLONG       9 
#define UPC_ULLONG      10 
#define UPC_INT8        11 
#define UPC_UINT8       12 
#define UPC_INT16       13 
#define UPC_UINT16      14 
#define UPC_INT32       15 
#define UPC_UINT32      16 
#define UPC_INT64       17 
#define UPC_UINT64      18 
#define UPC_FLOAT       19 
#define UPC_DOUBLE      20 
#define UPC_LDOUBLE     21 
#define UPC_PTS         22 

typedef int upc_flag_t;

#ifdef UPC_IN_ALLSYNC
/* this is not required for conformance, but helpful for 1.2->1.3 transition period */
#undef UPC_IN_ALLSYNC
#undef UPC_IN_MYSYNC   
#undef UPC_IN_NOSYNC   
#undef UPC_OUT_ALLSYNC 
#undef UPC_OUT_MYSYNC  
#undef UPC_OUT_NOSYNC  
#endif

#define UPC_IN_ALLSYNC       (1<<0)
#define UPC_IN_MYSYNC        (1<<1)
#define UPC_IN_NOSYNC        (1<<2)
#define UPC_OUT_ALLSYNC      (1<<3)
#define UPC_OUT_MYSYNC       (1<<4)
#define UPC_OUT_NOSYNC       (1<<5)

#endif /* _UPC_TYPES_H */
