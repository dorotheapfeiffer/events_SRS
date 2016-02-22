
include ../Makefile.mk


#CFLAGS  += -I./
CFLAGS  += -I../gui
CFLAGS  += -I../digitizer/CAEN5740 -I../digitizer/SIM

SRCCPP  := ABuildEvent.cpp ADataSave.cpp ADataSave2.cpp AEvent.cpp AManager.cpp ATrack.cpp \
ARingBuffer.cpp AHelp.cpp ControlAcq.cpp Digitizer.cpp OnlineServer.cpp main.cpp 

OBJECTS := $(SRCCPP:.cpp=.o)
HEADERS := ABuildEvent.h ADataSave.h ADataSave2.h AEvent.h AManager.h ATrack.h ARingBuffer.h AHelp.h ControlAcq.h OnlineServer.h Digitizer.h 
DICT    := coreDict.cpp

all: $(DICT) $(OBJECTS) 

$(DICT): $(HEADERS) coreLinkDef.h
	rootcint -f $@ -c -p -I. -I../gui $(HEADERS) coreLinkDef.h
	$(CC) -c $(CFLAGS) $(DICT) $(ROOTCFLAGS) -o coreDict.o	
.cpp.o:
	$(CC) -c $(CFLAGS) $< $(ROOTCFLAGS) -o $@

clean:
	@rm -f *.o *Dict.* *~
