#include<iostream>
#include<string>
#include <vector>
#include <cmath>
#include"noeud.h"
#include"tools.h"
#include"error.h"

using namespace std;


Noeud::Noeud(unsigned int id, double x, double y, unsigned int capacite, type_quartier type): 
            uid(id), quartier{ {x,y}, sqrt(capacite)}, nbp(capacite), type(type)
{
    test_nbp(uid, getrayon());	// verifie la validite de la capacite
}
    
string Noeud::test_max_link() {
	if(liens.size() > 3)				//MAGIC NUMBER
		error::max_link( getUid() );
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
type_quartier Noeud::getType() const {

	return type;
}

void Noeud::add_lien(Noeud* B) {

	 if(type == 2)
		test_max_link();
	 else if ((*B).getType() == 2) 
	 	(*B).test_max_link();
	
	liens.push_back(B);
}



int Noeud::verif_uid_quartier(vector<Noeud> ensemble) {		// verifie la non-duplicite de l'uid dans un seul type de quartier
	
	for(size_t i(0) ; i < ensemble.size() ; ++i) {
		for(size_t j(i+1) ; j < ensemble.size() ; ++j) {
			if (ensemble[i].uid == ensemble [j].uid) return ensemble[i].uid;
		}
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


bool test_lien_quartier(Noeud A, Noeud C, Noeud B) {	
	Point p = { A.getx(), A.gety()};
	Seg_droite d = { p, {B.getx() - A.getx(), B.gety() - A.gety() } };
	return collision_droite_cercle(C.getQuartier(), d);
} 

string test_coll_quartier(vector<Noeud> ensN) {		
	for(size_t i(0) ; i < ensN.size() - 1 ; ++i) {
		if(collision_cercle(ensN[i].getQuartier(), ensN[i+1].getQuartier()) )
			error::node_node_superposition(ensN[i].getUid(), ensN[i+1].getUid() );
	}
}

//Surcharge d'operateur == pour les noeuds 
bool Noeud::operator==(const Noeud& nd) const  {
	
	return ( (getx() == nd.getx()) and (gety() == nd.gety()) and (getrayon() == nd.getrayon()) ); 
}

