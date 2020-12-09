# CTestBottom.mak.  Generated from CTestBottom.mak.in by configure.
#
# Bottom half of common makefile definitions for C tests
#

# merge default from CTestTop.mak with any user additions
USE_INCLUDES = $(UPCR_INCLUDES) $(INCLUDES)
USE_CPPFLAGS = $(UPCR_CPPFLAGS) $(CPPFLAGS)
USE_CFLAGS = $(UPCR_CFLAGS) $(CFLAGS) -I..
USE_LDFLAGS = $(UPCR_LDFLAGS) $(LDFLAGS)
USE_LIBS = $(UPCR_LDFLAGS) $(LIBS) $(UPCR_LIBS)

COMPILE = $(UPCR_CC) $(USE_CPPFLAGS) $(USE_CFLAGS) $(USE_INCLUDES)
LINK = $(UPCR_LD) $(USE_LDFLAGS)



.SUFFIXES:
.SUFFIXES: .c .o
.c.o:
	$(COMPILE) -c $<

help: 
	@echo Usage: 'make CONDUIT=<conduit> PARSEQ=<par|seq> build'

build: clean $(TARGET_APP)

$(TARGET_APP): $(OBJS)
	$(LINK) -o $@ $(OBJS) $(USE_LIBS)

foo:
	echo UPCR_CC=$(UPCR_CC)
clean:
	-rm -rf $(TARGET_APP) $(OBJS) core
