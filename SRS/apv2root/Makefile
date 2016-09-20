ROOTCONFIG = $(ROOTSYS)/bin/root-config
ROOTCINT   = $(ROOTSYS)/bin/rootcint

#CXX = clang++
CXX = $(shell $(ROOTCONFIG) --cxx)
#LD  = clang++
LD = $(shell $(ROOTCONFIG) --ld)

CXXFLAGS = $(shell $(ROOTCONFIG) --cflags) 
LDFLAGS  = $(shell $(ROOTCONFIG) --ldflags)
LIBS     = $(shell $(ROOTCONFIG) --glibs)
%SOFLAGS  = -shared

EXECUTABLE = apv2root
TARGET     = lib$(EXECUTABLE).so
INCLUDES   = $(wildcard *.h)
SOURCES    = $(wildcard *.cxx)
OBJECTS    = $(SOURCES:.cxx=.o)
DICT       = $(EXECUTABLE)Dict
DICTIONARY = $(DICT).cxx
DICTOBJ    = $(DICT).o 
LINKDEF    = $(EXECUTABLE)LinkDef.h

all: executable

forceall: clean executable

executable: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(DICTOBJ)
	$(CXX) -v $(CXXFLAGS) -O2  -Wall $^ $(LIBS) -o $@ # This line works
	

target: $(TARGET)

$(TARGET): $(OBJECTS) $(DICTOBJ)
	$(LD) $(LDFLAGS) $(SOFLAGS) -Wall $(LIBS) -o $@ $^

objects: $(OBJECTS) $(DICTOBJ)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -O2 -Wall  -c -o $@ $<

dictionary: $(DICTIONARY)

$(DICTIONARY): $(INCLUDES) $(LINKDEF)
	$(ROOTCINT) -f $(DICTIONARY) -c -p $^

clean:
	rm -f $(EXECUTABLE) $(TARGET) $(OBJECTS) $(DICTIONARY) $(DICTOBJ) $(DICT)_rdict.pcm
	
.PHONY: clean
