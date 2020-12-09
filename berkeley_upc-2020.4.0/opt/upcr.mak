

GASNET_MAK_DIR=$(upcr_include)
UPCR_MAK_DIR=$(upcr_include)
###NOINSTALL### These are explained at the bottom of this file
GASNET_MAK_DIR=/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/gasnet###NOINSTALL###
UPCR_MAK_DIR=/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0###NOINSTALL###

# Include proper GASNet .mak fragment for our network/thread model/debug 
# settings.  
ifeq ($(strip $(UPCR_PARSEQ)),)
    PARSEQ  = seq
endif

# Pthreads settings
# 1) 'seq' means single threaded only
# 2) 'par' uses pthreads for UPC threads (both UPCRI_SUPPORT_PTHREADS 
#     and UPCRI_UPC_PTHREADS defined)
# 3) 'thr' one UPC thread per process, but thread safe (UPCRI_SUPPORT_PTHREADS is
#     defined but not UPCRI_UPC_PTHREADS)
ifeq ($(strip $(UPCR_PARSEQ)),seq)
    UPCR_EXTRA_CPPFLAGS=
    GASNET_PARSEQ=seq
endif
ifeq ($(strip $(UPCR_PARSEQ)),par)
    UPCR_EXTRA_CPPFLAGS=-DUPCRI_SUPPORT_PTHREADS=1 -DUPCRI_UPC_PTHREADS=1
    GASNET_PARSEQ=par
endif
ifeq ($(strip $(UPCR_PARSEQ)),thr)
    UPCR_EXTRA_CPPFLAGS=-DUPCRI_SUPPORT_PTHREADS=1
    GASNET_PARSEQ=par
endif

# don't do include if UPCR_CONDUIT not set--breaks 'make distclean'
ifneq ($(strip $(UPCR_CONDUIT)),)
    gasnet_makfile = $(GASNET_MAK_DIR)/$(UPCR_CONDUIT)-conduit/$(UPCR_CONDUIT)-$(GASNET_PARSEQ).mak
    -include $(gasnet_makfile) 
    GASNET_PREFIX=$(UPCR_HOME)
endif

###NOINSTALL### Overrides for these three when run in build dir appear at bottom of this file.
UPCR_INCLUDES = -I$(upcr_include)
UPCR_LIBDIRS  = -L$(upcr_lib)
UPCR_EXTERN_INCLUDEDIR = $(upcr_include)

# merge GASNet make settings with runtime's, and with any upcc-specified
# EXTRA_CFLAGS, EXTRA_CPPFLAGS, and/or EXTRA_LDFLAGS 

UPCR_CC         = $(GASNET_CC)
UPCR_CPPFLAGS_GENERIC   = $(EXTRA_CPPFLAGS) $(GASNET_DEFINES) $(UPCC_GENINCLUDE_CPPFLAGS) $(GASNET_INCLUDES) $(UPCR_INCLUDES) $(UPCR_EXTRA_CPPFLAGS)
UPCR_CPPFLAGS   = $(UPCR_CPPFLAGS_GENERIC) $(GASNET_MISC_CPPFLAGS)
UPCR_CFLAGS     = $(GASNET_CFLAGS) -fno-strict-aliasing -fcommon $(EXTRA_CFLAGS)


ifeq ($(UPCC_USER_LD),)
    UPCR_LD         = $(GASNET_LD)
    UPCR_LDFLAGS    = $(GASNET_LDFLAGS) $(EXTRA_LDFLAGS) $(UPCR_LIBDIRS)
else
    UPCR_LD         = $(UPCC_USER_LD)
    UPCR_LDFLAGS    = $(EXTRA_LDFLAGS) $(UPCR_LIBDIRS)
endif
UPCR_LIBS       = -lupcr-$(UPCR_CONDUIT)-$(UPCR_PARSEQ) -lumalloc $(GASNET_LIBS) $(EXTRA_LIBS)

###NOINSTALL### Nutty workaround to support building with both a build and install trees:
###NOINSTALL### second value overrides first during build, but is filtered out with a grep
###NOINSTALL### during the install process.
###NOINSTALL### No spaces between end of macro and '###', else space added to value
upcr_include  = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0###NOINSTALL###
upcr_lib      = /home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt###NOINSTALL###
UPCR_INCLUDES += -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt###NOINSTALL###
UPCR_LIBDIRS  += -L/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/opt/umalloc###NOINSTALL###
UPCR_CPPFLAGS_LIB = $(UPCR_CPPFLAGS) -I/home/beatrizdejesus/Documents/UEM/PC/projeto3/LU-Decomposition/berkeley_upc-2020.4.0/umalloc###NOINSTALL### used only to build libupcr-*
###NOINSTALL###
