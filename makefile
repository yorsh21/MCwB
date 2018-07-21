CC = gcc
CXX = g++
RM = rm -f
CPPFLAGS = -g $(shell root-config --cflags)
LDFLAGS = -g -Wall $(shell root-config --ldflags)
LDLIBS = $(shell root-config --libs)
WALL = -Wall

SRCS = main.cpp
OBJS = $(subst .cc,.o,$(SRCS))

all: clean main

main: $(OBJS) 
	$(CXX) $(WALL) $(SRCS) -o main
	./main

clean: 
	$(RM) main

distclean: clean $(RM) main
