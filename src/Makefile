# Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -g
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = prog.o noeud.o tools.o ville.o error.o gui.o

all : Archipelago

error.o : error.cc error.h constantes.h
	$(CXX) $(CXXFLAGS) -c error.cc -o error.o

tools.o : tools.cc tools.h constantes.h
	$(CXX) $(CXXFLAGS) -c tools.cc -o tools.o

noeud.o : noeud.cc noeud.h tools.h error.h constantes.h
	$(CXX) $(CXXFLAGS) -c noeud.cc -o noeud.o

ville.o : ville.cc ville.h noeud.h 
	$(CXX) $(CXXFLAGS) -c ville.cc -o ville.o

gui.o : gui.cc gui.h ville.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c gui.cc -o gui.o

prog.o : prog.cc ville.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c prog.cc -o prog.o 

Archipelago : $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o Archipelago $(LDLIBS)


clean:
	@echo "Nettoyage des fichiers de compilation"
	@rm *.o