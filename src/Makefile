# Makefile

CXX = g++
CXXFLAGS = -Wall -Wextra -g
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = prog.o noeud.o tools.o ville.o error.o graphics.o gui.o

all : projet

error.o : error.cc error.h constantes.h
	$(CXX) $(CXXFLAGS) -c error.cc -o error.o

tools.o : tools.cc tools.h graphics.h 
	$(CXX) $(CXXFLAGS) -c tools.cc -o tools.o

noeud.o : noeud.cc noeud.h tools.h error.h constantes.h
	$(CXX) $(CXXFLAGS) -c noeud.cc -o noeud.o

ville.o : ville.cc ville.h noeud.h constantes.h
	$(CXX) $(CXXFLAGS) -c ville.cc -o ville.o

graphics.o : graphics.cc graphics.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c graphics.cc -o graphics.o

gui.o : gui.cc gui.h ville.h graphics.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c gui.cc -o gui.o

prog.o : prog.cc ville.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c prog.cc -o prog.o 

projet : $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o projet $(LDLIBS)


clean:
	@echo "Nettoyage des fichiers de compilation"
	@rm *.o
