#include<iostream>
#include<string>
#include<vector>
using namespace std;

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
    vector<Noeud_L> logement;
    vector<Noeud_P> production;
    vector<Noeud_T> transport;
    vector<Lien> ponts;
};

#endif
