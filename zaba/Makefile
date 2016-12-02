ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

TARGET	:= zaba
TARGET2	:= szaba
DICT	:= zabaDict.cpp

CC	:= g++ -std=c++11 -Wall -fPIC 

CFLAGS	:= -g -DLINUX
LFLAGS	:= -lCAENVME -lm


SRCCPP  = main.cpp DGHelp.cpp DGModule.cpp DModule.cpp DGMultiGrid.cpp DMultiGrid.cpp DGMadc32.cpp DMadc32.cpp\
 DGV1718.cpp DV1718.cpp DGDisplay.cpp DDataSave.cpp zabaDict.cpp  
 
SRCCPP2 = szaba.cpp DMadc32.cpp
 
OBJECTS  = $(SRCCPP:.cpp=.o)
OBJECTS2 = $(SRCCPP2:.cpp=.o)

HEADERS = DGHelp.h DGModule.h DModule.h DGMultiGrid.h DMultiGrid.h DMadc32.h DGMadc32.h DV1718.h DGV1718.h DGDisplay.h DDataSave.h

all:  $(DICT) $(OBJECTS) $(TARGET) $(TARGET2)

$(DICT): $(HEADERS) zabaLinkDef.h
	echo "Generating builder root dictionary $@..."
	rootcling  -f $@ -c -DLINUX  $(HEADERS) zabaLinkDef.h


.cpp.o:
	$(CC) $< -c $(CFLAGS) $(ROOTCFLAGS) -o $@

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LFLAGS) -o $@

$(TARGET2):$(OBJECTS2) $(OBJECTS)

	$(CC) $(CFLAGS) $(OBJECTS2) DMultiGrid.o DModule.o DV1718.o DDataSave.o DGHelp.o DGModule.o DGMultiGrid.o DGMadc32.o DGV1718.cpp DGDisplay.o zabaDict.o\
 $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS) $(LFLAGS) -o $@

clean:
	rm -f *.o *Dict* $(TARGET) $(TARGET2) *log.txt
