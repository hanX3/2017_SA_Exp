out1 = decode
out2 = assemble

sourcefile1 = main_decode.cpp MidasData.h MidasData.cpp Decode.h Decode.cpp set.h
sourcefile2 = main_assemble.cpp Decode.h Decode.cpp Assemble.h Assemble.cpp MidasData.h MidasData.cpp set.h

ROOTCFLAGS  = $(shell root-config --cflags)
ROOTLIBS    = $(shell root-config --libs) -lSpectrum

CFLAGS = -g -Wall -std=c++11 -O2 -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64

GXX = g++

all:$(out1) $(out2)

$(out1): $(sourcefile1)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cpp ,$(sourcefile1)) $(ROOTLIBS)
$(out2): $(sourcefile2)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) -o $@ $(filter %.cpp ,$(sourcefile2)) $(ROOTLIBS)

install:
	cp $(out1) $(out2) ../
	cp * ./.backup
clean:
	rm -f *~ *.o $(out1) $(out2)
