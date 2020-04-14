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

enum Etat_lecture {NBL,LOGE,NBP,PROD,NBT,TRAN,NBLI,LIENS,FIN};

class Ville
{
public:
	Ville(unsigned int nbL, unsigned int nbT, unsigned int nbP);
	~Ville();
	double mta();
	double enj();
	double ci();
	void chargement(char* nom_fichier);

private:
	void decodage(std::string line);
	void ajout_noeud(std::istringstream& param,int& counter, Etat_lecture type);
	void creation_lien(unsigned int uid_a, unsigned int uid_b);
	void error_lien(Noeud* a, Noeud* b);
	Noeud* trouve_lien(unsigned int uid) const; 
	void redondance_uid(unsigned int numid) const;
	void collis_noeuds();

	std::string nom;
	std::vector<Noeud*> quartiers;
	unsigned int nbL;
	unsigned int nbT;
	unsigned int nbP;
	
	double nbp_nbL;
	double nbp_nbT;
	double nbp_nbP;
	
};

void main_ville(char* nom_fichier);

#endif
