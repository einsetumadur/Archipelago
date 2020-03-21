
#include <iostream>
#include "tools.h"
#include <string>
#include <vector>

#ifndef NOEUD_H
#define NOEUD_H

enum Type_quartier {PRODUCTION, TRANSPORT, LOGEMENT};
class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite, 
												Type_quartier type);
	//FAIRE UN DESTRUCTEUR
	std::string print();
	unsigned int get_nbp();
	double getx() const ;
	double gety() const ;
	double getrayon() const ;
	Cercle getQuartier() const ;
	unsigned int getUid() const ;
	Type_quartier getType() const;	
	void add_lien(Noeud* B);	
	bool operator==(const Noeud& nd) const;
	std::string test_max_link();
	int verif_capacite(Noeud node);
private:
	Cercle quartier;
	unsigned int uid;
	unsigned int nbp;
	Type_quartier type;
	std::vector<Noeud*> liens;
};


std::string test_lien_quartier(Noeud A, Noeud C, Noeud B);	
std::string test_coll_quartier(std::vector<Noeud> ensN);
std::string test_nbp(unsigned int nbp, double rayon);

#endif
