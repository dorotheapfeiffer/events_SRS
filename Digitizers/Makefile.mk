ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CC	:=  g++ -g --std=c++11 
CFLAGS	:= -m64 -pipe -Wshadow -W -Wall -Woverloaded-virtual -fsigned-char -fno-common -I./
#CFLAGS  += -DDEBUG
LFLAGS	:= 


