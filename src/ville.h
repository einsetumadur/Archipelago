
#include <iostream>
#include <string>
#include <vector>
#include "noeud.h"

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
    void redondance_uid();
    void chargement( char * nom_fichier);
    void sauvegarde(std::string file);
    void decodage(std::string line);
    void ajout_noeud(unsigned int numid,double posx,double posy,unsigned int popmax,Type_quartier type);
    Noeud* trouve_lien(unsigned int uid); 
    unsigned int nb_type(Type_quartier type);
    std::string print_type(Type_quartier type);

    std::string fichier;
    std::vector<Noeud*> quartiers;
};

#endif
