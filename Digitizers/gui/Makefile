
include ../Makefile.mk

CFLAGS  += -I./
CFLAGS  += -I../core

SRCCPP  := guiDict.cpp ADisplay.cpp ControlFrame.cpp MainFrame.cpp 

OBJECTS := $(SRCCPP:.cpp=.o)

HEADERS := ADisplay.h ControlFrame.h MainFrame.h

DICT    := guiDict.cpp


all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) guiLinkDef.h
	rootcint -f $@ -c -p -I../core -I. $(HEADERS) guiLinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o guiDict.o
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* *~ 
