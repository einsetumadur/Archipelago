/**\
 * \name    ville.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "ville.h"
#include "noeud.h"
#include "error.h"

using namespace std;

namespace
{
	Ville ville(0,0,0);
}

Ville::Ville(unsigned int nbL, unsigned int nbT, unsigned int nbP): 
			 nbL(nbL), nbT(nbT), nbP(nbP), nbp_nbL(0), nbp_nbP(0),
			 nbp_nbT(0)
{
}

Ville::~Ville()
{
	for(auto noeud : quartiers) {
		delete noeud;
	}
}

void main_ville(char* nom_fichier) 
{
	ville.chargement(nom_fichier);
}


void Ville::chargement(char* nom_fichier)
{
	string line;
	ifstream fichier(nom_fichier); 
	if(!fichier.fail()) 
	{
		while(getline(fichier >> ws,line)) 
		{
			if(line[0]=='#')  continue;  
			decodage(line);
		}
		cout << mta() << endl;
		cout << enj() << endl;
		cout << ci() << endl;
		cout << error::success();
	}
	else cout << "unable to open file." << endl;
}


double Ville::mta()
{
	if(quartiers.size() == 0 or nbL == 0)		return 0;
	else 										return short_path(quartiers)/nbL;
}

double Ville::enj()
{
	if(quartiers.size() == 0)	return 0;
	else 						return (nbp_nbL - (nbp_nbT + nbp_nbP))/ 
									   (nbp_nbL + nbp_nbP + nbp_nbT);
}

double Ville::ci()
{
	return cout_infra(quartiers);
}

void Ville::decodage(string line)
{
	istringstream data(line);

	static int etat(NBL); 
	static int i(0);
	static int total(0);
	unsigned int uid1;
	unsigned int uid2;

	switch(etat) 
	{
		case NBL: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0 ;
			if(total==0)	etat = NBT; 
			else etat = LOGE ; 
			break;

		case LOGE: 	
			ajout_noeud(data,i, LOGE);
			if(i == total)	etat = NBT ;
			break;

		case NBP: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0;
			if(total == 0)	etat = NBLI; 
			else etat = PROD ; 
			break;

		case PROD: 
			ajout_noeud(data, i, PROD);
			if(i == total)	etat = NBLI ; 
			break;

		case NBT: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0;
			if(total==0)	etat = NBP; 
			else etat = TRAN ; 
			break;

		case TRAN: 
			ajout_noeud(data, i, TRAN);
			if(i == total)	etat = NBP; 
			break;

		case NBLI: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0;
			if(total == 0)	etat = FIN; 
			else etat = LIENS ; 
			break;

		case LIENS: 
			if(!(data >> uid1 >> uid2)) 	cout << "wrong input format" << endl; 
			else
			{
				creation_lien(uid1, uid2);
				++i;
			}
			if(i == total)	etat = FIN ; 
			break;

		case FIN:  
			break;

		default:	cout << "defaultswitch error." << endl;
			break;
	}
}

void Ville::ajout_noeud(istringstream& param,int& counter, Etat_lecture type)
{
	unsigned int numid;
	unsigned int popmax;
	double posx;
	double posy;

	if( !(param >> numid >> posx >> posy >> popmax))	cout << "wrong input format" 
															 << endl;
	else
	{
		switch(type)
		{
			case(LOGE):
				quartiers.push_back(new Logement(numid,posx,posy,popmax));
				redondance_uid(numid);
				collis_noeuds();
				++nbL;
				nbp_nbL += popmax;
				++counter;
				break;
			case(TRAN):
				quartiers.push_back(new Transport(numid,posx,posy,popmax));
				redondance_uid(numid);
				collis_noeuds();
				++nbT;
				nbp_nbT += popmax;
				++counter;
				break;
			case(PROD):
				quartiers.push_back(new Production(numid,posx,posy,popmax));
				redondance_uid(numid);
				collis_noeuds();	
				++nbP;		
				nbp_nbP += popmax;	
				++counter;
				break;
		}
	}
}
	
void Ville::creation_lien(unsigned int uid_a, unsigned int uid_b) 
{
	if(uid_a == uid_b)
	{
		cout << error::self_link_node(uid_a);
		exit(-1);
	}
	
	Noeud* a = trouve_lien(uid_a); 
	Noeud* b = trouve_lien(uid_b);
	
	error_lien(a, b);
		
	a->ajout_lien(b); 
	b->ajout_lien(a);
}

void Ville::error_lien(Noeud* a, Noeud* b)
{
	unsigned int uid_a = a->getUid();
	unsigned int uid_b = b->getUid();
	
	if (a->multiple_link(b))
	{
		cout << error::multiple_same_link(uid_a, uid_b);
		exit(-1);
	}

	// Pour les noeuds LOGE :
	if (a->maxi_link())
	{
		cout << error::max_link(uid_a);
		exit(-1);
	}
	if (b->maxi_link())
	{
		cout << error::max_link(uid_b);
		exit(-1);
	}
	
	for(size_t i(0) ; i < quartiers.size() ; ++i) 
	{
		if(quartiers[i]->collis_lien_quartier(a, b))
		{
			cout << error::node_link_superposition(quartiers[i]->getUid());
			exit(-1);
		}	
	}
}

Noeud* Ville::trouve_lien(unsigned int uid) const
{
	size_t sizetab(quartiers.size());

	for(size_t i(0) ; i <  sizetab ; ++i)
	{
		if(uid == quartiers[i]->getUid())	return quartiers[i];
	}

	cout << error::link_vacuum(uid);
	exit(-1);
}

void Ville::redondance_uid(unsigned int numid) const
{
	size_t sizetab(quartiers.size());

	for (size_t i(0); i < sizetab - 1 ; i++) 
	{
		if(quartiers[i]->getUid() == numid)
		{
			cout << error::identical_uid(numid);
			exit(0);
		}	
	}
}

void Ville::collis_noeuds() 
{		
	size_t sizetab(quartiers.size());
	
	for(size_t i(0) ; i < sizetab ; ++i) 
	{
		for(size_t j(i+1) ; j < sizetab ; ++j) 
		{
			if(collision_cercle(quartiers[i]->getBatiment(), 
								quartiers[j]->getBatiment()))
			{
				cout << error::node_node_superposition(quartiers[i]->getUid(), 
													quartiers[j]->getUid());
				exit(-1);
			}
		}
	}
}
