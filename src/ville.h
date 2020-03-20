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
    int redondance_uid();
    void chargement( char * nom_fichier);
    void sauvegarde(string file);
    void decodage(string line);
    Noeud* trouve_lien(unsigned int uid); 
    unsigned int nb_type(Type_quartier type);
    string print_type(Type_quartier type);

    string fichier;
    vector<Noeud*> quartiers;
};

#endif
