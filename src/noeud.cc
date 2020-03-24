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
    test_nbp();
    if(uid == no_link)
    {
		cout << error::reserved_uid();
		exit(-1);
	}		
}

string Noeud::print()
{
    return to_string(uid)+" "+to_string(getX())+" "+
			to_string(getY())+" "+to_string(nbp);
}
    
unsigned int Noeud::getNbp()
{
	
    return nbp;
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

void Noeud::test_nbp() 
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

void Noeud::ajout_lien(Noeud* b) 
{
	liens.push_back(b);
}

bool Noeud::multiple_link(Noeud* b) 
{
	for(size_t i(0) ; i < liens.size() ; ++i) 
	{
		if(liens[i] == b)	return true;
	}
	
	return false;

}

bool Noeud::test_max_link() 
{
	if(type == LOGEMENT and liens.size() == max_link)		return true;
	else 													return false;
}

bool Noeud::test_lien_quartier(Noeud* lien_a, Noeud* lien_b) 
{	
	if( not(this == lien_a) and not(this == lien_b) ) 
	{
		if(champ(lien_a->batiment, lien_b->batiment, this->batiment)) 
		{
			Point p = { lien_a->getX(), lien_a->getY() };
			Seg_droite d = { p, {lien_b->getX() - lien_a->getX(), 
								lien_b->getY() - lien_a->getY()}};
			if(collision_droite_cercle(this->batiment, d))	return true;
		}
	}
	
	return false;
} 

bool Noeud::operator==(const Noeud& nd) const 
{
	return (getX() == nd.getX() and getY() == nd.getY() and
			getRayon() == nd.getRayon() and uid == nd.uid and type == nd.type); 
}
