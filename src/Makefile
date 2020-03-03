# Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -g
OFILES = rendu.o noeud.o tools.o ville.o

all : Archipelago

tools.o : tools.cc tool.h
	$(CXX) $(CXXFLAGS) -c tools.cc -o tools.o

noeud.o : noeud.cc noeud.h tools.o
	$(CXX) $(CXXFLAGS) -c noeud.cc -o noeud.o

ville.o : ville.cc ville.h tools.o noeud.o
	$(CXX) $(CXXFLAGS) -c ville.cc -o ville.o

rendu.o : rendu.cc ville.o
	$(CXX) $(CXXFLAGS) -c rendu.cc -o rendu.o

Archipelago : $(OFILES)
	$(CXX) $(CXXFLAGS) $(OFILES) -o Archipelago
