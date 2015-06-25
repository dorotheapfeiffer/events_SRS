ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CC	:= clang -g 
CFLAGS	:= -O2 -m64 -pipe -Wshadow -W -Wall -Woverloaded-virtual -fsigned-char -fno-common -std=c++11 -I./
#CFLAGS  += -DDEBUG
LFLAGS	:= 


