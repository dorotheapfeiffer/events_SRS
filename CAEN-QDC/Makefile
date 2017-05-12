CC	= gcc
EXE 	= caen_qdc
SRC 	= src/_CAENDigitizer_DPP-QDC.c src/dpp_qdc.c src/readout_demo.c 
COPTS	= -DLINUX -Wall
FLAGS	=	
LIBS	= -lCAENDigitizer -lm
INCDIR 	= -I/usr/include # adapt to your include path
OBJS 	= *.o $(EXE)

#########################################################################

all	:	$(EXE)

clean	:
		/bin/rm -f $(OBJS) 

$(EXE)	:	$(SRC)
		$(CC) $(SRC) $(COPTS) $(INCDIR) $(FLAGS) -o $(EXE) $(LIBS) 

%.o	:	%.c
		$(CC) $(COPTS) $(INCDIR) -c -o $@ $<

