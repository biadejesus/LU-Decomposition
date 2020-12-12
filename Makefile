GCC=gcc
UPC = upcc
FLAGS = -network=smp
TARGETL=LUlarge.upc
TARGETM=LUmedium.upc
TARGETS=LUsmall.upc
EXEL=lularge
EXEM=lumedium
EXES=lusmall

all: 

	$(UPC) $(TARGETL) $(FLAGS) -o $(EXEL)
	$(UPC) $(TARGETM) $(FLAGS) -o $(EXEM)
	$(UPC) $(TARGETS) $(FLAGS) -o $(EXES)
	

