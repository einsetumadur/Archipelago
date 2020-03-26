/**\
 * \name   noeud.h
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */
 
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
	void test_nbp();
	void ajout_lien(Noeud* b);
	bool multiple_link(Noeud* b);	
	bool test_max_link();
	bool collis_lien_quartier(Noeud* lien_a, Noeud* lien_b); 
	bool operator==(const Noeud& nd) const;

private:
	Cercle batiment;
	unsigned int uid;
	unsigned int nbp;
	Type_quartier type;
	std::vector<Noeud*> liens;
};

#endif
