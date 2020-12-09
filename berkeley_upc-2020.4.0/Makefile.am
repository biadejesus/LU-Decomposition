AUTOMAKE_OPTIONS  = foreign 1.4 no-dependencies
ACLOCAL_AMFLAGS   = -I gasnet

SUBDIRS       = gasnet umalloc upcr_preinclude upcr_geninclude upcr_extinclude \
		upcr_postinclude man detect-upc contrib harness config-aux \
		upcr_geninclude2 docs profile .

DIST_SUBDIRS = $(SUBDIRS)

# .pl files to install in bindir, but w/o exec permissions
dotpldir = $(bindir)
dotpl_DATA = upcc.pl upcrun.pl

# .ld files to (optionally) install in libdir, but w/o treatment as libs
ld_scripts = upc_shared.ld
if USING_LD_SCRIPT
dotlddir = $(libdir)
dotld_DATA = $(ld_scripts)
endif

makefile_fragments = upcr.mak upcc.mak $(gcc_as_cc_mak)
sysconf_DATA  = upcc.conf upcrun.conf
bin_SCRIPTS   = upcc upcrun
noinst_SCRIPTS= gcc_as_cc.pl
config_DATA   = $(CONFIG_HEADER)
configdir     = $(includedir)
docs          = ChangeLog README INSTALL.TXT
# deliberately omitted git-only docs: README.developers README-gasnet
EXTRA_DIST    = $(docs)                         \
		.autom4te.cfg                   \
                Bootstrap                       \
                unBootstrap                     \
                README                          \
                LICENSE.TXT                     \
                upc-examples                    \
                upc-tests                       \
                gcc_as_cc.pl                    \
		Makefile.multiconf              \
		multiconf.conf.in               \
		multiconf_gupc.conf.in          \
		multiconf_cupc.conf.in          \
		multiconf_cupc2c.conf.in        \
		multiconf.pl                    \
		upcc_multi.in                   \
		upcc_multi.pl                   \
		$(ld_scripts)                   \
		$(dotpl_DATA)

# .pl libraries needed by $(dotpl_DATA)
perl_libs = \
	upcr_ctuple.pl \
	upcr_getopt.pl \
	upcr_util.pl

libupcr_hdrs= upcr.h upcr_alloc.h upcr_barrier.h                    	\
                 upcr_err.h upcr_handler_decls.h upcr_handlers.h       	\
                 upcr_init.h upcr_internal.h upcr_locks.h              	\
                 upcr_threads.h upcr_shaccess.h upcr_sptr.h            	\
                 upcr_util.h upcr_translator_tld.h       		\
                 upcr_memcpy.h upcr_collective.h        		\
                 bupc_extern.h bupc_tentative.h upcr_io.h upcr_gupc.h upcr_syshdrs.h   \
                 upcr_sem.h upcr_profile.h upcr_atomic.h

include_HEADERS= $(perl_libs) $(libupcr_hdrs)

#
# Each version of the runtime library is built via a recursive make call--we
# need this since each requires different CFLAGS settings, GASNET libs, etc.  
#
# We include upcr.mak, which figures out which GASNet .mak fragment has the
# correct settings for the version of the runtime library we want to build.
# We have to do this in a separate file since we can't put GNU make conditionals
# in this file (or automake will barf).
#
# Silly: automake uses 'include' for its own purposes, so use an autoconf
# variable to sneak one past it to regular GNU make...
@MAKE_INCLUDE@ upcr.mak

libupcr_srcs = upcr_alloc.c upcr_barrier.c upcr_err.c upcr_init.c \
               upcr_locks.c upcr_broadcast.c upcr_handlers.c \
               upcr_sptr.c upcr_threads.c upcr_util.c upcr_globfiles.c \
               upcr_gupc.c upcr_memcpy.c upcr_collective.c upcr_coll_init.c \
               upcr_coll_templates.c upcr_extern.c upcr_io.c upcr_sem.c \
               upcr_sizes.c upcr_profile.c upcr_atomic.c

# Because of tracing/stats macros we must recompile when GASNet headers change.
# We avoid needing to keep the list current by using overkill.
# Note that using $(shell find ...) makes automake very unhappy.
gasnet_hdrs = gasnet/gasnet_config.h  \
              $(srcdir)/gasnet/*.h    \
              $(srcdir)/gasnet/*/*.h  \
              $(srcdir)/gasnet/*/*/*.h

# Umalloc dir has exectly 2 public headers that we use
umalloc_hdrs = $(srcdir)/umalloc/umalloc.h $(srcdir)/umalloc/umalloc_2_upcr.h

libupcr_deps = $(libupcr_srcs) $(libupcr_hdrs) $(gasnet_hdrs) acconfig.h \
	       upcr_config.h upcr_preinclude/upc_collective_bits.h \
	       upcr_preinclude/upc_io_bits.h upcr_preinclude/upc_bits.h \
               $(umalloc_hdrs)

# List of all libraries that can be built: needed by automake
EXTRA_LIBRARIES     = libupcr-mpi-seq.a    libupcr-mpi-par.a    libupcr-mpi-thr.a  \
                      libupcr-udp-seq.a    libupcr-udp-par.a    libupcr-udp-thr.a  \
                      libupcr-ibv-seq.a    libupcr-ibv-par.a    libupcr-ibv-thr.a  \
                      libupcr-smp-seq.a    libupcr-smp-par.a    libupcr-smp-thr.a  \
                      libupcr-aries-seq.a  libupcr-aries-par.a  libupcr-aries-thr.a \
                      libupcr-ofi-seq.a    libupcr-ofi-par.a    libupcr-ofi-thr.a \
                      libupcr-ucx-seq.a    libupcr-ucx-par.a    libupcr-ucx-thr.a

# list of libraries to actually build, determined at configure time
lib_LIBRARIES       = @TARGETLIBS@

LIBTESTS=$(lib_LIBRARIES:.a=-test)

# Each version of the library is built via a recursive make call with
# different parameters

libupcr_mpi_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-mpi-seq.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=mpi UPCR_PARSEQ=seq build_lib

libupcr_mpi_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-mpi-par.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=mpi UPCR_PARSEQ=par build_lib

libupcr_mpi_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-mpi-thr.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=mpi UPCR_PARSEQ=thr build_lib

libupcr_udp_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-udp-seq.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=udp UPCR_PARSEQ=seq build_lib

libupcr_udp_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-udp-par.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=udp UPCR_PARSEQ=par build_lib

libupcr_udp_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-udp-thr.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=udp UPCR_PARSEQ=thr build_lib

libupcr_smp_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-smp-seq.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=smp UPCR_PARSEQ=seq build_lib

libupcr_smp_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-smp-par.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=smp UPCR_PARSEQ=par build_lib

libupcr_smp_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-smp-thr.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=smp UPCR_PARSEQ=thr build_lib

libupcr_ibv_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ibv-seq.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=ibv UPCR_PARSEQ=seq build_lib

libupcr_ibv_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ibv-par.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=ibv UPCR_PARSEQ=par build_lib

libupcr_ibv_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ibv-thr.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=ibv UPCR_PARSEQ=thr build_lib

libupcr_aries_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-aries-seq.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=aries UPCR_PARSEQ=seq build_lib

libupcr_aries_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-aries-par.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=aries UPCR_PARSEQ=par build_lib

libupcr_aries_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-aries-thr.a: $(libupcr_deps) 
	$(MAKE) UPCR_CONDUIT=aries UPCR_PARSEQ=thr build_lib

libupcr_ofi_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ofi-seq.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ofi UPCR_PARSEQ=seq build_lib

libupcr_ofi_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ofi-par.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ofi UPCR_PARSEQ=par build_lib

libupcr_ofi_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ofi-thr.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ofi UPCR_PARSEQ=thr build_lib

libupcr_ucx_seq_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ucx-seq.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ucx UPCR_PARSEQ=seq build_lib

libupcr_ucx_par_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ucx-par.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ucx UPCR_PARSEQ=par build_lib

libupcr_ucx_thr_a_SOURCES   = $(libupcr_srcs) $(libupcr_hdrs)
libupcr-ucx-thr.a: $(libupcr_deps)
	$(MAKE) UPCR_CONDUIT=ucx UPCR_PARSEQ=thr build_lib

if UPCR_OBJECT_STRINGS
else
# dirty hack to make our sizes strings scannable on platforms like X1, 
# where character arrays are stored compressed in the object file
# build and link a minimal GASNet program including our strings, 
# then scan them into a header where libupcr can compile them in using GASNETI_IDENT
UPCR_SIZES_HEADER=upcr_sizes-$(UPCR_CONDUIT)-$(GASNET_PARSEQ).h
UPCR_SIZES_ACTIVATE=-DUPCRI_SIZES_H=$(UPCR_SIZES_HEADER)
UPCR_SIZES_BASE=upcr_sizes-$(UPCR_CONDUIT)-$(GASNET_PARSEQ)
UPCR_SIZES_CLEAN=upcr_sizes-*-*.h
$(UPCR_SIZES_HEADER): $(libupcr_deps)
	$(UPCR_CC) -c $(EXTRA) $(UPCR_CPPFLAGS) -DUPCRI_SIZES_DUMMY $(UPCR_CFLAGS) \
          -o $(UPCR_SIZES_BASE).o $(srcdir)/upcr_sizes.c 
	$(UPCR_LD) $(UPCR_LDFLAGS) -o $(UPCR_SIZES_BASE) $(UPCR_SIZES_BASE).o $(GASNET_LIBS)
	@PERL@ -044 -ne 'if (/^(UPCRSizeof: [^\$$]+\$$)/) { \
           print "GASNETT_IDENT(upcri_IdentString_sizes_".++$$i.",\"\$$$$1\");\n";}' \
           $(UPCR_SIZES_BASE) > $@
	test -s $@
	rm -f $(UPCR_SIZES_BASE) $(UPCR_SIZES_BASE).o
endif

if BUILDCONFIG_GASP
  UPCR_GASP_CPPFLAGS=-DUPCRI_INST_UPCCFLAG=1 -I $(srcdir)/profile
endif

# templatized library build 
$(gasnet_makfile):
	$(MAKE) -C gasnet/$(UPCR_CONDUIT)-conduit $(UPCR_CONDUIT)-$(GASNET_PARSEQ).mak

UPCR_LIB_COMPILE = $(UPCR_CC) $(EXTRA) $(UPCR_CPPFLAGS_LIB) $(UPCR_GASP_CPPFLAGS) $(UPCR_SIZES_ACTIVATE) $(GASNET_DEVWARN_CFLAGS) $(UPCR_CFLAGS) -DUPCRI_BUILDING_LIBUPCR=1
upcr_reduce_types = C UC S US I UI L UL F D LD
upcr_reduce_objs = $(foreach type,$(upcr_reduce_types),upcr_reduce$(type)$(upcr_lib_suffix).o) \
		   $(foreach type,$(upcr_reduce_types),upcr_prefix_reduce$(type)$(upcr_lib_suffix).o)
upcr_reduce_flags = -DUPCRI_COLL_RED_OPS=UPCRI_COLL_$(UPCR_RED_OPS) -DUPCRI_COLL_RED_SUFF=$(UPCR_RED_SUFF) -DUPCRI_COLL_RED_TYPE="$(UPCR_RED_TYPE)"

build_reduce:
	$(UPCR_LIB_COMPILE) $(upcr_reduce_flags) -DUPCRI_BUILD_REDUCE \
		-c -o upcr_reduce$(UPCR_RED_SUFF)$(upcr_lib_suffix).o $(srcdir)/upcr_coll_templates.c
	$(UPCR_LIB_COMPILE) $(upcr_reduce_flags) -DUPCRI_BUILD_PREFIX_REDUCE \
		-c -o upcr_prefix_reduce$(UPCR_RED_SUFF)$(upcr_lib_suffix).o $(srcdir)/upcr_coll_templates.c

upcr_lib_suffix = -$(UPCR_CONDUIT)-$(UPCR_PARSEQ)
upcr_lib_dot_a = libupcr$(upcr_lib_suffix).a
upcr_lib_srcs  = upcr_globfiles.c upcr_collective.c upcr_memcpy.c upcr_io.c upcr_extern.c
upcr_lib_objs  = $(upcr_lib_srcs:.c=$(upcr_lib_suffix).o)

$(upcr_lib_objs):
	$(UPCR_LIB_COMPILE) -c -o $@ $(srcdir)/$(@:$(upcr_lib_suffix).o=.c)

build_lib: $(gasnet_makfile) $(UPCR_SIZES_HEADER)
	-rm -f $(upcr_lib_objs) $(upcr_reduce_objs)
	@$(MAKE) $(upcr_lib_objs)
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=C  UPCR_RED_TYPE='signed char'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=UC UPCR_RED_TYPE='unsigned char'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=S  UPCR_RED_TYPE='signed short'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=US UPCR_RED_TYPE='unsigned short'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=I  UPCR_RED_TYPE='signed int'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=UI UPCR_RED_TYPE='unsigned int'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=L  UPCR_RED_TYPE='signed long'
	@$(MAKE) build_reduce UPCR_RED_OPS=INT UPCR_RED_SUFF=UL UPCR_RED_TYPE='unsigned long'
	@$(MAKE) build_reduce UPCR_RED_OPS=NONINT UPCR_RED_SUFF=F  UPCR_RED_TYPE='float'
	@$(MAKE) build_reduce UPCR_RED_OPS=NONINT UPCR_RED_SUFF=D  UPCR_RED_TYPE='double'
	@$(MAKE) build_reduce UPCR_RED_OPS=NONINT UPCR_RED_SUFF=LD UPCR_RED_TYPE='UPCRI_COLL_LONG_DOUBLE'
	$(AR) cru $(upcr_lib_dot_a) $(upcr_lib_objs) $(upcr_reduce_objs)
	$(RANLIB) $(upcr_lib_dot_a)
	-rm -f $(upcr_lib_objs) $(upcr_reduce_objs)

# make sure certain files get rebuilt if their source files changed.
all-local: $(makefile_fragments) $(sysconf_DATA) $(bin_SCRIPTS) \
	$(perl_libs) $(lib_LIBRARIES) $(dotld_DATA) upc_trace

# Things to clean
CLEANFILES = $(LIBTESTS) $(UPCR_SIZES_CLEAN) $(ctuple_DATA)
DISTCLEANFILES = config.status.bak upc_trace $(files_we_copy) $(gcc_as_cc_mak)
distclean-local:
	rm -Rf .subst_text

# Bug3285: the behavior of distclean-recursive varies in incompatible ways
# across automake versions, such that no matter what variables we specify
# some versions will break the dependencies between subdirectories.
# Very early automakes ignore DIST_SUBDIRS entirely.
# We override it with our own top-level rule to ensure consistent ordering.
distclean: force
	@rev=''; for subdir in $(DIST_SUBDIRS); do          \
          if test "$$subdir" = "."; then :; else            \
            rev="$$subdir $$rev";                           \
          fi;                                               \
        done;                                               \
	for dir in $$rev; do                                \
           if test -f "$$dir/Makefile"; then                \
             ( cd "$$dir" && $(MAKE) distclean ) || exit 1; \
           fi;                                              \
         done
	$(MAKE) $(AM_MAKEFLAGS) distclean-am
	rm -f $(am__CONFIG_DISTCLEAN_FILES)
	rm -f config.status upcr_config.h stamp-h1
	rm -f Makefile


$(top_builddir)/gasnet/other/perlstart: $(top_srcdir)/gasnet/other/perlstart.in
	@$(MAKE) -C $(top_builddir)/gasnet/other perlstart
upcc: $(top_builddir)/gasnet/other/perlstart upcc.pl $(perl_libs)
upcrun: $(top_builddir)/gasnet/other/perlstart upcrun.pl $(perl_libs)

upc_trace: force
	@$(MAKE) -C $(top_builddir)/gasnet/other/contrib gasnet_trace
	@rm -f $@
	ln -s $(top_builddir)/gasnet/other/contrib/gasnet_trace $@

# Things to copy from srcdir to buildir when they differ
if BUILD_IS_SRC
files_we_copy =
else
files_we_copy =  $(dotld_DATA) $(dotpl_DATA) $(perl_libs)
$(files_we_copy): force
	@srcfile="$(srcdir)/`basename $@`" ; \
         if test ! -f "$@" -o "`find $$srcfile -newer '$@' 2>&1`" ; then \
          echo cp -f "$$srcfile" . ;         \
          cp -f "$$srcfile" . ;              \
         fi
endif

# Support for gcc as substitute preprocessor
if USE_GNU_PREPROC
gcc_as_cc_mak = gcc_as_cc.mak
gcc_as_cc.mak: $(srcdir)/gcc_as_cc.pl
	@GCC_AS_CC_CMD@
else
gcc_as_cc_mak =
endif

install-data-local: $(makefile_fragments)
	$(mkinstalldirs) $(DESTDIR)$(includedir)
	@list='$(makefile_fragments)'; for p in $$list; do \
	  if test -f $$p; then \
	    filename=`basename $$p`; \
	    echo @GASNET_INSTALL_CMD@ " < $$p > $(DESTDIR)$(includedir)/$$filename"; \
	    @GASNET_INSTALL_CMD@ < $$p > $(DESTDIR)$(includedir)/$$filename || exit $$? ; \
	  else :; fi; \
	done 

install-exec-local:
	@rm -f $(DESTDIR)$(bindir)/upc_trace
	ln -s gasnet_trace $(DESTDIR)$(bindir)/upc_trace

uninstall-local:
	@rm -f $(DESTDIR)$(bindir)/upc_trace
	@list='$(makefile_fragments)'; for p in $$list; do \
	  if test -f $$p; then \
	    filename=`basename $$p`; \
	    echo " rm -f $(DESTDIR)$(includedir)/$$filename;"; \
	    rm -f $(DESTDIR)$(includedir)/$$filename || exit $$? ; \
	  else :; fi; \
	done

dist-hook:
	@if grep -q 'cache_file=/dev/null' $(srcdir)/configure; then \
          echo ERROR: Refusing to distribute configure not generated by Bootstrap. Please start with a clean source tree and Bootstrap. ; \
          exit 1 ; \
        fi
	if test -d $(srcdir)/.git ; then \
	  UPCR_DESC=`$${GIT=git} --git-dir=$(srcdir)/.git describe` || exit $$? ; \
	  $(PERL) -pi -e "s:no-version-control-info:$$UPCR_DESC:;" -- $(distdir)/configure || exit $$? ; \
	  echo $$UPCR_DESC > $(distdir)/version.git || exit $$? ; \
	fi
	@: # Prune some dirs from the upc-tests:
	rm -rf $(distdir)/upc-tests/NPB2.3 # Waaay out-of-date
	rm -rf $(distdir)/upc-tests/NPB2.4 # Superseded by gwu-npb-upc
	rm -rf $(distdir)/upc-tests/perverse\ naming # Too much "collateral damage"
	rm -rf $(distdir)/upc-examples/HPCS/gups # Buggy
	@: # automake-1.6 + autoconf-2.53 break distcheck w/ unresolved SUBSTs
	@list=`grep '^[A-Za-z0-9_]* = @' Makefile | cut -d' ' -f1`; \
	 if test -n "$$list"; then \
	   expr=`echo $$list | tr ' ' '|'`; \
	   cmd="$(PERL) -pi -e 's/^($$expr) = @.*/#\$$&/;' -- $(distdir)/Makefile.in"; \
	   echo $$cmd; eval $$cmd; \
	 fi

# If we've disabled LEX, ensure it stays disabled for 'make distcheck'
if UPCR_USE_LEX
UPCR_LEX_CONFIG_FLAG=
else
UPCR_LEX_CONFIG_FLAG=--without-lex
endif
DISTCHECK_CONFIGURE_FLAGS = $(UPCR_LEX_CONFIG_FLAG) --without-multiconf

force:

if BUILDCONFIG_GASP
  TEST_BINDIR=$(top_builddir)/profile/dump
  UPCC_TEST=$(TEST_BINDIR)/upcc-dump -inst
  TESTPROGRAM=$(top_srcdir)/upc-tests/benchmarks/gasp_test.upc 
else
  # tests backend compile only
  # TESTPROGRAM=$(top_srcdir)/upc-tests/upcr-tests/test-compiles/compiles.trans.c
  # tests translate and backend compile
  TESTPROGRAM=$(top_srcdir)/upc-tests/bugzilla/bug87.upc

  TEST_BINDIR=$(top_builddir)
  UPCC_TEST=$(TEST_BINDIR)/upcc
endif

# targets conduitname-{seq,par,thr,all} build everything required 
# to build and run a working UPC program using that conduit config
# We break this up in to distinct -seq, -par and -thr rules to avoid bug 2355
one-target: force
	@conduit=`echo '$(TARGET)' | $(PERL) -pe 's/-[^-]+$$//'` ; 		\
	 conduitcfg=`echo '$(TARGET)' | $(PERL) -pe 's/[^-]+-//; s/thr/par/'` ;\
         $(MAKE) -C gasnet/$$conduit-conduit $$conduitcfg
	@$(MAKE) -C profile all
	@$(MAKE) libupcr-$(TARGET).a libupcr-$(TARGET).ct
	@$(MAKE) -C detect-upc all
	@$(MAKE) -C umalloc all
	@$(MAKE) -C upcr_geninclude all
	@$(MAKE) -C upcr_geninclude2 all
	@$(MAKE) $(bin_SCRIPTS) $(sysconf_DATA) $(makefile_fragments) $(dotld_DATA)
%-seq: force 
	@$(MAKE) one-target TARGET=$@
%-par: force 
	@$(MAKE) one-target TARGET=$@
%-thr: force 
	@$(MAKE) one-target TARGET=$@

%-all: force
	@conduit=`echo '$@' | $(PERL) -pe 's/-[^-]+$$//'` ; 	\
	$(MAKE) $$conduit-seq || exit $$? ; 			\
	$(MAKE) $$conduit-par || exit $$? ; 			\
	$(MAKE) $$conduit-thr || exit $$? ;

# .ct (ctuple) files to install in libdir
ctupledir = $(libdir)
ctuple_DATA=$(lib_LIBRARIES:.a=.ct)
%.ct: %.a
	-rm -f $@
	$(PERL) -044 -ne 'print "\$$$$_\n" if m/^(UPC|GASNet)\S+: /' < $< > $@


# targets for tests

one-test: force
	@conduit=`echo '$(TARGET)' | cut -d- -f2` ;    \
	 case    `echo '$(TARGET)' | cut -d- -f3` in  \
         seq) flags='-nopthreads'                 ;;   \
         par) flags='-pthreads -nolink-cache'     ;;   \
         thr) flags='-uses-threads'               ;;   \
         esac;                                         \
	cmd="$(UPCC_TEST) -o $(TARGET) -network $$conduit $$flags $(TESTPROGRAM)"; \
	echo $$cmd; eval $$cmd

libupcr-%-test: %
	$(MAKE) one-test TARGET=$@

tests-installed:  # Note: does NOT build any dependencies
	@if test -n "$(DESTDIR)"; then \
	   echo 'SKIPPING tests-installed for non-empty $$(DESTDIR)'; \
	 else \
	   for target in $(LIBTESTS); do \
	      echo $(MAKE) $$target TEST_BINDIR=$(bindir); \
	      $(MAKE) one-test TARGET=$$target TEST_BINDIR=$(bindir) || exit $$?; \
	   done; \
	 fi

tests-local:
	$(MAKE) $(LIBTESTS)

tests-hello:
	@$(MAKE) $(LIBTESTS) TESTPROGRAM=$(top_srcdir)/upc-examples/hello.upc
	@echo ' '
	@echo '       ==== UPC "HELLO WORLD" TESTS SUCCESSFULLY COMPILED ===='
	@echo ' '
	@echo 'Your UPC compilation setup appears to be working for all detected networks:'
	@echo ' '
	@echo '      $(CONDUITS)'
	@echo ' '
	@echo 'You should now test UPC runtime operation for each network of interest. '
	@if test -x libupcr-smp-par-test ; then \
	 echo 'Start by testing the single-node smp/pthreaded network, with a command like:' ;  \
	 echo ' ' ; \
	 dir='@MULTICONF_NAME@'; \
	 echo "  ./upcrun -n 2 $${dir:-.}/libupcr-smp-par-test" ;  \
	fi
	@if test "@DEFAULT_CONDUIT@" != "smp" ; then \
	 echo ' ' ; \
	 echo 'Next, ensure you are in a proper environment for launching parallel jobs' ; \
	 echo '(eg batch system session, if necessary) and then issue a command like:' ; \
	 echo ' ' ; \
	 dir='@MULTICONF_NAME@'; \
	 echo "  ./upcrun -n 2 $${dir:-.}/libupcr-@DEFAULT_CONDUIT@-seq-test" ; \
	 echo ' ' ; \
	 echo 'And similarly for any other networks listed above.' ; \
	fi
	@echo ' '

tests-clean:
	-rm -f $(LIBTESTS)
	$(MAKE) -C $(top_builddir)/gasnet $@

tests tests-seq tests-par tests-parsync run-tests run-tests-seq run-tests-par run-tests-parsync: tests-local
	$(MAKE) -C $(top_builddir)/gasnet $@

test: tests

check: tests-local check-exports

if GNU_NM
GCCUPC_EXPORTS = -e ' [\._]*upc_' -e ' [\._]*gccupc_' \
                 -e ' [\._]*THREADS' -e ' [\._]*MYTHREAD' \
		 -e ' [\.]*___*put' -e ' [\.]*___*get'  \
		 -e ' [\.]*___*cvtaddr' -e ' [\.]*__copy'
if USING_GCCUPC
EXTRA_EXPORTS = $(GCCUPC_EXPORTS)
endif
if USING_CUPC
EXTRA_EXPORTS = $(GCCUPC_EXPORTS)
endif

check-exports: $(lib_LIBRARIES)
	@echo Checking libupcr exports...
	@if test x$(CHECK_EXPORTS) = x0; then                                       \
	  echo Skipped by user request ;                                            \
	  exit 0 ;                                                                  \
	 fi ;                                                                       \
         failed=0 ;                                                                 \
	 for lib in $(lib_LIBRARIES) ; do                                           \
          echo $$lib: ;                                                             \
          echo ;                                                                    \
          @NM@ --defined-only $$lib |                                               \
	    @PERL@ -pe 's/ \.refptr\.//' |                                          \
            grep -v -e ' [\._]*upcr_' -e ' [\._]*upcri_' -e ' [\._]*bupc_'          \
                    -e ' [\._]*pupc_'                                               \
                    -e ' [\._]*gasnet_' -e ' [\._]*gasnet[tiecX]_'                  \
                    -e __FUNCTION__ -e __PRETTY_FUNCTION__ -e ' [\._]*DWinfo'       \
                    -e ' [\._]*TV_s2s_' -e ' [\._]*MPIR_'                           \
                    -e ' [\._][\._]*debug_' -e 'get_pc_thunk'                       \
                    -e ' [\._]*stab' -e ' [\._]*gnu_dev_' -e ' [\._]*emutls_'       \
                    $(EXTRA_EXPORTS)  |                                             \
            @PERL@ -n -e 'print if /^[0-9a-fA-F]+\s+[A-Z]\s+/' > .$$lib.exp;        \
          if test -s .$$lib.exp ; then                                              \
            cat .$$lib.exp ;                                                        \
            echo FAILED ;                                                           \
            failed=1 ;                                                              \
          else                                                                      \
            echo PASSED ;                                                           \
          fi ;                                                                      \
          rm -f .$$lib.exp ;                                                        \
        done ; exit $$failed
else
check-exports: $(lib_LIBRARIES)
	@echo check-exports test SKIPPED
endif
#ends 'if GNU_NM'

.PHONY: force test tests tests-local build_lib build_reduce dist-hook check-exports one-target one-test

# Unfortunately, we know something is wrong with parallel make and UPCR.
# Ideally we'd FIX it, but the problem is very hard to isolate.
.NOTPARALLEL:
