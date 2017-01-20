ROOTCONFIG = $(ROOTSYS)/bin/root-config
ROOTCINT   = $(ROOTSYS)/bin/rootcint

#CXX = clang++
CXX = $(shell $(ROOTCONFIG) --cxx)
#LD  = clang++
LD = $(shell $(ROOTCONFIG) --ld)

CXXFLAGS = $(shell $(ROOTCONFIG) --cflags) 
LDFLAGS  = $(shell $(ROOTCONFIG) --ldflags)
LIBS     = $(shell $(ROOTCONFIG) --glibs)
SOFLAGS  = -shared

EXECUTABLE = uTPCViewer
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

$(EXECUTABLE): $(TARGET)
	#$(CXX) -v $(CXXFLAGS) -std=c++11 -Wall -fPIC $^ $(LIBS) -lTreeViewer -o $@ # This line works
	$(CXX) -v $(CXXFLAGS) -std=c++11 -Wall -O3 -fPIC $(LIBS)  -o $@ $^

target: $(TARGET)

$(TARGET): $(OBJECTS) $(DICTOBJ)
	$(LD) $(LDFLAGS) $(SOFLAGS) -Wall -O3 -fPIC $(LIBS) -o $@ $^

objects: $(OBJECTS) $(DICTOBJ)

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -std=c++11 -Wall -O3 -fPIC -c -o $@ $<

dictionary: $(DICTIONARY)

$(DICTIONARY): $(INCLUDES) $(LINKDEF)
	$(ROOTCINT) -f $(DICTIONARY) -c -p $^

clean:
	rm -f $(EXECUTABLE) $(TARGET) $(OBJECTS) $(DICTIONARY) $(DICTOBJ) $(DICT)_rdict.pcm
.PHONY: clean
