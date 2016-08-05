ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

APPPATH	     := $(PWD)
USERAPP	     := $(APPPATH)/app

CC = g++ -std=c++11 -g  
CFLAGS = -Wall -fPIC -I $(APPPATH)/lib/ -I $(APPPATH)/src -I$(USERAPP)
#CFLAGS += -DDEBUG
#LFLAGS = -L$(APPPATH)/lib -v

