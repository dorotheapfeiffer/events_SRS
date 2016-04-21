
include Makefile.mk

EXE	:= ryba
LIB	:= $(APPPATH)/lib
SRC	:= $(APPPATH)/src

.PHONY: all $(LIB) $(USERAPP) $(SRC) 

all: $(EXE)

$(LIB) $(USERAPP) $(SRC):
	@ $(MAKE) --directory=$@

$(EXE): $(LIB) $(USERAPP) $(SRC)
	 $(CC) $(CFLAGS) $(LFLAGS) -o$@ $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS)\
 $(USERAPP)/UserClass.o $(APPPATH)/src/MainWindow.o $(APPPATH)/src/exdnaDict.o\
 $(APPPATH)/src/main.o -L$(LIB) -lData

clean:
	@ for d in $(LIB) $(USERAPP) $(SRC) ;  \
	   do                              \
	    $(MAKE) --directory=$$d clean; \
	   done
