## Process this file with automake to produce Makefile.in       -*- makefile -*-

AUTOMAKE_OPTIONS  = foreign 1.4 no-dependencies

SUBDIRS = sys bits

built_headers = 	\
	ctype.h.in	\
	byteswap.h.in	\
	alloca.h.in	\
	fenv.h.in	\
	inttypes.h.in	\
	iso646.h.in	\
	limits.h.in	\
	math.h.in	\
	time.h.in	\
	stdarg.h.in	\
	stdbool.h.in	\
	stddef.h.in	\
	stdint.h.in 	\
	stdlib.h.in	\
	stdio.h.in	\
	string.h.in

all_headers = $(built_headers:.h.in=.h)
EXTRA_DIST = README $(built_headers)
DISTCLEANFILES = $(all_headers)

all: $(all_headers)

builddir = $(top_builddir)/$(subdir)

if USING_CUPC2C
# automake 1.4-p6 doesn't like anything to exist only conditionally
install-data-local: inst-bupc-atomics
uninstall-local:
else
install-data-local: $(all_headers) inst-bupc-atomics
	$(mkinstalldirs) $(DESTDIR)$(includedir)/$(subdir)
	@list='$(all_headers)'; for q in $$list; do \
	  if test -f $(builddir)/$$q; then \
	    filename=`basename $$q`; \
	    echo " $(INSTALL_DATA) $(builddir)/$$q $(DESTDIR)$(includedir)/$(subdir)/$$filename"; \
	    $(INSTALL_DATA) $(builddir)/$$q $(DESTDIR)$(includedir)/$(subdir)/$$filename; \
	  else :; fi; \
	done

uninstall-local:
	rm -f $(DESTDIR)$(includedir)/$(subdir)/*.h
endif

.PHONY: force inst-bupc-atomics
