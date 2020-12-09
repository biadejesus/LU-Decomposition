/*
 * Grisly, preprocessor-addled implementation of Active Message handler wrapper
 * functions.
 *
 * You are not expected to understand this--read the notes in
 * upcr_handler_decls.h, and you shouldn't need to.
 *
 * A wrapper for each handler declared in <upcr_handler_decls.h> is
 * implemented.  The wrapper handles converting 64 bit pointers into two 32
 * bit arguments, and calls the 'real' handler function that the user writes.
 * 
 * This framework assumes that the user implements each handler they declare
 * somewhere--this file doesn't need to see prototypes of them, but the linker
 * will fail if any of them don't exist.
 *
 * An array of all the handlers is constructed, and used to implement the
 * upcri_get_handlertable() function.
 */


#include <upcr_internal.h>

/* 
 * These argument-list macros foreshadow the madness to come...
 */
#define UPCRI_HANDLER_ARGS0
#define UPCRI_HANDLER_ARGS1  , gex_AM_Arg_t a0
#define UPCRI_HANDLER_ARGS2  , gex_AM_Arg_t a0, gex_AM_Arg_t a1
#define UPCRI_HANDLER_ARGS3  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2
#define UPCRI_HANDLER_ARGS4  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3
#define UPCRI_HANDLER_ARGS5  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4
#define UPCRI_HANDLER_ARGS6  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5
#define UPCRI_HANDLER_ARGS7  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6
#define UPCRI_HANDLER_ARGS8  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7
#define UPCRI_HANDLER_ARGS9  , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8
#define UPCRI_HANDLER_ARGS10 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9
#define UPCRI_HANDLER_ARGS11 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10
#define UPCRI_HANDLER_ARGS12 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10, gex_AM_Arg_t a11
#define UPCRI_HANDLER_ARGS13 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10, gex_AM_Arg_t a11\
			       gex_AM_Arg_t a12
#define UPCRI_HANDLER_ARGS14 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
			       gex_AM_Arg_t a12, gex_AM_Arg_t a13
#define UPCRI_HANDLER_ARGS15 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
			       gex_AM_Arg_t a12, gex_AM_Arg_t a13, \
			       gex_AM_Arg_t a14
#define UPCRI_HANDLER_ARGS16 , gex_AM_Arg_t a0, gex_AM_Arg_t a1, \
			       gex_AM_Arg_t a2, gex_AM_Arg_t a3, \
			       gex_AM_Arg_t a4, gex_AM_Arg_t a5, \
			       gex_AM_Arg_t a6, gex_AM_Arg_t a7, \
			       gex_AM_Arg_t a8, gex_AM_Arg_t a9, \
			       gex_AM_Arg_t a10, gex_AM_Arg_t a11, \
			       gex_AM_Arg_t a12, gex_AM_Arg_t a13, \
			       gex_AM_Arg_t a14, gex_AM_Arg_t a15

/*
 * Declare prototypes for handler functions declared in
 * <upcr_handler_decls.h>.
 *
 * This relies on the fact that the result of '##' will be expanded if it
 * resolves to a macro name.  A fine preprocessor feature to know about.
 *
 * Notes:  
 *   1) The user's handler function always has the 32 bit # of args (pointers
 *	are 1 arg): providing that constant # of args across ptr sizes is the
 *	whole point of all this rigmarole.
 *   2) In this file we declare all of the user's args as
 *      gex_AM_Arg_t's, including pointers (we can't do otherwise,
 *      since we don't know which args are pointers).  This is OK, since these
 *      prototypes won't be seen outside this file, and the number of arguments
 *      (and their size) is the same.
 */
#define UPCRI_SHORT_HANDLER(name, cnt32, cnt64, protoargs, call32, call64) \
	  void name protoargs ;
#define UPCRI_MEDIUM_HANDLER(name, cnt32, cnt64, protoargs, call32, call64) \
	  void name protoargs ;
#define UPCRI_LONG_HANDLER(name, cnt32, cnt64, protoargs, call32, call64) \
	  void name protoargs ;
#include <upcr_handler_decls.h>


/*
 * Now define wrapper functions that expand out pointers into 2 parameters as
 * necessary, and then call the user's handler function.
 */
#if PLATFORM_ARCH_32
#  define UPCRI_SHORT_HANDLER(name, cnt32, cnt64, protorgs, call32, call64)  \
          void name##_wrap (gex_Token_t token  UPCRI_HANDLER_ARGS##cnt32) \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call32 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#  define UPCRI_MEDIUM_HANDLER(name, cnt32, cnt64, protorgs, call32, call64) \
	  void name##_wrap (gex_Token_t token, void *addr,		     \
			    size_t nbytes  UPCRI_HANDLER_ARGS##cnt32)	     \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call32 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#  define UPCRI_LONG_HANDLER(name, cnt32, cnt64, protorgs, call32, call64)   \
	  void name##_wrap (gex_Token_t token, void *addr,		     \
			   size_t nbytes  UPCRI_HANDLER_ARGS##cnt32)	     \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call32 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#elif defined(PLATFORM_ARCH_64)
#  define UPCRI_SHORT_HANDLER(name, cnt32, cnt64, protorgs, call32, call64)  \
	  void name##_wrap (gex_Token_t token  UPCRI_HANDLER_ARGS##cnt64) \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call64 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#  define UPCRI_MEDIUM_HANDLER(name, cnt32, cnt64, protorgs, call32, call64) \
	  void name##_wrap (gex_Token_t token, void *addr,		     \
			    size_t nbytes  UPCRI_HANDLER_ARGS##cnt64)	     \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call64 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#  define UPCRI_LONG_HANDLER(name, cnt32, cnt64, protorgs, call32, call64)   \
	  void name##_wrap (gex_Token_t token, void *addr,		     \
			   size_t nbytes  UPCRI_HANDLER_ARGS##cnt64)	     \
          {   UPCRI_BEGIN_AMHANDLER();                                       \
              name call64 ;                                                  \
              UPCRI_END_AMHANDLER();                                         \
	  }
#endif

#include <upcr_handler_decls.h>


/*
 * Now make a table of the wrapper functions and their IDs.
 * This is the only place where we make a Request/Reply/ReqRep distinction
 */

typedef void (*handler_fn)();  /* prototype for generic handler function */


#if PLATFORM_ARCH_32
  #define UPCRI_HANDLER_ENTRY(name, cat, reqrep, cnt32, cnt64) \
	{ UPCRI_HANDLER_ID(name), (handler_fn) name##_wrap, \
          GEX_FLAG_AM_##cat|GEX_FLAG_AM_##reqrep, cnt32, NULL, _STRINGIFY(name) },
#elif PLATFORM_ARCH_64
  #define UPCRI_HANDLER_ENTRY(name, cat, reqrep, cnt32, cnt64) \
	{ UPCRI_HANDLER_ID(name), (handler_fn) name##_wrap, \
          GEX_FLAG_AM_##cat|GEX_FLAG_AM_##reqrep, cnt64, NULL, _STRINGIFY(name) },
#endif

#define UPCRI_SHORT_REQUEST(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, SHORT,  REQUEST, (cnt32), (cnt64))
#define UPCRI_SHORT_REPLY(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, SHORT,  REPLY,   (cnt32), (cnt64))
#define UPCRI_SHORT_REQREP(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, SHORT,  REQREP,  (cnt32), (cnt64))
#define UPCRI_MEDIUM_REQUEST(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, MEDIUM, REQUEST, (cnt32), (cnt64))
#define UPCRI_MEDIUM_REPLY(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, MEDIUM, REPLY,   (cnt32), (cnt64))
#define UPCRI_MEDIUM_REQREP(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, MEDIUM, REQREP,  (cnt32), (cnt64))
#define UPCRI_LONG_REQUEST(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, LONG,   REQUEST, (cnt32), (cnt64))
#define UPCRI_LONG_REPLY(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, LONG,   REPLY,   (cnt32), (cnt64))
#define UPCRI_LONG_REQREP(name, cnt32, cnt64, protorgs, call32, call64) \
          UPCRI_HANDLER_ENTRY(name, LONG,   REQREP,  (cnt32), (cnt64))

static gex_AM_Entry_t handler_table[] = {
#   include <upcr_handler_decls.h>
    /* one last bogus entry, since last must not have a comma */
    { 0, NULL, 0, 0, NULL, NULL }
};

/*
 * Provide the table and the number of entries in it to the initialization
 * functions.
 */
gex_AM_Entry_t * upcri_get_handlertable(void)
{
    return handler_table;
}

size_t upcri_get_handlertable_count(void)
{
    return (sizeof(handler_table) / sizeof(gex_AM_Entry_t)) - 1;
}

