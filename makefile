CC = gcc
CXX = g++
RM = rm -f
CPPFLAGS = -g -std=c++11 -pthread
WALL = -Wall
FSFLAGS = -lstdc++fs

SRCS = main.cpp
OBJS = $(subst .cc,.o,$(SRCS))

ifdef instance
	X = :
else
	X = \;
endif

ifdef threads
	Y = :
else
	Y = \;
endif

all: clean main

main: $(OBJS) 
	$(CXX) $(WALL) $(SRCS) $(CPPFLAGS) $(FSFLAGS) -o main
	./main ${instance} ${threads}

clean: 
	$(RM) main

distclean: clean $(RM) main
