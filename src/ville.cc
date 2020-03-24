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


Ville::Ville(char* file)
{
	nom = file;
	chargement(file);
}

void Ville::chargement( char * nom_fichier)
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
	cout<<error::success()<<endl;
	}
	else cout<<"unable to open file."<<endl;
}

void Ville::decodage(string line)
{
	istringstream data(line);

	enum Etat_lecture {NBL,LOGE,NBP,PROD,NBT,TRAN,NBLI,LIENS,FIN};

	static int etat(NBL); // etat de base
	static int i(0), total(0);
	unsigned int uid1,uid2;

	switch(etat) 
	{
		case NBL: 
			if(!(data >> total)) cout<<"wrong input format"<<endl; else i=0 ;
			if(total==0) etat=NBT; else etat=LOGE ; 
			break;

		case LOGE: 	
			ajout_noeud(data,i,LOGEMENT);
			if(i == total) etat=NBT ;
			break;

		case NBP: 
			if(!(data >> total)) cout<<"wrong input format"<<endl; else i=0 ;
			if(total==0) etat=NBLI; else etat=PROD ; 
			break;

		case PROD: 
			ajout_noeud(data,i,PRODUCTION);
			if(i == total) etat=NBLI ; 
			break;

		case NBT: 
			if(!(data >> total)) cout<<"wrong input format"<<endl; else i=0;
			if(total==0) etat=NBP; else etat=TRAN ; 
			break;

		case TRAN: 
			ajout_noeud(data,i,TRANSPORT);
			if(i == total) etat=NBP ; 
			break;

		case NBLI: 
			if(!(data >> total)) cout<<"wrong input format"<<endl; else i=0;
			if(total==0) etat=FIN; else etat=LIENS ; 
			break;

		case LIENS: 
			if( !(data >> uid1 >> uid2)) cout<<"wrong input format"<<endl; else
			{
				if(uid1==uid2)
				{
					cout<<error::self_link_node(uid1)<<endl;
					exit(-1);
				}
			Noeud* n1 = trouve_lien(uid1); 
			Noeud* n2 = trouve_lien(uid2);
			n1->add_lien(n2,quartiers); 
			n2->add_lien(n1,quartiers);
			++i;
			}
			if(i == total) etat=FIN ; 
			break;

		case FIN:  
		break;

		default: cout<<"defaultswitch error."<<endl;
		break;
	}
}

Noeud* Ville::trouve_lien(unsigned int uid)
{
	for(auto noeud : quartiers)
	{
		if(uid == noeud->getUid()) return noeud;
	}
	cout<<error::link_vacuum(uid)<<endl;
	exit(-1);
}

void Ville::sauvegarde(string file)
{
	fstream fichier;
	fichier.open(file, ios::out | ios::trunc);

	if(!fichier.is_open()) std::cout<<"unable to save file"<<file<<std::endl;
	else
	{
		fichier << print_type(LOGEMENT) << endl;
		fichier << print_type(TRANSPORT) << endl;
		fichier << print_type(PRODUCTION) << endl;
	}
	fichier.close();
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

void Ville::redondance_uid() // verifie la non-duplicite des uids
{
	size_t sizetab = quartiers.size();

	for (size_t i = 0; i < sizetab ; i++) //parcours le tableau
	{
		for (size_t j = i+1; j < sizetab ; j++) //parcour le reste du tableau
		{
			if(quartiers[i]->getUid() == quartiers[j]->getUid())
			{
				cout<<error::identical_uid(quartiers[i]->getUid())<<endl;
				exit(0);
			}
		}
	}
}


void Ville::ajout_noeud(istringstream &param,int &counter,Type_quartier type)
{
	unsigned int numid,popmax;
	double posx,posy;

	if( !(param >> numid >> posx >> posy >> popmax))
		cout<<"wrong input format"<<endl;
	else
	{
		quartiers.push_back(new Noeud(numid,posx,posy,popmax,type));
  	redondance_uid();
  	test_coll_quartier(quartiers);
		++counter;
	}
}


