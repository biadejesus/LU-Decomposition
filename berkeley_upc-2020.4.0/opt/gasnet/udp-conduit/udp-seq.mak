# Description: Makefile fragment for udp-conduit, GASNET_SEQ mode
# WARNING: This file is automatically generated - do NOT edit directly
# other/fragment-head.mak.  Generated from fragment-head.mak.in by configure.
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

# ----------------------------------------------------------------------
# GASNet client usage instructions:
#
# Clients should include this file in their Makefile, using: (no leading '#')
#     include ###INSTALL_INCLUDE###/udp-conduit/udp-seq.mak
# or alternatively, just:
#     include udp-seq.mak
# and use a -I###INSTALL_INCLUDE###/udp-conduit
# command-line option when invoking make
#
# Then in the Makefile, use a compile line something like one of these:
#  $(GASNET_CC)  $(GASNET_CPPFLAGS)    $(GASNET_CFLAGS)   -c myfile.c
#  $(GASNET_CXX) $(GASNET_CXXCPPFLAGS) $(GASNET_CXXFLAGS) -c myfile.cxx
#
# and a link line something like this:
#  $(GASNET_LD) $(GASNET_LDFLAGS) -o myfile myfile.o $(GASNET_LIBS)
# 
# For GASNet clients who need finer-grained control over compiler options,
#  GASNET_CFLAGS is an alias for: 
#     $(GASNET_OPT_CFLAGS) $(GASNET_MISC_CFLAGS)
#  GASNET_CPPFLAGS is an alias for: 
#     $(GASNET_MISC_CPPFLAGS) $(GASNET_DEFINES) $(GASNET_INCLUDES)
#  GASNET_CXXFLAGS is an alias for:
#     $(GASNET_OPT_CXXFLAGS) $(GASNET_MISC_CXXFLAGS)
#  GASNET_CXXCPPFLAGS is an alias for:
#     $(GASNET_MISC_CXXCPPFLAGS) $(GASNET_DEFINES) $(GASNET_INCLUDES)
# and those finer-grained variables can be used rather than the convenience aliases
#
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# GASNet conduit-writer instructions:
#
# Most conduit-specific tweaks for these fragments can be accomplished 
# by setting specially-named variables in your conduit.mak file:
#
# * Variables of the form CONDUIT_WHATEVER will be added to the GASNET_WHATEVER
#   variable in the fragment.
#
# * Variables of the form CONDUIT_WHATEVER_<MODE> (where <MODE> is SEQ, PAR or PARSYNC)
#   will be added to the GASNET_WHATEVER variable for the <MODE> fragment only.
#
# * Settings which are contingent on whether the fragment is being used 
#   within the build tree or from an installed copy should be controlled using
#   the standard ###NO INSTALL### marker (without the internal space)
#
# * In the rare cases where a conduit needs to completely replace the 
#   default value for a GASNET_WHATEVER variable (rather than simply adding
#   to it), this can be accomplished by setting one of the GASNET_WHATEVER_OVERRIDE
#   variables in the conduit.mak file. Note this is only supported for the 
#   overridable variables listed below, and should only be used as a last resort
#   because it clobbers the default values established by configure and
#   complicates GASNet framework maintenance.
#
# ----------------------------------------------------------------------

GASNET_PREFIX = ###INSTALL_PREFIX###

# ----------------------------------------------------------------------
# conduit-overridable variables 

GASNET_LD_OVERRIDE = /usr/bin/gcc
GASNET_LDFLAGS_OVERRIDE = -O3 --param max-inline-insns-single=35000 --param inline-unit-growth=10000 --param large-function-growth=200000  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address 

# NOTE: conduits setting GASNET_LD(FLAGS)_OVERRIDE should ALSO set corresponding
# GASNET_LD_REQUIRES_* feature flags, for consumption by clients who 
# do not or cannot directly consume GASNET_LD.

# ----------------------------------------------------------------------
# udp-conduit/conduit.mak settings

# AMUDP is C++-based, which requires us to link using the C++ compiler
GASNET_LD_OVERRIDE = /usr/bin/g++
GASNET_LDFLAGS_OVERRIDE = -O2  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address  

# Linker feature requirements embedded in GASNET_LD(FLAGS) which are not satisfied solely by GASNET_LIBS 
# (eg possible dependence on implicit MPI or C++ libraries added by a linker wrapper in GASNET_LD):
GASNET_LD_REQUIRES_CXX = 1

# hooks for using AMUDP library from within build tree ###NOINSTALL###
# (nothing additional required for installed copy)     ###NOINSTALL###
CONDUIT_INCLUDES = -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/other/amudp -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet/other/amudp ###NOINSTALL###

CONDUIT_LDFLAGS = 
CONDUIT_LIBDIRS = -L/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet/other/amudp ###NOINSTALL###

CONDUIT_LIBS = $(CONDUIT_LIBDIRS) -lamudp    

# $Source: bitbucket.org:berkeleylab/gasnet.git/other/fragment-body.mak.in $
# ----------------------------------------------------------------------
# Following section other/fragment-body.mak.  Generated from fragment-body.mak.in by configure.

# ----------------------------------------------------------------------
# Directory-based options

GASNET_INCLUDES = -I###INSTALL_INCLUDE### -I###INSTALL_INCLUDE###/udp-conduit $(CONDUIT_INCLUDES) $(CONDUIT_INCLUDES_SEQ)
GASNET_LIBDIRS = -L###INSTALL_LIB###

# Textual lines containing the string "###NOINSTALL###" are removed by the install process
# (must be one continuous line) ###NOINSTALL###
GASNET_INCLUDES = -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/udp-conduit -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/other $(CONDUIT_INCLUDES) $(CONDUIT_INCLUDES_SEQ) -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/extended-ref/vis -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/extended-ref/coll -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/extended-ref/ratomic -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/gasnet/extended-ref -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet  ###NOINSTALL###
GASNET_LIBDIRS = -L/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet/udp-conduit ###NOINSTALL###

# ----------------------------------------------------------------------
# C compiler and options

GASNET_CC = /usr/bin/gcc

GASNET_OPT_CFLAGS = -O3 --param max-inline-insns-single=35000 --param inline-unit-growth=10000 --param large-function-growth=200000 $(CONDUIT_OPT_CFLAGS) $(CONDUIT_OPT_CFLAGS_SEQ)
GASNET_MISC_CFLAGS =  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address $(CONDUIT_MISC_CFLAGS) $(CONDUIT_MISC_CFLAGS_SEQ)
GASNET_MISC_CPPFLAGS =  $(CONDUIT_MISC_CPPFLAGS) $(CONDUIT_MISC_CPPFLAGS_SEQ)
GASNET_DEVWARN_CFLAGS =  -Winline -Wall -Wpointer-arith -Wnested-externs -Wwrite-strings -Wmissing-format-attribute -Winit-self -Wvla -Wexpansion-to-defined -Woverlength-strings -Wclobbered -Wcast-function-type -Wempty-body -Wignored-qualifiers -Wimplicit-fallthrough -Wmissing-parameter-type -Wold-style-declaration -Wuninitialized -Wshift-negative-value -Wno-format-overflow -Wno-format-truncation $(CONDUIT_DEVWARN_CFLAGS) $(CONDUIT_DEVWARN_CFLAGSSEQ)

# ----------------------------------------------------------------------
# C++ compiler and options
# TODO: some options (especially CONDUIT_*) are not distinct from C compiler

GASNET_CXX = /usr/bin/g++

GASNET_OPT_CXXFLAGS = -O2 $(CONDUIT_OPT_CFLAGS) $(CONDUIT_OPT_CFLAGS_SEQ)
GASNET_MISC_CXXFLAGS =  -Wno-unused -Wunused-result -Wno-unused-parameter -Wno-address $(CONDUIT_MISC_CFLAGS) $(CONDUIT_MISC_CFLAGS_SEQ)
GASNET_MISC_CXXCPPFLAGS =  $(CONDUIT_MISC_CPPFLAGS) $(CONDUIT_MISC_CPPFLAGS_SEQ)
GASNET_DEVWARN_CXXFLAGS =  -Wall -Wpointer-arith -Wwrite-strings -Wmissing-format-attribute -Winit-self -Wvla -Wexpansion-to-defined -Woverlength-strings -Wclobbered -Wcast-function-type -Wempty-body -Wignored-qualifiers -Wimplicit-fallthrough -Wuninitialized -Wshift-negative-value -Wno-format-overflow -Wno-format-truncation $(CONDUIT_DEVWARN_CXXFLAGS) $(CONDUIT_DEVWARN_CXXFLAGSSEQ)

# ----------------------------------------------------------------------
# Common defines

GASNET_EXTRADEFINES_SEQ = 
GASNET_EXTRADEFINES_PAR = -D_REENTRANT
GASNET_EXTRADEFINES_PARSYNC = -D_REENTRANT

GASNET_DEFINES = -D_GNU_SOURCE=1 -DGASNET_SEQ $(GASNET_EXTRADEFINES_SEQ) $(CONDUIT_DEFINES) $(CONDUIT_DEFINES_SEQ) $(MANUAL_DEFINES)

# ----------------------------------------------------------------------
# Documented compilation convenience aliases

GASNET_CFLAGS = $(GASNET_OPT_CFLAGS) $(GASNET_MISC_CFLAGS) $(MANUAL_CFLAGS)
GASNET_CPPFLAGS = $(GASNET_MISC_CPPFLAGS) $(GASNET_DEFINES) $(GASNET_INCLUDES)

GASNET_CXXFLAGS = $(GASNET_OPT_CXXFLAGS) $(GASNET_MISC_CXXFLAGS) $(MANUAL_CXXFLAGS)
GASNET_CXXCPPFLAGS = $(GASNET_MISC_CXXCPPFLAGS) $(GASNET_DEFINES) $(GASNET_INCLUDES)

# ----------------------------------------------------------------------
# linker and options

GASNET_LD = $(GASNET_LD_OVERRIDE)

# linker flags that GASNet clients should use 
GASNET_LDFLAGS = $(GASNET_LDFLAGS_OVERRIDE)  $(CONDUIT_LDFLAGS) $(CONDUIT_LDFLAGS_SEQ) $(MANUAL_LDFLAGS)

GASNET_EXTRALIBS_SEQ = 
GASNET_EXTRALIBS_PAR = -lpthread
GASNET_EXTRALIBS_PARSYNC = -lpthread

# libraries that GASNet clients should append to link line
GASNET_LIBS =                             \
    $(GASNET_LIBDIRS)                     \
    -lgasnet-udp-seq \
    $(CONDUIT_LIBS)                       \
    $(CONDUIT_LIBS_SEQ)        \
    $(GASNET_EXTRALIBS_SEQ)    \
    -lrt                    \
    -L/usr/lib/gcc/x86_64-linux-gnu/9 -lgcc                              \
                                    \
    -lm                                \
    $(MANUAL_LIBS)

# ----------------------------------------------------------------------
