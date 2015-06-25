
include Makefile.mk

EXECUTABLE	:= acq
GUI		:= gui
CORE		:= core
CAEN5740	:= digitizer/CAEN5740


.PHONY: all $(GUI) $(CORE) $(CAEN5740) 

all: $(EXECUTABLE)

$(GUI) $(CORE) $(CAEN5740) $(SIM) :
	@$(MAKE) --directory=$@


$(EXECUTABLE): $(GUI) $(CORE) $(CAEN5740) 
	$(CC) $(CFLAGS) -o acq $(ROOTCFLAGS) $(ROOTLIBS) $(ROOTGLIBS)  gui/ADisplay.o  gui/ControlFrame.o \
gui/MainFrame.o   gui/guiDict.o  core/ABuildEvent.o  core/ADataSave.o  core/AEvent.o core/OnlineServer.o \
core/AManager.o core/ARingBuffer.o  core/ATrack.o  core/ControlAcq.o  core/Digitizer.o  core/coreDict.o \
core/main.o  digitizer/CAEN5740/CAEN5740Dlg.o digitizer/CAEN5740/CAEN5740.o digitizer/CAEN5740/CAEN5740Dict.o \
core/AHelp.o -lCAENComm -lCAENDigitizer -lusb -lstdc++

clean:
	@for d in  $(GUI) $(CORE) $(CAEN5740); \
	   do                              \
	    $(MAKE) --directory=$$d clean; \
	   done
	@rm -f acq *~





#@for d in  $(GUI) $(CORE) $(ACQIRIS) $(SCALER) $(SPECTRUM) $(SPD) $(SIM) \






















