/**\
 * \name    ville.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#ifndef VILLE_H
#define VILLE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "noeud.h"

class Ville
{
public:
	Ville(char* file);
	void sauvegarde(std::string file);

private:
	void chargement( char * nom_fichier);
	void decodage(std::string line);
	unsigned int nb_type(Type_quartier type);
	std::string print_type(Type_quartier type);
	void ajout_noeud(std::istringstream &param,int &counter,Type_quartier type);
	void creation_lien(unsigned int uid_a, unsigned int uid_b);
	Noeud* trouve_lien(unsigned int uid); 
	void redondance_uid();
	void test_coll_quartier(std::vector<Noeud*> ensN);

	std::string nom;
	std::vector<Noeud*> quartiers;
};


#endif
