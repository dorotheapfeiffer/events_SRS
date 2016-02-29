
include Makefile.mk

EXE	:= ryba
LIB	:= lib
SRC	:= src

.PHONY: all $(LIB) $(USER) $(SRC) 

all: $(EXE)

$(LIB) $(USER) $(SRC):
	@ $(MAKE) --directory=$@

$(EXE): $(LIB) $(USER) $(SRC)
	 $(CC) $(CFLAGS) $(LFLAGS) -o$@ $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) \
$(USER)/UserClass.o src/MainWindow.o src/exdnaDict.o src/main.o src/UserBase.o -lData
#lib/AEvent.o lib/ATrack.o lib/Data.o lib/Files.o lib/ReadData.o lib/ReadDataFile.o lib/dataDict.o lib/ReadDataNet.o 

clean:
	@ for d in $(LIB) $(APP) $(SRC) ;  \
	   do                              \
	    $(MAKE) --directory=$$d clean; \
	   done
