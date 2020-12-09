# CTestTop.mak.  Generated from CTestTop.mak.in by configure.
#
# Template for common makefile definitions for all C tests in the 'test' tree.
#
# Autoconf will turn this file into CTestTop.mak, which you should include at
# the top of all Makefiles for tests.
# 

ifneq (1,1)
	GNU Make is required, but you're not using it: try 'gmake'
endif

SHELL = /bin/bash

# upcr.mak want to see these: use upper case versions (full paths instead of relative)
top_srcdir = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0
top_builddir = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/dbg
prefix = @INSTALL_PREFIX@

PARSEQ=seq
CONDUIT=mpi
include $(top_builddir)/upcr.mak

UPCR_CPPFLAGS += -D_IN_UPCR_GLOBFILES_C=1

# default to dynamic threads, unless THREADS=N set by user
ifeq ($(THREADS),)
  UPCR_CPPFLAGS += -D__UPC_DYNAMIC_THREADS__=1
else
  UPCR_CPPFLAGS += -D__UPC_STATIC_THREADS__=1 -DTHREADS=$(THREADS)
endif

ifneq ($(PTHREADS),)
  UPCR_CPPFLAGS += -D__BERKELEY_UPC_PTHREADS__=1 -DUPCRI_USING_TLD -I$(top_srcdir)/test
endif

# default name for executable: overridable by user
TARGET_APP = a.out


# vim: set ft=automake: