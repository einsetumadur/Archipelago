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

class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite);
	unsigned int getNbp();
	double getX() const ;
	double getY() const ;
	double getRayon() const ;
	Cercle getBatiment() const ;
	unsigned int getUid() const ;
	void test_nbp();
	void ajout_lien(Noeud* b);
	bool multiple_link(Noeud* b) ;	
	virtual bool maxi_link();
	bool collis_lien_quartier(Noeud* lien_a, Noeud* lien_b); 
	bool operator==(const Noeud& nd) const;

protected:
	Cercle batiment;
	unsigned int uid;
	unsigned int nbp;
	std::vector<Noeud*> liens;
};


class Logement : public Noeud 
{
	
public:
	Logement(unsigned int id,double x, double y, unsigned int capacite);
	virtual bool maxi_link() override;

private:
};

class Transport : public Noeud
{
public:
	Transport(unsigned int id,double x, double y, unsigned int capacite);
private:
};

class Production : public Noeud
{
public:
	Production(unsigned int id,double x, double y, unsigned int capacite);
	 
private:

};

#endif
