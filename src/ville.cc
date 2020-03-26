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
	Ville ville;
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
	cout << error::success() << endl;
	}
	else cout << "unable to open file." << endl;
}

void Ville::decodage(string line)
{
	istringstream data(line);

	enum Etat_lecture {NBL,LOGE,NBP,PROD,NBT,TRAN,NBLI,LIENS,FIN};

	static int etat(NBL); 
	static int i(0);
	static int total(0);
	unsigned int uid1;
	unsigned int uid2;

	switch(etat) 
	{
		case NBL: 
			if(!(data >> total)) cout << "wrong input format" << endl; 
			else i = 0 ;
			if(total==0) etat=NBT; 
			else etat = LOGE ; 
			break;

		case LOGE: 	
			ajout_noeud(data,i,LOGEMENT);
			if(i == total) etat = NBT ;
			break;

		case NBP: 
			if(!(data >> total)) cout << "wrong input format" << endl; 
			else i = 0;
			if(total == 0) etat = NBLI; 
			else etat = PROD ; 
			break;

		case PROD: 
			ajout_noeud(data, i, PRODUCTION);
			if(i == total) etat = NBLI ; 
			break;

		case NBT: 
			if(!(data >> total)) cout << "wrong input format" << endl; 
			else i = 0;
			if(total==0) etat=NBP; 
			else etat = TRAN ; 
			break;

		case TRAN: 
			ajout_noeud(data, i, TRANSPORT);
			if(i == total) etat = NBP ; 
			break;

		case NBLI: 
			if(!(data >> total)) cout << "wrong input format" << endl; 
			else i = 0;
			if(total == 0) etat = FIN; 
			else etat = LIENS ; 
			break;

		case LIENS: 
			if( !(data >> uid1 >> uid2)) cout << "wrong input format" << endl; 
			else
			{
				creation_lien(uid1, uid2);
				++i;
			}
			if(i == total) etat = FIN ; 
			break;

		case FIN:  
		break;

		default: cout << "defaultswitch error." << endl;
		break;
	}
}

void Ville::ajout_noeud(istringstream& param,int& counter,Type_quartier type)
{
	unsigned int numid;
	unsigned int popmax;
	double posx;
	double posy;

	if( !(param >> numid >> posx >> posy >> popmax))	
		cout << "wrong input format" << endl;
	else
	{
		quartiers.push_back(new Noeud(numid,posx,posy,popmax,type));
		redondance_uid();
		collis_noeuds();
		++counter;
	}
}

string Ville::print_type(Type_quartier type)
{
	string bloc(to_string(nb_type(type)));
	bloc[0] = nb_type(type);
	for(auto noeud : quartiers)
	{
		if(noeud->getType()==type) bloc.append("\t" + noeud->print() + "\n");
	}
	return bloc;
}

unsigned int Ville::nb_type(Type_quartier type)
{
	unsigned int count(0);
	for(auto noeud : quartiers)
	{
		if(noeud->getType()==type) count++;
	}
	return count;
}

void Ville::creation_lien(unsigned int uid_a, unsigned int uid_b) 
{
	if(uid_a == uid_b)
	{
		cout << error::self_link_node(uid_a) << endl;
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

	if (a->test_max_link())
	{
		cout << error::max_link(uid_a);
		exit(-1);
	}
	if (b->test_max_link())
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

Noeud* Ville::trouve_lien(unsigned int uid)
{
	size_t sizetab(quartiers.size());

	for(size_t i(0) ; i <  sizetab ; ++i)
	{
		if(uid == quartiers[i]->getUid()) return quartiers[i];
	}

	cout << error::link_vacuum(uid) << endl;
	exit(-1);
}

void Ville::redondance_uid()
{
	size_t sizetab(quartiers.size());

	for (size_t i = 0; i < sizetab ; i++) 
	{
		for (size_t j = i+1; j < sizetab ; j++)
		{
			if(quartiers[i]->getUid() == quartiers[j]->getUid())
			{
				cout << error::identical_uid(quartiers[i]->getUid()) << endl;
				exit(0);
			}
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
