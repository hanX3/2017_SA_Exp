out1 = cut

sourcefile1 = main.cpp Cut.h Cut.cpp set.h

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lSpectrum

CFLAGS = -g -Wall -std=c++11 -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++

all:$(out1) $(out2)

$(out1): $(sourcefile1)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cpp ,$(sourcefile1)) $(ROOTLIBS)

install:
	cp $(out1) ../
	cp * ./.backup
clean:
	rm -f *~ *.o $(out1)
