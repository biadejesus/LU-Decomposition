# Description: Makefile fragment for GASNet_tools, GASNET_#THREAD_MODEL# mode
# #AUTOGEN#
# other/gasnet_tools-fragment.mak.  Generated from gasnet_tools-fragment.mak.in by configure.
# Copyright 2011, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

# ----------------------------------------------------------------------
# Usage instructions:
#
# Clients should include this file in their Makefile, using: (no leading '#')
#     include ###INSTALL_INCLUDE###/gasnet_tools-#thread_model#.mak
# or alternatively, just:
#     include gasnet_tools-#thread_model#.mak  
# and use a -I###INSTALL_INCLUDE###
# command-line option when invoking make
#
# Then in the Makefile, use a compile line something like this:
#  $(GASNETTOOLS_CC) $(GASNETTOOLS_CPPFLAGS) $(GASNETTOOLS_CFLAGS) -c myfile.c
#
# and a link line something like this:
#  $(GASNETTOOLS_LD) $(GASNETTOOLS_LDFLAGS) -o myfile myfile.o $(GASNETTOOLS_LIBS)
# ----------------------------------------------------------------------

GASNET_PREFIX = ###INSTALL_PREFIX###

GASNETTOOLS_INCLUDES = -I###INSTALL_INCLUDE###
GASNETTOOLS_INCLUDES = -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/other -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet ###NOINSTALL###

GASNETTOOLS_LIBDIR = ###INSTALL_LIB###
GASNETTOOLS_LIBDIR = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet  ###NOINSTALL###

GASNETTOOLS_DEBUGFLAGS = -DNDEBUG

GASNETTOOLS_THREADFLAGS_PAR = -DGASNETT_THREAD_SAFE -D_REENTRANT
GASNETTOOLS_THREADFLAGS_SEQ = -DGASNETT_THREAD_SINGLE
GASNETTOOLS_THREADFLAGS = $(GASNETTOOLS_THREADFLAGS_#THREAD_MODEL#)

GASNETTOOLS_THREADLIBS_PAR = -lpthread 
GASNETTOOLS_THREADLIBS_SEQ = 
GASNETTOOLS_THREADLIBS = $(GASNETTOOLS_THREADLIBS_#THREAD_MODEL#)

GASNETTOOLS_TOOLLIB_NAME = gasnet_tools-#thread_model#

GASNETTOOLS_CC = /usr/bin/gcc
GASNETTOOLS_CPPFLAGS = $(GASNETTOOLS_DEBUGFLAGS) $(GASNETTOOLS_THREADFLAGS) -D_GNU_SOURCE=1  $(GASNETTOOLS_INCLUDES) $(MANUAL_DEFINES)
GASNETTOOLS_CFLAGS = -O3 --param max-inline-insns-single=35000 --param inline-unit-growth=10000 --param large-function-growth=200000  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address $(KEEPTMP_CFLAGS) $(MANUAL_CFLAGS)
GASNETTOOLS_LD = /usr/bin/gcc
GASNETTOOLS_LDFLAGS = -O3 --param max-inline-insns-single=35000 --param inline-unit-growth=10000 --param large-function-growth=200000  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address $(MANUAL_LDFLAGS)
GASNETTOOLS_LIBS = -L$(GASNETTOOLS_LIBDIR) -l$(GASNETTOOLS_TOOLLIB_NAME) $(GASNETTOOLS_THREADLIBS)  -lm $(MANUAL_LIBS)
GASNETTOOLS_CXX = /usr/bin/g++
GASNETTOOLS_CXXFLAGS = -O2  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address $(MANUAL_CXXFLAGS)

