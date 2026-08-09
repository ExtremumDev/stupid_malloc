CC = gcc
CFLAGS = -Wall -g

CXX = g++
CXXFLAGS = -Wall -g

malloc.o: ../../malloc.c ../../malloc.h
	$(CC) $(CFLAGS) -DBUFFERVERSION $< -c 

free.o: ../../free.c ../../malloc.h
	$(CC) $(CFLAGS) -DBUFFERVERSION $< -c

core.o: core.cpp core.hpp
	$(CXX) $(CXXFLAGS) $< -c


prog: main.cpp malloc.o free.o core.o
	$(CXX) $(CXXFLAGS) $^ -lfltk -o $@