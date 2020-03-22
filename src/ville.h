/**\
 * \name    ville.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#ifndef VILLE_H
#define VILLE_H

#include <iostream>
#include <string>
#include <vector>
#include "noeud.h"

class Ville{
public:
  Ville(char* file);
  void sauvegarde(std::string file);

private:
  void redondance_uid();
  void chargement( char * nom_fichier);
  void decodage(std::string line);
  void ajout_noeud(u_int numid,double posx,double posy,
                   u_int popmax,Type_quartier type);
  Noeud* trouve_lien(unsigned int uid); 
  unsigned int nb_type(Type_quartier type);
  std::string print_type(Type_quartier type);

  std::string nom;
  std::vector<Noeud*> quartiers;
};


#endif
