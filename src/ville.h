/**\
 * \name    ville.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version Rendu 2
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
	Ville(bool val);
	~Ville();
	double mta();
	double enj();
	double ci();
	void chargement(char* nom_fichier);
	bool get_chargement_verif();
	unsigned int get_error_param_un();
	unsigned int get_error_param_deux();
	// sauvegarde du fichier :
	void sauvegarde(std::string file) const;
	std::string print_type(std::string type) const;
	unsigned int nb_type(std::string type) const;

private:
	void decodage(std::string line);
	static Ville* getVille();
	// creation de la ville & fonctions errors
	void ajout_noeud(std::istringstream& param,int& counter, Etat_lecture type);
	void error_noeud(Noeud* const nd);
	void creation_lien(unsigned int uid_a, unsigned int uid_b);
	void error_lien(Noeud* const a, Noeud* const b);
	Noeud* trouve_lien(unsigned int uid) const; 
	void redondance_uid(unsigned int numid);
	void collis_noeuds();

	std::string nom;
	bool chargement_verif;
	Type_error msg_error;
	unsigned int error_param_un;
	unsigned int error_param_deux;
	std::vector<Noeud*> quartiers;	
	double nbp_nbL;
	double nbp_nbT;
	double nbp_nbP;
	
};

void main_ville(char* nom_fichier);

#endif
