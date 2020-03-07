#include<iostream>
#include<string>
#include<vector>
#include"noeud.h"
#include"error.h"

#ifndef VILLE_H
#define VILLE_H

class Ville{
  public:
    Ville(char* file);
    ~Ville();
    double ENJ();
    double CI();
    double MTA();

  private:
    void chargement( char * nom_fichier);
    void sauvegarde(string file);
    void decodage(string line);

    string fichier;
    vector<Noeud> logement;
    vector<Noeud> production;
    vector<Noeud> transport;
    vector<Lien> ponts;
};

#endif
