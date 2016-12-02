
include ../Makefile.mk


CFLAGS  += -I$(USERAPP) -I ../lib

SRCCPP  := srcDict.cpp MainWindow.cpp main.cpp  

OBJECTS := $(SRCCPP:.cpp=.o)
HEADERS := MainWindow.h $(USERAPP)/UserClass.h 
DICT    := srcDict.cpp

all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) srcLinkDef.h
	rootcling -f $@ -c -p -I. $(HEADERS) srcLinkDef.h
	
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* 
