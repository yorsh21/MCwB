CC = gcc
CXX = g++
RM = rm -f
CPPFLAGS = -g $(shell root-config --cflags)
LDFLAGS = -g -Wall $(shell root-config --ldflags)
LDLIBS = $(shell root-config --libs)
WALL = -Wall
EXTRA = -lstdc++fs

SRCS = main.cpp
OBJS = $(subst .cc,.o,$(SRCS))

ifdef instance
	X = :
else
	X = \;
endif

all: clean main

main: $(OBJS) 
	$(CXX) $(WALL) $(SRCS) $(EXTRA) -o main
	./main ${instance}

clean: 
	$(RM) main

distclean: clean $(RM) main
