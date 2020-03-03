#include<iostream>
#include<string>
#include<vector>

#ifndef VILLE_H
#define VILLE_H

class Ville{
  public:
    Ville(string file);
    ~Ville(string file);

  private:
    void chargement(string file);
    void sauvegarde(string file);
    void verification();

    string fichier;
    vector<Noeud_L> logement;
    vector<Noeud_P> production;
    vector<Noeud_T> transport;
    vector<Lien> ponts;
};

#endif VILLE_H
