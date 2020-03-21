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
            uid(id), quartier{ {x,y}, sqrt(capacite)}, nbp(capacite), type(type)
{
    if(not(test_nbp(uid, getrayon())) 
    {
		switch(tmp) 
		{
		case 0 : error::too_little_capacity(nbp);
				break;
		case 2 : error::too_much_capacity(nbp);
				break;
		}	
	}
}
    
string Noeud::test_max_link() 
{
	if(liens.size() > max_link)			
	{			
		error::max_link( getUid() );
	}
}
unsigned int Noeud::get_nbp()
{
    return nbp;
}

string Noeud::print()
{
    return to_string(uid)+" "+to_string(quartier.centre.pos_x)+" "+
								to_string(quartier.centre.pos_y)+" "+to_string(nbp);
}

double Noeud::getx() const 
{
	
	return (getQuartier() ).centre.pos_x;
}
double Noeud::gety() const 
{
	
	return (getQuartier() ).centre.pos_y;
}
double Noeud::getrayon() const 
{
	
	return (getQuartier() ).rayon;
}
Cercle Noeud::getQuartier() const  
{
	
	return quartier;
}
unsigned int Noeud::getUid() const 
{
	
	return uid;
}
Type_quartier Noeud::getType() const 
{

	return type;
}

void Noeud::add_lien(Noeud* B) 
{
	// SELF-NODE
	if( (*B) == *this )
	{
	 	error::self_link_node((*B).getUid()); 
	}

	// MULTIPLE NODE
	for(size_t i(0) ; i < liens.size() ; ++i) 
	{
		if(liens[i] == B) 
		{
			error::multiple_same_link(uid, (*B).getUid());
		}
	}

	// MAX HOUSING LINKS
	if(type == 2)
	{
		test_max_link();
	}
	else if ((*B).getType() == 2) 
	{
	 	(*B).test_max_link();
	}
	
	liens.push_back(B);
}

int test_nbp(unsigned int nbp, double rayon) 
{
	unsigned int tmp(0);
	if(sqrt(nbp) == rayon)
	{
		tmp = 1;
		return tmp;
	}
	else if(sqrt(nbp) < rayon)
	{
		tmp = 0;
		return tmp;
	}
	else if(sqrt(nbp) > rayon)
	{
		tmp = 2;
		return tmp;
	}
}

bool test_lien_quartier(Noeud A, Noeud C, Noeud B) 
{	
	Point p = { A.getx(), A.gety() };
	Seg_droite d = { p, {B.getx() - A.getx(), B.gety() - A.gety()} };
	return (collision_droite_cercle(C.getQuartier(), d));
} 

bool test_coll_quartier(vector<Noeud*> ensN) 
{		
	for(size_t i(0) ; i < ensN.size() - 1 ; ++i) 
	{
		return (collision_cercle(ensN[i].getQuartier(), ensN[i+1].getQuartier()) );
	}
}

bool Noeud::operator==(const Noeud& nd) const 
{
	
	return ( (getx() == nd.getx()) and (gety() == nd.gety()) and
													(getrayon() == nd.getrayon()) ); 
}


