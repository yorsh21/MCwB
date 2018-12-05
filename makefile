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

ifdef mov
	Y = :
else
	Y = \;
endif

all: clean main

main: $(OBJS) 
	$(CXX) $(WALL) $(SRCS) $(CPPFLAGS) $(FSFLAGS) -o main
	./main ${instance} ${mov}

clean: 
	$(RM) main

distclean: clean $(RM) main
