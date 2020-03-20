# Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -g
OFILES = prog.o noeud.o tools.o ville.o error.o

all : Archipelago

error.o : error.cc error.h
	$(CXX) $(CXXFLAGS) -c error.cc -o error.o

tools.o : tools.cc tools.h
	$(CXX) $(CXXFLAGS) -c tools.cc -o tools.o

noeud.o : noeud.cc noeud.h tools.o error.o
	$(CXX) $(CXXFLAGS) -c noeud.cc -o noeud.o

ville.o : ville.cc ville.h tools.o noeud.o 
	$(CXX) $(CXXFLAGS) -c ville.cc -o ville.o

prog.o : prog.cc ville.o
	$(CXX) $(CXXFLAGS) -c prog.cc -o prog.o

Archipelago : $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o Archipelago
