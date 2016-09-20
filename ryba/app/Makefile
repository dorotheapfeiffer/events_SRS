
include ../Makefile.mk

CFLAGS	+= -I../lib
SOURCES	= UserClass.cpp
HEADERS	= UserClass.h
OBJECTS	= $(SOURCES:.cpp=.o)

all: $(OBJECTS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o 

