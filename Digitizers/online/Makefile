
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CC = g++ -g  
CFLAGS = -O2 -Wall -fPIC -I ../core/ 
LFLAGS = -L ./lib/ -v


SOURCES := mySpy.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o) 
DICT    := mySpyDict.cpp
SHARED  := libSPY.so
EXEC	:= spy

all: $(DICT) $(OBJECTS) $(SHARED) $(EXEC) 

$(DICT): mySpy.h ../core/AEvent.h ../core/ATrack.h mySpyLinkDef.h
	rootcint -f $@ -c -p -I. -I../core mySpy.h ATrack.h AEvent.h mySpyLinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o mySpyDict.o

$(SHARED): $(OBJECTS)
	$(CC) -shared $(CFLAGS) ../core/AEvent.o ../core/ATrack.o mySpyDict.o mySpy.o  $(ROOTGLIBS) $(ROOTCFLAGS) -o $@

.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

$(EXEC):
	$(CC) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) main.o -L. -lSPY  -o $@

clean:
	@rm -f  $(SHARED) $(OBJECTS) *Dict* spy
	
