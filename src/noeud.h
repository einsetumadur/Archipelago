#ifndef NOEUD_H
#define NOEUD_H

#include <iostream>
#include "tools.h"
#include <string>
#include <vector>

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
	void add_lien(Noeud* B, std::vector<Noeud*> ensN);	
	bool operator==(const Noeud& nd) const;
	void test_max_link();
	int verif_capacite(Noeud node);
private:
	Cercle quartier;
	unsigned int uid;
	unsigned int nbp;
	Type_quartier type;
	std::vector<Noeud*> liens;
};


void test_lien_quartier(Noeud* A, Noeud* C, Noeud* B);	
void test_coll_quartier(std::vector<Noeud*> ensN);
void test_nbp(unsigned int nbp);

#endif
