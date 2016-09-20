
include ../../Makefile.mk


CFLAGS  += -I./
CFLAGS  += -I../../core
CFLAGS  += -I../../gui

SRCCPP  := CAEN5740.cpp CAEN5740Dlg.cpp
HEADERS := CAEN5740.h CAEN5740Dlg.h
DICT    := CAEN5740Dict.cpp
OBJECTS := $(SRCCPP:.cpp=.o)

all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) CAEN5740LinkDef.h
	rootcint -f $@ -c -p -I./ -I../../core $(HEADERS) CAEN5740LinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o CAEN5740Dict.o	
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* 
