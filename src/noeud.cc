/**\
 * \name    noeud.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "noeud.h"
#include "tools.h"
#include "error.h"
#include "constantes.h"

using namespace std;


Noeud::Noeud(unsigned int id, double x, double y, unsigned int capacite, 
													Type_quartier type): 
            uid(id), batiment{ {x,y}, sqrt(capacite)}, nbp(capacite), type(type)
{
    test_nbp(nbp);
    if(uid == no_link)
    {
		cout << error::reserved_uid();
		exit(-1);
	}		
}
    
void Noeud::test_max_link() 
{
	if(liens.size() == max_link)			
	{			
		cout << error::max_link( getUid() );
		exit(-1);
	}
}
unsigned int Noeud::getNbp()
{
    return nbp;
}

string Noeud::print()
{
    return to_string(uid)+" "+to_string(getX())+" "+
								to_string(getY())+" "+to_string(nbp);
}

double Noeud::getX() const 
{
	
	return batiment.centre.pos_x;
}
double Noeud::getY() const 
{
	
	return batiment.centre.pos_y;
}
double Noeud::getRayon() const 
{
	
	return batiment.rayon;
}
Cercle Noeud::getBatiment() const  
{
	
	return batiment;
}
unsigned int Noeud::getUid() const 
{
	
	return uid;
}
Type_quartier Noeud::getType() const 
{

	return type;
}

void Noeud::add_lien(Noeud* b, vector<Noeud*> ensN) 
{
	// MULTIPLE NODE
	for(size_t i(0) ; i < liens.size() ; ++i) 
	{
		if(liens[i] == b) 
		{
			cout << error::multiple_same_link(uid, b->uid);
			exit(-1);
		}
	}

	// MAX HOUSING LINKS
	if(type == LOGEMENT)	test_max_link();
	
	// COLLISION LIENS-BATIMENT
	for(size_t i(0) ; i < ensN.size() ; ++i) 
	{
		if( not(ensN[i] == this) and not(ensN[i] == b) ) 
		{
			if(triangle(this->batiment, b->batiment, ensN[i]->batiment))	
											test_lien_quartier(this, b, ensN[i]);
		}
	}
	liens.push_back(b);
}

void test_nbp(unsigned int nbp) 
{
	if(nbp < min_capacity) 
	{
		cout << error::too_little_capacity(nbp);
		exit(-1);
	}
	else if(nbp > max_capacity)
	{
		cout << error::too_much_capacity(nbp);
		exit(-1);
	}
}

void test_lien_quartier(Noeud* lien_a, Noeud* lien_b, Noeud* nd) 
{	
	Point p = { lien_a->getX(), lien_a->getY() };
	Seg_droite d = { p, {lien_b->getX() - lien_a->getX(), 
						lien_b->getY() - lien_a->getY()}};
	if(collision_droite_cercle(nd->getBatiment(), d))
	{
		cout << error::node_link_superposition(nd->getUid());
		exit(-1);
	}

} 

void test_coll_quartier(vector<Noeud*> ensN) 
{		
	for(size_t i(0) ; i < ensN.size() ; ++i) 
	{
		for(size_t j(i+1) ; j < ensN.size() ; ++j) 
		{
			if(collision_cercle(ensN[i]->getBatiment(), ensN[j]->getBatiment()))
			{
				cout << error::node_node_superposition(ensN[i]->getUid(), 
														ensN[j]->getUid());
				exit(-1);
			}
		}
	}
}

bool Noeud::operator==(const Noeud& nd) const 
{
	return (getX() == nd.getX() and getY() == nd.getY() and
			getRayon() == nd.getRayon() and uid == nd.uid and type == nd.type); 
}

