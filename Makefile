#Makefile

CXX:g++
CXXFLAGS:-Wall -W_extra

all: Archipelago

Archipelago : rendu1.cc
  $(CXX) $(CXXFLAGS) -o rendu1
  #@echo "compilation términée avec succes"
