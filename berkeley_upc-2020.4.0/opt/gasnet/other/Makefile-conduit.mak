# This Makefile fragment is used to build GASNet conduits 
# it is not meant to be used directly 
# other/Makefile-conduit.mak.  Generated from Makefile-conduit.mak.in by configure.

.PHONY: do-error do-make-fragment libs force clean-local        \
        do-clean-local do-install-data-local do-uninstall-local \
	do-install-data-conduit do-uninstall-conduit \
	test tests tests-seq tests-par tests-parsync tests-clean \
	tests-check do-run-testexit 

# header files to be installed
# Conduit-specific headers will overwrite any conflicts in extended-ref
headers = $(top_srcdir)/extended-ref/*.h                    \
          $(top_srcdir)/extended-ref/vis/gasnet_vis_fwd.h   \
          $(top_srcdir)/extended-ref/ratomic/gasnet_ratomic_fwd.h   \
          $(top_srcdir)/extended-ref/coll/gasnet_coll_fwd.h   \
          $(srcdir)/*.h                                     \
	  $(CONDUIT_EXTRAHEADERS)

# headers to exclude from $(headers) - please don't use wildcards
private_headers = \
                  $(CONDUIT_PRIVATEHEADERS)

     makefile_fragments_seq =     $(CONDUIT_NAME)-seq.mak
     makefile_fragments_par =     $(CONDUIT_NAME)-par.mak 
# makefile_fragments_parsync = $(CONDUIT_NAME)-parsync.mak
makefile_fragments = $(makefile_fragments_seq) $(makefile_fragments_par) $(makefile_fragments_parsync)

     pkgconfig_files_seq =     gasnet-$(CONDUIT_NAME)-seq.pc
     pkgconfig_files_par =     gasnet-$(CONDUIT_NAME)-par.pc 
# pkgconfig_files_parsync = gasnet-$(CONDUIT_NAME)-parsync.pc
pkgconfig_files = $(pkgconfig_files_seq) $(pkgconfig_files_par) $(pkgconfig_files_parsync)

GASNET_SEGMENT_STR="FAST"
#GASNET_SEGMENT_STR="LARGE"
#GASNET_SEGMENT_STR="EVERYTHING"

include $(top_builddir)/other/Makefile-libgasnet.mak

EXTRA_DIST = $(CONDUIT_FILELIST) README

#docdir = $(datadir)/doc/GASNet

# convenience aliases
seq par parsync: force
	@$(MAKE) gasnet-$(CONDUIT_NAME)-$@.pc libgasnet-$(CONDUIT_NAME)-$@.a
	@for dir in $(SUBDIRS) ; do 		\
	  if test "$$dir" != "." ; then 	\
	    ( cd $$dir && $(MAKE) all ) || exit 1 ; 	\
	  fi ;					\
	 done

# test targets 

tests: force
	@echo You must specify one of tests-seq, tests-par or tests-parsync
	@exit 1

test: tests

TESTINSTANCE=$${PPID:-xxxx}
TESTLOG=`pwd`/.test-results
TESTLOG_INHERIT=0

do-begin-tests do-end-tests:
	@if test "$(TESTLOG_INHERIT)" = "0"; then \
          ( cd "$(top_builddir)" && $(MAKE) TESTINSTANCE=$(TESTINSTANCE) TESTLOG=$(TESTLOG) $@ ) || exit $$? ; \
        fi

tests-seq: force libgasnet-$(CONDUIT_NAME)-seq.a $(CONDUIT_NAME)-seq.mak $(CONDUIT_SEQ_HOOK)
	@echo "*** Building all GASNet tests in SEQ/$(GASNET_SEGMENT_STR) mode ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile tests-seq \
	  configfile="$(CONDUIT_NAME)-seq.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-parsync: force libgasnet-$(CONDUIT_NAME)-parsync.a $(CONDUIT_NAME)-parsync.mak $(CONDUIT_PARSYNC_HOOK)
	@echo "*** Building all GASNet tests in PARSYNC/$(GASNET_SEGMENT_STR) mode ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile tests-parsync \
	  configfile="$(CONDUIT_NAME)-parsync.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-par: force libgasnet-$(CONDUIT_NAME)-par.a $(CONDUIT_NAME)-par.mak $(CONDUIT_PAR_HOOK)
	@echo "*** Building all GASNet tests in PAR/$(GASNET_SEGMENT_STR) mode ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile tests-par \
	  configfile="$(CONDUIT_NAME)-par.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

do-check-install: force
	@if test -r $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$(CONDUIT_NAME)-seq.mak && \
            test -r $(DESTDIR)$(libdir)/libgasnet-$(CONDUIT_NAME)-seq.a ; then :; else \
            echo "ERROR: tests-installed-* targets require first running a top-level 'make install', but there does not appear to be a valid install for $(CONDUIT_NAME)-conduit in prefix=$(prefix)" ; \
	    exit 1 ; \
	 fi

tests-installed-seq: do-check-install
	@echo "*** Building all GASNet tests in SEQ/$(GASNET_SEGMENT_STR) mode against install in $(DESTDIR)$(libdir) ***"
	$(MAKE) -f $(top_builddir)/tests/Makefile tests-seq \
	  configfile="$(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$(CONDUIT_NAME)-seq.mak" \
	  tools_fragment="$(DESTDIR)$(includedir)/gasnet_tools-seq.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-installed-par: do-check-install
	@echo "*** Building all GASNet tests in PAR/$(GASNET_SEGMENT_STR) mode against install in $(DESTDIR)$(libdir) ***"
	$(MAKE) -f $(top_builddir)/tests/Makefile tests-par \
	  configfile="$(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$(CONDUIT_NAME)-par.mak" \
	  tools_fragment="$(DESTDIR)$(includedir)/gasnet_tools-par.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-installed-parsync: do-check-install
	@echo "*** Building all GASNet tests in parsync/$(GASNET_SEGMENT_STR) mode against install in $(DESTDIR)$(libdir) ***"
	$(MAKE) -f $(top_builddir)/tests/Makefile tests-parsync \
	  configfile="$(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$(CONDUIT_NAME)-parsync.mak" \
	  tools_fragment="$(DESTDIR)$(includedir)/gasnet_tools-parsync.mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-mpi: force 
	@echo "*** Building all MPI tests ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile tests-mpi \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

tests-mpi2: force 
	@echo "*** Building all MPI2 tests ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile tests-mpi2 \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

testtools testtoolscxx: force 
	@echo "*** Building $@ ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile $@   \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

testtool%-seq: force
	@$(MAKE) `basename $@ -seq` tools_fragment=/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet/gasnet_tools-seq.mak

testtool%-par: force
	@$(PTHREADS_ERROR_CHECK)
	@$(MAKE) `basename $@ -par` tools_fragment=/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet/gasnet_tools-par.mak

test%: $(top_srcdir)/tests/test%.c force 
	@$(MAKE) libgasnet-$(CONDUIT_NAME)-$(THREAD_MODEL_LC).a $(CONDUIT_NAME)-$(THREAD_MODEL_LC).mak
	@echo "*** Building $@ in $(THREAD_MODEL)/$(GASNET_SEGMENT_STR) mode ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile $@   \
	  configfile="$(CONDUIT_NAME)-$(THREAD_MODEL_LC).mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

test%: $(top_srcdir)/tests/test%.cc force 
	@$(MAKE) libgasnet-$(CONDUIT_NAME)-$(THREAD_MODEL_LC).a $(CONDUIT_NAME)-$(THREAD_MODEL_LC).mak
	@echo "*** Building $@ in $(THREAD_MODEL)/$(GASNET_SEGMENT_STR) mode ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile $@   \
	  configfile="$(CONDUIT_NAME)-$(THREAD_MODEL_LC).mak" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

test%: $(top_srcdir)/tests/mpi/test%.c force 
	@echo "*** Building MPI test $@ ***"
	@$(MAKE) -f $(top_builddir)/tests/Makefile $@   \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

test%-seq: force 
	@$(MAKE) `basename $@ -seq` THREAD_MODEL=SEQ
test%-parsync: force 
	@$(MAKE) `basename $@ -parsync` THREAD_MODEL=PARSYNC
test%-par: force 
	@$(MAKE) `basename $@ -par` THREAD_MODEL=PAR

tests-clean: 
	@$(MAKE) -f $(top_builddir)/tests/Makefile clean \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS)

check-run-tests-deps: force
	@if test "$(SUBDIRS)" != "" ; then 	\
	   for dir in "$(SUBDIRS)" ; do 	\
	     if test "$$dir" != "."; then       \
	   	( cd "$$dir" && $(MAKE) ) || exit $$?;	\
             fi ;                               \
	   done ;				\
	 fi

run-tests do-run-testexit: check-run-tests-deps
	@$(MAKE) TESTINSTANCE=$(TESTINSTANCE) TESTLOG=$(TESTLOG) do-begin-tests
	@$(MAKE) -f $(top_builddir)/tests/Makefile $@ \
	  TESTINSTANCE=$(TESTINSTANCE) TESTLOG=$(TESTLOG) \
	  CONDUIT_NAME=$(CONDUIT_NAME) \
	  CONDUIT_RUNCMD="$(CONDUIT_RUNCMD)" \
	  CONDUIT_TESTS="$(CONDUIT_TESTS)" $(CONDUIT_TEST_MAKEARGS) ; \
	 $(MAKE) TESTINSTANCE=$(TESTINSTANCE) TESTLOG=$(TESTLOG) do-end-tests

RUN_TESTS_TARGETS= \
   run-tests-seq run-tests-par run-tests-parsync \
   run-tests-installed-seq run-tests-installed-par run-tests-installed-parsync \
   run-tests-mpi run-tests-mpi2
# these are separate make commands to ensure they can be composed 
# and still get multiple run-tests invocations
$(RUN_TESTS_TARGETS): tests-clean
	@$(MAKE) `echo "$@" | sed 's/^run-//'` run-tests

.PRECIOUS: run-testexit
run-testexit: force
	@if test ! -x testexit ; then \
	  $(MAKE) testexit-par || exit $$? ; \
	fi
	@$(MAKE) do-run-testexit	

tests-check:
	@echo "checking for diagnostic export..."
	@symbols=`/usr/bin/nm testgasnet 2>&1 | grep gasneti_run_diagnostics`; \
   if test "$$symbols" ; then                                      \
       echo "$$symbols" ;                                          \
       echo FAILED ;                                               \
       failed=1 ;                                                  \
   else                                                            \
       echo PASSED ;                                               \
   fi ;                                                            \
   exit $$failed

#tests-check:
#	@echo "diagnostic export test SKIPPED"

     check_test_target = tests-seq
# check_test_target = tests-parsync
     check_test_target = tests-par
check: force
	@CHECK_TEST_TARGET_OVERRIDE=$(CHECK_TEST_TARGET_OVERRIDE) ; \
	$(MAKE) check-pkgconfig CHECK_FILES="$(pkgconfig_files)" ; \
	$(MAKE) check-exports $${CHECK_TEST_TARGET_OVERRIDE:-$(check_test_target)} tests-check

force:

do-clean-local: tests-clean
	@echo rm -f $(makefile_fragments) $(pkgconfig_files) $(libgasnet_objects) $(libraries) *.o core .test-results
	@rm -f $(makefile_fragments) $(pkgconfig_files) $(libgasnet_objects) $(libraries) *.o core .test-results
	@rm -Rf .SEQ .PAR .PARSYNC


do-install-data-local: force $(headers) $(makefile_fragments)
	$(mkinstalldirs) $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit
	@list='$(headers)'; privlist='$(private_headers)'; \
         for p in $$list; do \
	  if test -f $$p; then \
	    filename=`basename $$p`; \
            for q in $$privlist; do \
              if test "$$filename" = "$$q"; then filename=something_internal.h; break; fi; \
            done; \
            if test "$$filename" = "`basename $$p _internal.h`" ; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$$filename"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$$filename || exit $$? ; \
	    else :; fi; \
	  else :; fi; \
	done
	$(mkinstalldirs) $(DESTDIR)$(libdir)/pkgconfig
	@list='$(makefile_fragments)'; for p in $$list; do \
	  if test -f $$p; then \
	    filename=`basename $$p`; \
	    destmak="$(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/$$filename"; \
	    echo sed -e '/###NOINSTALL###/d' -e 's@###INSTALL_INCLUDE###@$(includedir)@g' -e 's@###INSTALL_LIBEXEC###@$(libexecdir)@g' -e 's@###INSTALL_DOC###@$(docdir)@g' -e 's@###INSTALL_LIB###@$(libdir)@g' -e 's@###INSTALL_BIN###@$(bindir)@g' -e 's@###INSTALL_MAN###@$(mandir)@g' -e 's@###INSTALL_ETC###@$(sysconfdir)@g' -e 's@$(prefix)/@$$(GASNET_PREFIX)/@g' -e 's@###INSTALL_PREFIX###@$(prefix)@g' " < $$p > $$destmak"; \
	    sed -e '/###NOINSTALL###/d' -e 's@###INSTALL_INCLUDE###@$(includedir)@g' -e 's@###INSTALL_LIBEXEC###@$(libexecdir)@g' -e 's@###INSTALL_DOC###@$(docdir)@g' -e 's@###INSTALL_LIB###@$(libdir)@g' -e 's@###INSTALL_BIN###@$(bindir)@g' -e 's@###INSTALL_MAN###@$(mandir)@g' -e 's@###INSTALL_ETC###@$(sysconfdir)@g' -e 's@$(prefix)/@$$(GASNET_PREFIX)/@g' -e 's@###INSTALL_PREFIX###@$(prefix)@g' < $$p > $$destmak || exit $$? ; \
	    thread_model=`echo "$$filename" | sed 's/^.*-\(.*\)\.mak$$/\1/'` ; \
	    destpc="$(DESTDIR)$(libdir)/pkgconfig/gasnet-$(CONDUIT_NAME)-$$thread_model.pc"; \
	    $(MAKE) do-pkgconfig-conduit thread_model=$$thread_model pkgconfig_file="$$destpc" FRAGMENT="$$destmak" || exit $$? ; \
	    chmod 644 "$$destpc" || exit $$? ; \
	  else :; fi; \
	done
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(srcdir)/README $(DESTDIR)$(docdir)/README-$(CONDUIT_NAME)
	if test -f $(srcdir)/license.txt ; then \
	  $(INSTALL_DATA) $(srcdir)/license.txt $(DESTDIR)$(docdir)/license-$(CONDUIT_NAME).txt || exit $$? ; \
        fi
	if test "$(conduit_has_install_data_hook)" = yes; then \
	  $(MAKE) do-install-data-conduit || exit $$? ; \
	fi

do-uninstall-local:
	rm -f $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/*.h $(DESTDIR)$(includedir)/$(CONDUIT_NAME)-conduit/*.mak
	rm -f $(DESTDIR)$(libdir)/pkgconfig/gasnet-$(CONDUIT_NAME)-*.pc
	rm -Rf $(DESTDIR)$(docdir)
	if test "$(conduit_has_uninstall_hook)" = yes; then \
	  $(MAKE) do-uninstall-conduit || exit $$? ; \
	fi

do-error: 
	@echo "ERROR: $(CONDUIT_NAME)-conduit support was not detected at configure time"
	@echo "       try re-running configure with --enable-$(CONDUIT_NAME)"
	@exit 1

$(top_builddir)/other/Makefile-conduit.mak: $(top_srcdir)/other/Makefile-conduit.mak.in
	cd $(top_builddir)/other && $(MAKE) Makefile-conduit.mak

$(top_builddir)/other/fragment-body.mak: $(top_srcdir)/other/fragment-body.mak.in
	cd $(top_builddir)/other && $(MAKE) fragment-body.mak

$(top_builddir)/other/fragment-head.mak: $(top_srcdir)/other/fragment-head.mak.in
	cd $(top_builddir)/other && $(MAKE) fragment-head.mak

$(top_builddir)/tests/Makefile: $(top_srcdir)/tests/Makefile.in
	cd $(top_builddir)/tests && $(MAKE) Makefile

Makefile: $(srcdir)/Makefile.in 			\
          $(top_builddir)/other/Makefile-conduit.mak 	\
          $(top_builddir)/other/Makefile-libgasnet.mak 	\
          $(top_builddir)/tests/Makefile
	cd $(top_builddir) && CONFIG_FILES=$(CONDUIT_NAME)-conduit/$@ CONFIG_HEADERS= ./config.status

make_fragment_deps = conduit.mak $(top_builddir)/other/fragment-head.mak $(top_builddir)/other/fragment-body.mak

$(CONDUIT_NAME)-seq.mak: $(make_fragment_deps)
	$(MAKE) do-make-fragment thread_model=seq THREAD_MODEL=SEQ

$(CONDUIT_NAME)-par.mak: $(make_fragment_deps)
	@$(PTHREADS_ERROR_CHECK)
	$(MAKE) do-make-fragment thread_model=par THREAD_MODEL=PAR

$(CONDUIT_NAME)-parsync.mak: $(make_fragment_deps)
	@$(PTHREADS_ERROR_CHECK)
	$(MAKE) do-make-fragment thread_model=parsync THREAD_MODEL=PARSYNC

do-make-fragment: force
	rm -f $(CONDUIT_NAME)-$(thread_model).mak
	@echo Building $(CONDUIT_NAME)-$(thread_model).mak... ;                             \
	AUTOGENMSG='WARNING: This file is automatically generated - do NOT edit directly' ; \
	cat $(top_builddir)/other/fragment-head.mak                                         \
	    conduit.mak                                                                     \
	    $(top_builddir)/other/fragment-body.mak |                                       \
	sed -e 's@#conduit_name#@$(CONDUIT_NAME)@g'                                         \
	    -e 's@#THREAD_MODEL#@$(THREAD_MODEL)@g'                                         \
	    -e 's@#thread_model#@$(thread_model)@g'                                         \
	    -e '/#INSTRUCTIONS#/d'                                                          \
	    -e "s@#AUTOGEN#@$${AUTOGENMSG}@g"                                               \
	> $(CONDUIT_NAME)-$(thread_model).mak || exit 1

pkgconfig_conduit = $(top_srcdir)/other/pkgconfig-conduit.pc

gasnet-$(CONDUIT_NAME)-seq.pc: $(CONDUIT_NAME)-seq.mak $(pkgconfig_conduit)
	@$(MAKE) do-pkgconfig-conduit thread_model=seq pkgconfig_file="$@" FRAGMENT="$<"

gasnet-$(CONDUIT_NAME)-par.pc: $(CONDUIT_NAME)-par.mak $(pkgconfig_conduit)
	@$(PTHREADS_ERROR_CHECK)
	@$(MAKE) do-pkgconfig-conduit thread_model=par pkgconfig_file="$@" FRAGMENT="$<"

gasnet-$(CONDUIT_NAME)-parsync.pc: $(CONDUIT_NAME)-parsync.mak $(pkgconfig_conduit)
	@$(PTHREADS_ERROR_CHECK)
	@$(MAKE) do-pkgconfig-conduit thread_model=parsync pkgconfig_file="$@" FRAGMENT="$<"

do-pkgconfig-conduit: force
	rm -f $(pkgconfig_file)
	@echo Building $(pkgconfig_file) from $$FRAGMENT...
	@echo '# WARNING: This file is automatically generated - do NOT edit directly' > $(pkgconfig_file)
	@echo '# Copyright 2017, The Regents of the University of California' >> $(pkgconfig_file)
	@echo '# Terms of use are as specified in license.txt' >> $(pkgconfig_file)
	@echo '# See the GASNet README for instructions on using these variables' >> $(pkgconfig_file)
	@VARS="GASNET_CC GASNET_OPT_CFLAGS GASNET_MISC_CFLAGS \
               GASNET_MISC_CPPFLAGS GASNET_DEFINES GASNET_INCLUDES \
               GASNET_CXX GASNET_OPT_CXXFLAGS GASNET_MISC_CXXFLAGS GASNET_MISC_CXXCPPFLAGS \
               GASNET_LD GASNET_LDFLAGS GASNET_LIBS" ; \
         $(MAKE) --no-print-directory -f $(top_srcdir)/other/Makefile-echovar.mak VARS="$$VARS" echovars \
	           >> $(pkgconfig_file)
	@cat $(pkgconfig_conduit) | \
	sed -e 's@#conduit_name#@$(CONDUIT_NAME)@g'  \
	    -e 's@#thread_model#@$(thread_model)@g'  \
	    -e 's@#version#@$(VERSION)@g'            \
	>> $(pkgconfig_file)


