/**\
 * \name    ville.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version Rendu 2
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
	Ville ville(true);
}

Ville::Ville(bool val): 
			 chargement_verif(val), nbp_nbL(0), nbp_nbP(0), nbp_nbT(0)
{
}

Ville::~Ville()
{
	for(auto noeud : quartiers) {
		delete noeud;
	}
}

Ville* Ville::getVille()
{
	return &ville;
}

// chargement du fichier :

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
			if(line[0] == '#')  continue;  
			decodage(line);
		}
		cout << "MTA : " << mta() << endl;
		cout << "ENJ : " << enj() << endl;
		cout << "CI : " << ci() << endl;
		cout << error::success();
	}
	else 	
	{
		cout << "unable to open file." << endl;
		chargement_verif = false;
	}
}

// sauvegarde du fichier : 
void Ville::sauvegarde(string file) const
{
	fstream fichier;
	fichier.open(file, ios::out | ios::trunc);
	
	if(!fichier.is_open())	cout << "unable to save file" << file << endl;
	else
	{
		fichier << print_type(logement) << endl;
		fichier << print_type(transport) << endl;
		fichier << print_type(production) << endl;
	}
	fichier.close();
}

string Ville::print_type(string type) const
{
	string bloc(to_string(nb_type(type)));
	bloc[0] = nb_type(type);
	for(auto noeud : quartiers)
	{
		if(noeud->getType()==type) bloc.append("\t" + noeud->print() + "\n");
	}
	
	return bloc;
}

unsigned int Ville::nb_type(string type) const
{
	unsigned int count(0);
	for(auto noeud : quartiers)
	{
		if(noeud->getType() == type)	count++;
	}
	
	return count;
}

bool Ville::get_chargement_verif()
{
	return chargement_verif;
}

double Ville::mta()
{
	unsigned int nbL = nb_type(logement);
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

// lecture du fichier : 

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
			else 					i = 0 ;
			if(total == 0)			etat = NBT; 
			else 					etat = LOGE ; 
			break;

		case LOGE: 	
			ajout_noeud(data,i, LOGE);
			if(i == total)			etat = NBT ;
			break;

		case NBP: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else 	i = 0;
			if(total == 0)			etat = NBLI; 
			else 					etat = PROD ; 
			break;

		case PROD: 
			ajout_noeud(data, i, PROD);
			if(i == total) 			etat = NBLI ; 
			break;

		case NBT: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0;
			if(total==0)			etat = NBP; 
			else 					etat = TRAN ; 
			break;

		case TRAN: 
			ajout_noeud(data, i, TRAN);
			if(i == total)			etat = NBP; 
			break;

		case NBLI: 
			if(!(data >> total))	cout << "wrong input format" << endl; 
			else i = 0;
			if(total == 0)			etat = FIN; 
			else etat = LIENS ; 
			break;

		case LIENS: 
			if(!(data >> uid1 >> uid2)) 	cout << "wrong input format" << endl; 
			else
			{
				creation_lien(uid1, uid2);
				++i;
			}
			if(i == total)			etat = FIN ; 
			break;

		case FIN:  
			break;

		default:	
			cout << "defaultswitch error." << endl;
			break;
	}
}

void Ville::update_chargement(bool val)
{
	chargement_verif = val;
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
				error_noeud(quartiers.back());
				collis_noeuds();
				nbp_nbL += popmax;
				++counter;
				break;
			case(TRAN):
				quartiers.push_back(new Transport(numid,posx,posy,popmax));
				error_noeud(quartiers.back());
				collis_noeuds();
				nbp_nbT += popmax;
				++counter;
				break;
			case(PROD):
				quartiers.push_back(new Production(numid,posx,posy,popmax));
				error_noeud(quartiers.back());
				collis_noeuds();	
				nbp_nbP += popmax;	
				++counter;
				break;
		}
	}
}

void Ville::error_noeud(Noeud* const nd)
{
	if(nd->test_uid())	update_chargement(false);
	if(nd->test_nbp())	update_chargement(false);
	redondance_uid(nd->getUid());
}
	
void Ville::creation_lien(unsigned int uid_a, unsigned int uid_b) 
{
	if(uid_a == uid_b)
	{
		cout << error::self_link_node(uid_a);
		update_chargement(false);
	}
	
	Noeud* a = trouve_lien(uid_a); 
	if(a == nullptr)
	{
		cout << error::link_vacuum(uid_a);
		update_chargement(false);
	}
	Noeud* b = trouve_lien(uid_b);
	if(b == nullptr)
	{
		cout << error::link_vacuum(uid_b);
		update_chargement(false);
	}
	
	error_lien(a, b);
		
	a->ajout_lien(b); 
	b->ajout_lien(a);
}

void Ville::error_lien(Noeud* const a, Noeud* const b)
{
	unsigned int uid_a = a->getUid();
	unsigned int uid_b = b->getUid();
	
	if (a->multiple_link(b))
	{
		cout << error::multiple_same_link(uid_a, uid_b);
		update_chargement(false);
	}

	// Pour les noeuds LOGE :
	if (a->maxi_link())
	{
		cout << error::max_link(uid_a);
		update_chargement(false);
	}
	if (b->maxi_link())
	{
		cout << error::max_link(uid_b);
		update_chargement(false);
	}
	
	for(size_t i(0) ; i < quartiers.size() ; ++i) 
	{
		if(quartiers[i]->collis_lien_quartier(a, b))
		{
			cout << error::node_link_superposition(quartiers[i]->getUid());
			update_chargement(false);
		}	
	}
}

Noeud* Ville::trouve_lien(unsigned int uid) const
{
	for(size_t i(0) ; i <  quartiers.size() ; ++i)
	{
		if(uid == quartiers[i]->getUid())	return quartiers[i];
	}

	return nullptr;
}

void Ville::redondance_uid(unsigned int numid)
{
	for (size_t i(0); i < quartiers.size() - 1 ; i++) 
	{
		if(quartiers[i]->getUid() == numid)
		{
			cout << error::identical_uid(numid);
			update_chargement(false);
		}	
	}
}

void Ville::collis_noeuds()
{		
	for(size_t i(0) ; i < quartiers.size() ; ++i) 
	{
		for(size_t j(i+1) ; j < quartiers.size() ; ++j) 
		{
			if(collision_cercle(quartiers[i]->getBatiment(), 
								quartiers[j]->getBatiment()))
			{
				cout << error::node_node_superposition(quartiers[i]->getUid(), 
													   quartiers[j]->getUid());
				update_chargement(false);
			}
		}
	}
}
