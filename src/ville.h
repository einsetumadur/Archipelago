#include<iostream>
#include<string>
#include<vector>
#include"noeud.h"
#include"error.h"

#ifndef VILLE_H
#define VILLE_H

class Ville{
  public:
    Ville(string file);
    ~Ville();
    double ENJ();
    double CI();
    double MTA();

  private:
    void chargement(string file);
    void sauvegarde(string file);

    string fichier;
    vector<Noeud> logement;
    vector<Noeud> production;
    vector<Noeud> transport;
    vector<Lien> ponts;
};

#endif
