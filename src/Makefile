# Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -g
OFILES = rendu.o

all : Archipelago

rendu.o : rendu.cc
	$(CXX) $(CXXFLAGS) -c rendu.cc -o rendu.o

Archipelago : $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o Archipelago
