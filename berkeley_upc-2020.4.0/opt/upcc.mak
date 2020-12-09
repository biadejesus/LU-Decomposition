# upcc.mak.  Generated from upcc.mak.in by configure.
#
#

SHELL = /bin/bash

# upcr.mak want to see these: use upper case versions (full paths instead of relative)
top_srcdir = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0
top_builddir = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt

# Installed locations:
# UPCR_HOME is set by upcc.pl
upcr_include  = $(UPCR_HOME)/include
upcr_include2 = $(UPCR_HOME)/include
upcr_libexec  = $(UPCR_HOME)/libexec
upcr_lib      = $(UPCR_HOME)/lib

# Include makefile that'll sort out which gasnet makefile settings to use
upcrmak_dir=$(UPCR_HOME)/include
upcrmak_dir=$(top_builddir)###NOINSTALL###
include $(upcrmak_dir)/upcr.mak

# this perl goop encodes any unfriendly characters in the CC/LD commands 
# that might otherwise break the shell or preprocessor. upcrun -i decodes them
UPCC_ENCODE = /usr/bin/perl -pe 's/^\s*//g; s/\s*$$//g; s+([^\.A-Za-z0-9_/-])+sprintf("@%02X",ord($$1))+eg'

POSTTRANS_CPPFLAGS = -D__BERKELEY_UPC_SECOND_PREPROCESS__=1 \
  -DUPCRI_CC="$(shell echo '$(UPCR_CC)' | $(UPCC_ENCODE) )" \
  -DUPCRI_LD="$(shell echo '$(UPCR_LD)' | $(UPCC_ENCODE) )" 

I_OR_ISYSTEM = I

UPCC_EXTINCLUDE_CPPFLAGS= -I$(upcr_include)/upcr_extinclude
#UPCC_GENINCLUDE_CPPFLAGS= -$(I_OR_ISYSTEM) $(upcr_include2)/upcr_geninclude2
UPCC_GENINCLUDE_CPPFLAGS= -I$(upcr_include2)/upcr_geninclude
UPCC_PRETRANS_CPPFLAGS  = -$(I_OR_ISYSTEM) $(upcr_include)/upcr_preinclude  -std=gnu99 -D_GNU_SOURCE=1 -D__NO_STRING_INLINES -D__NO_MATH_INLINES -D__NO_INLINE__ -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0
UPCC_POSTTRANS_CPPFLAGS = -I$(upcr_include)/upcr_postinclude $(POSTTRANS_CPPFLAGS)
DETECT_UPC		= $(upcr_libexec)/detect-upc
PRE_PREPROC		= $(upcr_libexec)/upcppp
UPCC_UPC_HEADER_DIRS	= -I $(upcr_include)/upcr_preinclude

UPCC_GENERIC_PRETRANS_CPPFLAGS = $(UPCR_CPPFLAGS_GENERIC) $(UPCC_PRETRANS_CPPFLAGS)

ifeq ($(USE_GCCUPC_COMPILER),yes)
    # GNU UPC Compiler
    PREPROC = $(GCCUPC) -x upc -E $(UPCR_CPPFLAGS_GENERIC) $(UPCC_EXTINCLUDE_CPPFLAGS)
    COMPILE = $(GCCUPC) -x upc $(EXTRA_CFLAGS)
    HASH_LINE = \# 
else
ifeq ($(USE_CUPC_COMPILER),yes)
    # CLANG UPC Compiler
    PREPROC = $(CUPC) -E $(UPCR_CPPFLAGS_GENERIC) $(UPCC_EXTINCLUDE_CPPFLAGS)
    COMPILE = $(CUPC) $(EXTRA_CFLAGS)
    HASH_LINE = \#
else
    # Source-to-source Translator
    #include $(upcrmak_dir)/gcc_as_cc.mak
    #PREPROC = $(GCC_AS_CC) $(UPCC_GENERIC_PRETRANS_CPPFLAGS)
    PREPROC = $(UPCR_CC) -E $(UPCR_CPPFLAGS) $(UPCC_PRETRANS_CPPFLAGS)
    COMPILE = $(UPCR_CC) $(UPCR_CFLAGS) $(UPCR_CPPFLAGS) $(UPCC_POSTTRANS_CPPFLAGS)
    HASH_LINE = \# line
endif
endif

LINK = $(UPCR_LD) 

all: link

.SUFFIXES:
.SUFFIXES: .c .o .i .inc .ipre1 .ipre2
.PHONY: force link echovar
.SECONDARY:

force: 

# Rules to run clang-based translator or preprocessor:
#PHONY: do-trans do-preproc
#do-trans: force
#	$(TRANS) $(UPCC_GENERIC_PRETRANS_CPPFLAGS) -o $(DST) $(SRC)
#do-preproc: force
#	$(PREPROC) $(UPCC_GENERIC_PRETRANS_CPPFLAGS) -E $(SRC)

%.ipre1: %.c
	$(PRE_PREPROC) -o '$*.inc' $(UPCC_UPC_HEADER_DIRS) $(UPCPPP_INCLUDEDIRS) '$<'
	$(PREPROC) '$<' >'$@'

# Perl code below performs #line preprocessing:
# - Replace the original .upc source name in place of the temporary .c file name
# - Replace any occurence of '//' in #include pathnames with '/', because some preprocessors have been 
#   observed to treat this as the beginning of a comment and consequently barf on the include directive
%.ipre2: %.ipre1
	/usr/bin/perl -p -e 'BEGIN { print qq|$(HASH_LINE) 1 "<Berkeley UPC 2020.4.0>"\n|; } ' \
          -e 'if (/^#(?:line)?\s*[0-9]+/) { s)\Q"$(PREPROCNAME)")"$(ORIGNAME)"); s@//@/@g; }' \
          '$<'  > '$@'

%.i: %.ipre2
	$(DETECT_UPC) $(DETECT_UPC_FLAGS) -i '$*.inc' -o '$@' '$<'

# for trans.c to trans.o 
%.o: %.c
	$(COMPILE) -c '$<'

# GCCUPC compiles to .o directly from .i
%.o: %.i
	$(COMPILE) -c '$<'

link: force
	$(LINK) -o '$(OUT)' $(OBJS) $(UPCR_LDFLAGS) $(UPCR_LIBS) $(GCCUPC_LASTOBJ) $(CUPC_LASTOBJ)

# echo an arbitrary make/environment variable
echovar: force
	@echo $($(VARNAME))

###NOINSTALL###
###NOINSTALL### Overrides for running from builddir:
upcr_include  = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0###NOINSTALL###     include  = distributed headers
upcr_include2 = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt###NOINSTALL###   include2 = generated headers
upcr_libexec  = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/detect-upc###NOINSTALL###
upcr_lib      = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt###NOINSTALL###
###NOINSTALL### upcr_extinclude is split, but we must avoid picking up any dirs twice (bug1713):
UPCC_EXTINCLUDE_CPPFLAGS+= -I$(upcr_include2)/upcr_extinclude###NOINSTALL###
###NOINSTALL###

# vim: set ft=automake:
