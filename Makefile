GCC=gcc
UPC = upcc
FLAGS = -network=smp
TARGETL=codigos/LUlarge.upc
TARGETM=codigos/LUmedium.upc
TARGETS=codigos/LUsmall.upc
EXEL=lularge
EXEM=lumedium
EXES=lusmall

all: 

	$(UPC) $(TARGETL) $(FLAGS) -o $(EXEL)
	mv $(EXEL) codigos
	$(UPC) $(TARGETM) $(FLAGS) -o $(EXEM)
	mv $(EXEM) codigos
	$(UPC) $(TARGETS) $(FLAGS) -o $(EXES)
	mv $(EXES) codigos
	

