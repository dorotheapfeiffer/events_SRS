ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

TARGET	:= zaba
TARGET2	:= szaba
DICT	:= zabaDict.cpp

CC	:= g++ -std=c++11 -Wall -fPIC 

CFLAGS	:= -g -DLINUX
LFLAGS	:= -lCAENVME -lm -lCAENComm -lCAENDigitizer -lboost_iostreams -lboost_system -lboost_filesystem
INCLUDE := /home/dlab/Documents/caen/CAENDigitizer_2.6.7/include/

SRCCPP  = main.cpp DGHelp.cpp DGModule.cpp DModule.cpp DGAcquisition.cpp DAcquisition.cpp DAcqInfo.cpp DGMadc32.cpp DMadc32.cpp DKeyboard.cpp\
 DGV1718.cpp DV1718.cpp DCAEN1740.cpp DCAEN1740D.cpp DGCAEN1740D.cpp DGCAEN1740.cpp DGDisplay.cpp DDataSave.cpp _CAENDigitizer_DPP-QDC.cpp zabaDict.cpp  
 
SRCCPP2 = szaba.cpp DMadc32.cpp
 
OBJECTS  = $(SRCCPP:.cpp=.o)
OBJECTS2 = $(SRCCPP2:.cpp=.o)

HEADERS = DGHelp.h DGModule.h DModule.h DGAcquisition.h DAcquisition.h DAcqInfo.h DMadc32.h DGMadc32.h DCAEN1740.h DGCAEN1740.h DCAEN1740D.h\
	  DGCAEN1740D.h DV1718.h DGV1718.h DGDisplay.h DDataSave.h DKeyboard.h

all:  $(DICT) $(OBJECTS) $(TARGET) $(TARGET2)

$(DICT): $(HEADERS) zabaLinkDef.h
	echo "Generating builder root dictionary $@..."
	rootcling  -f $@ -c -I$(INCLUDE) -DLINUX  $(HEADERS) zabaLinkDef.h


.cpp.o:
	$(CC) $< -c $(CFLAGS) -I$(INCLUDE) $(ROOTCFLAGS) -o $@

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LFLAGS) -o $@

$(TARGET2):$(OBJECTS2) $(OBJECTS)

	$(CC) $(CFLAGS) $(OBJECTS2) DAcquisition.o DAcqInfo.o DModule.o DV1718.o DDataSave.o DGHelp.o DCAEN1740.o DGCAEN1740.o DCAEN1740D.o DGCAEN1740D.o\
	       	DGModule.o DGAcquisition.o _CAENDigitizer_DPP-QDC.o DGMadc32.o DGV1718.o DGDisplay.o DKeyboard.o zabaDict.o\
	       	$(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LFLAGS) -o $@

clean:
	rm -f *.o *Dict* $(TARGET) $(TARGET2) *log.txt
