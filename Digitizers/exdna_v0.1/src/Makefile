
include ../Makefile.mk


CFLAGS  += -I$(USERAPP) -I ../lib

SRCCPP  := exdnaDict.cpp MainWindow.cpp main.cpp UserBase.cpp 

OBJECTS := $(SRCCPP:.cpp=.o)
HEADERS := MainWindow.h $(USERAPP)/UserClass.h UserBase.h 
DICT    := exdnaDict.cpp

all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) exdnaLinkDef.h
	rootcling -f $@ -c -p -I. $(HEADERS) exdnaLinkDef.h
	
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* 
