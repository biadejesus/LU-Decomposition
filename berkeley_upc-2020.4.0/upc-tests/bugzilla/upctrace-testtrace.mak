TRACEFILE= 
CC=cc
CFLAGS= 
LD=$(CC) $(CFLAGS)
LDFLAGS=
LIBS=

upctrace-testtrace: upctrace-testtrace.c
	$(CC) $(CFLAGS) -o $@.o -c $<
	$(LD) $(LDFLAGS) -o $@ $@.o $(LIBS)
	if test $$TOP_BUILDDIR != $$HARNESS_WORKDIR; then \
	  cp $$TOP_BUILDDIR/gasnet/other/contrib/gasnet_trace.pl $$HARNESS_WORKDIR/upc_trace.pl; \
	  cp $$TOP_BUILDDIR/gasnet/other/contrib/gasnet_trace    $$HARNESS_WORKDIR/upc_trace;    \
	fi
upctrace-testtrace_st%: upctrace-testtrace.c
	$(CC) $(CFLAGS) -o $@.o -c $<
	$(LD) $(LDFLAGS) -o $@ $@.o $(LIBS)

