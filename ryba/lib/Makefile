
include ../Makefile.mk

SOURCES := ReadData.cpp Files.cpp AEvent.cpp ATrack.cpp ASignal.cpp
OBJECTS := $(SOURCES:.cpp=.o)
DICT    := dataDict.cpp
SHARED  := libData.so

all: $(DICT) $(OBJECTS) $(SHARED) 

$(DICT): ReadData.h Files.h AEvent.h ATrack.h ASignal.h dataLinkDef.h 
	rootcling -f $@ -c -p -I. ReadData.h Files.h AEvent.h ATrack.h ASignal.h dataLinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o dataDict.o

$(SHARED): $(OBJECTS)
	$(CC) -shared $(CFLAGS) dataDict.o $(OBJECTS) $(ROOTGLIBS) $(ROOTCFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f  $(SHARED) $(OBJECTS) *Dict* 
	
