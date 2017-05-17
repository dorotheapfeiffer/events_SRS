ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


TARGET2	:= szaba
DICT	:= zabaDict.cpp

CC	:= g++ -std=c++11 -Wall -fPIC 

CFLAGS	:= -g -DLINUX
LFLAGS	:= -lCAENVME -lm -lCAENComm -lCAENDigitizer -lboost_iostreams -lboost_system -lboost_filesystem
INCLUDE := /home/dlab/Documents/caen/CAENDigitizer_2.6.7/include/

SRCCPP  = DModule.cpp DAcquisition.cpp DAcqInfo.cpp DMadc32.cpp DKeyboard.cpp\
 DV1718.cpp DCAEN1740.cpp DCAEN1740D.cpp DDataSave.cpp _CAENDigitizer_DPP-QDC.cpp zabaDict.cpp  
 
SRCCPP2 = szaba.cpp DMadc32.cpp
 
OBJECTS  = $(SRCCPP:.cpp=.o)
OBJECTS2 = $(SRCCPP2:.cpp=.o)

HEADERS = DModule.h DAcquisition.h DAcqInfo.h DMadc32.h DCAEN1740.h DCAEN1740D.h\
	  DV1718.h DDataSave.h DKeyboard.h

all:  $(DICT) $(OBJECTS) $(TARGET2)

$(DICT): $(HEADERS) zabaLinkDef.h
	echo "Generating builder root dictionary $@..."
	rootcling  -f $@ -c -I$(INCLUDE) -DLINUX  $(HEADERS) zabaLinkDef.h


.cpp.o:
	$(CC) $< -c $(CFLAGS) -I$(INCLUDE) $(ROOTCFLAGS) -o $@


$(TARGET2):$(OBJECTS2) $(OBJECTS)

	$(CC) $(CFLAGS) $(OBJECTS2) DAcquisition.o DAcqInfo.o DModule.o DV1718.o DDataSave.o DCAEN1740.o DCAEN1740D.o\
	       	_CAENDigitizer_DPP-QDC.o DKeyboard.o zabaDict.o\
	       	$(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LFLAGS) -o $@

clean:
	rm -f *.o *Dict* $(TARGET2) *log.txt
