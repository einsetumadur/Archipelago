#include<iostream>
#include<string>
#include <vector>
#include <cmath>
#include"noeud.h"
#include"tools.h"
#include"error.h"

using namespace std;


Noeud::Noeud(unsigned int id, double x, double y, unsigned int capacite): 
            uid(id), quartier{ {x,y}, sqrt(capacite)}, nbp(capacite)
{
    test_nbp(uid, getrayon());	// verifie la validite de la capacite
    
    // test si un lien apparait 2 fois
   /* test_duplicite_lien(); */
	
}
    
unsigned int Noeud::get_nbp()
{
    return nbp;
}

string Noeud::print()
{
    return to_string(uid)+" "+to_string(quartier.centre.pos_x)+" "+to_string(quartier.centre.pos_y)+" "+to_string(nbp);
}

double Noeud::getx() const {
	
	return (getQuartier() ).centre.pos_x;
}
double Noeud::gety() const {
	
	return (getQuartier() ).centre.pos_y;
}
double Noeud::getrayon() const {
	
	return (getQuartier() ).rayon;
}
Cercle Noeud::getQuartier() const  {
	
	return quartier;
}
unsigned int Noeud::getUid() const  {
	
	return uid;
}

int Noeud::verif_uid_quartier(vector<Noeud> ensemble) {		// verifie la non-duplicite de l'uid dans un seul type de quartier
	
	for(size_t i(0) ; i < ensemble.size() ; ++i) {
		for(size_t j(i+1) ; j < ensemble.size() ; ++j) {
			if (ensemble[i].uid == ensemble [j].uid) return ensemble[i].uid;
		}
	}
	return 0;
}
int Noeud::verif_uid_interquartier(vector<Noeud> logement, vector<Noeud> production, vector<Noeud> transport) { // verifie la non-duplicite de l'uid dans tous les types de quartier
	
	size_t taille;
	if(logement.size() <= production.size() and logement.size() <= transport.size())
		taille = logement.size();
	else if(production.size()  <= logement.size() and production.size() <= transport.size())
		taille = production.size();
	else
		taille = transport.size();
		
	for(size_t i(0) ; i < taille ; ++i) {
		if(logement[i].uid == production[i].uid or logement[i].uid == transport[i].uid)
			return logement[i].uid;
		else if (production[i].uid == transport[i].uid)
			return production[i].uid;
	}
	
	return 0;
}

string test_nbp(unsigned int nbp, double rayon) {
	unsigned int i(0);
	if(sqrt(nbp) == rayon)
		i = 1;
	else if(sqrt(nbp) < rayon)
		i = 0;
	else if(sqrt(nbp) > rayon)
		i = 2;
	
	switch(i) {
		case 0 : error::too_little_capacity(nbp);
				break;
		case 2 : error::too_much_capacity(nbp);
				break;
	}
}

Logement::Logement(unsigned int id, double x, double y, unsigned int capacite):  
						Noeud(id,x,y,capacite)
{
	// test max_link
   /* if(test_max_link(liens.size()))
		error::max_link(id); */
}

/*bool Logement::test_max_link(size_t nb_liens) {
	if(nb_liens > 3)
		return 1;
	else
		return 0;
} */

Lien::Lien(Noeud* nd1, Noeud* nd2): noeud1(nd1), noeud2(nd2) 
{
	// test self link node
	if(*nd1 == *nd2)	
		error::self_link_node(nd1->getUid());

	// Modelisation geometrique du lien
	double vx = ((*nd2).getx() - (*nd2).getrayon()) - ((*nd1).getx()-(*nd1).getrayon());
	double vy = ((*nd2).gety() - (*nd2).getrayon()) - ((*nd1).gety()-(*nd1).getrayon());
	trait.directeur = {vx, vy};
	
	// test node overlap link
	if(collision_droite_cercle( (*nd1).getQuartier(), trait))
		error::node_link_superposition( (*nd1).getUid());
	if(collision_droite_cercle( (*nd2).getQuartier(), trait))
		error::node_link_superposition( (*nd2).getUid());
} 

Seg_droite Lien::getTrait() {
	
	return trait;
}


string test_lien_quartier(vector<Lien> ensL, vector<Noeud> ensN) {		//collision lien quartier
	
	for(size_t i(0) ; i < ensL.size() ; ++i) {
		for(size_t j(0) ; j < ensN.size() ; ++j) {
			if(collision_droite_cercle( ensN[j].getQuartier(), ensL[i].getTrait() ))
				error::node_link_superposition( ensN[j].getUid() );
		}
	}
}

string test_coll_quartier(vector<Noeud> ensN) {							// collision lien quartier
	
	for(size_t i(0) ; i < ensN.size() - 1 ; ++i) {
		if(collision_cercle(ensN[i].getQuartier(), ensN[i+1].getQuartier()) )
			error::node_node_superposition(ensN[i].getUid(), ensN[i+1].getUid() );
	}
}

//Surcharge d'operateur == pour les noeuds 
bool Noeud::operator==(const Noeud& nd) const  {
	
	return ( (getx() == nd.getx()) and (gety() == nd.gety()) and (getrayon() == nd.getrayon()) ); 
}

/*string Noeud::test_duplicite_lien() {
	
	for(size_t i(0) ; i < liens.size() ; ++i) {
		for(size_t j(i+1) ; j < liens.size() ; ++j) {
			if(liens[i] == liens[j])
				return error::multiple_same_link(getUid(), (*liens[i]).getUid());
		}
	}
} */
