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
	std::string print();
	unsigned int getNbp();
	double getX() const ;
	double getY() const ;
	double getRayon() const ;
	Cercle getBatiment() const ;
	unsigned int getUid() const ;
	Type_quartier getType() const;	
	void add_lien(Noeud* b, std::vector<Noeud*> ensN);	
	bool operator==(const Noeud& nd) const;
	void test_max_link();
private:
	Cercle batiment;
	unsigned int uid;
	unsigned int nbp;
	Type_quartier type;
	std::vector<Noeud*> liens;
};

void test_lien_quartier(Noeud* lien_a, Noeud* lien_b, Noeud* nd);
void test_coll_quartier(std::vector<Noeud*> ensN);
void test_nbp(unsigned int nbp);

#endif

