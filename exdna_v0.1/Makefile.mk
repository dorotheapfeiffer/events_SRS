ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

APPPATH	     := /home/dlab/dg_epool/Digitizers/exdna_v0.1
USERAPP	     := $(APPPATH)/app

CC = g++ -std=c++11 -g  
CFLAGS = -O2 -Wall -fPIC -I $(APPPATH)/lib/ -I $(APPPATH)/src -I$(USERAPP)
#CFLAGS += -DDEBUG
#LFLAGS = -L$(APPPATH)/lib -v

