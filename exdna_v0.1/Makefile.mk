ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
USER	     := /home/dlab/Documents/programC/exdna_v0.1/app

CC = g++ -std=c++11 -g  
CFLAGS = -O2 -Wall -fPIC -I ./lib/ -I ./src -I$(USER)
#CFLAGS += -DDEBUG
LFLAGS = -L/home/dlab/Documents/programC/exdna_v0.1/lib -v

