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
    test_nbp(nbp);
    
			
}
    
void Noeud::test_max_link() 
{
	if(liens.size() > max_link)			
	{			
		cout << error::max_link( getUid() );
		exit(0);
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

void Noeud::add_lien(Noeud* B, vector<Noeud*> ensN) 
{
	// SELF-NODE
	if( (*B) == *this )
	{
	 	cout << error::self_link_node((*B).getUid()); 
	 	exit(0);
	}

	// MULTIPLE NODE
	for(size_t i(0) ; i < liens.size() ; ++i) 
	{
		if(liens[i] == B) 
		{
			cout << error::multiple_same_link(uid, (*B).getUid());
			exit(0);
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
	
	// COLLISION LIENS-QUARTIER
	for(size_t i(0) ; i < ensN.size() ; ++i) 
	{
		if( triangle( (*this).getQuartier(), (*B).getQuartier(), (*ensN[i]).getQuartier() ) )
		{
			test_lien_quartier(this, B, ensN[i]);
		}
	}
	
	liens.push_back(B);
}

void test_nbp(unsigned int nbp) 
{
 
	if(nbp < min_capacity) 
	{
		cout << error::too_little_capacity(nbp);
		exit(0);
	}
	else if(nbp > max_capacity)
	{
		cout << error::too_much_capacity(nbp);
		exit(0);
	}
}

void test_lien_quartier(Noeud* A, Noeud* B, Noeud* C) 
{	
	Point p = { (*A).getx(), (*A).gety() };
	Seg_droite d = { p, {(*B).getx() - (*A).getx(), (*B).gety() - (*A).gety()} }; // AB
	if(collision_droite_cercle((*C).getQuartier(), d))
	{
		cout << error::node_link_superposition((*B).getUid());
		exit(0);
	}

} 

void test_coll_quartier(vector<Noeud*> ensN) 
{		
	for(size_t i(0) ; i < ensN.size() - 1 ; ++i) 
	{
		if(collision_cercle( (*(ensN[i])).getQuartier(), (*(ensN[i+1])).getQuartier()) )
		{
			cout << error::node_node_superposition( (*(ensN[i])).getUid(), (*(ensN[i+1])).getUid() );
			exit(0);
		}
	}
}

bool Noeud::operator==(const Noeud& nd) const 
{
	
	return ( (getx() == nd.getx()) and (gety() == nd.gety()) and
													(getrayon() == nd.getrayon()) ); 
}

