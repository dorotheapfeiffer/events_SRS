
include ../../Makefile.mk


CFLAGS  += -I./
CFLAGS  += -I../../core
CFLAGS  += -I../../gui

SRCCPP  := SimCard.cpp SimCardDlg.cpp
HEADERS := SimCard.h SimCardDlg.h
DICT    := SimCardDict.cpp
OBJECTS := $(SRCCPP:.cpp=.o)

all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) SimCardLinkDef.h
	rootcint -f $@ -c -p -I. -I../../core $(HEADERS) SimCardLinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o SimCardDict.o	
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* 
