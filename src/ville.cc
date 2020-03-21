#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include"ville.h"
#include"noeud.h"
#include"error.h"
#include"constantes.h"
using namespace std;


Ville::Ville(char* file){
  fichier = file;
  chargement(file);
}

Ville::~Ville(){
  sauvegarde(fichier);
  delete &quartiers;
  delete &fichier;
}

double Ville::ENJ(){


return -1;
}

double Ville::CI(){

return -1;
}

double Ville::MTA(){

return -1;
}

void Ville::chargement( char * nom_fichier)
{
  string line;
  ifstream fichier(nom_fichier); 
  if(!fichier.fail()) 
  {
    // l’appel de getline filtre aussi les séparateurs
    while(getline(fichier >> ws,line)) 
      {
		    // ligne de commentaire à ignorer, on passe à la suivante
		    if(line[0]=='#')  continue;  
        decodage(line);
      }
    cout << "fin de la lecture" << endl;
	}
	else cout<<"erreur lectur fichier"<<endl;
}

void Ville::decodage(string line)
{
	istringstream data(line);
				 
	enum Etat_lecture {NBL,LOGE,NBP,PROD,NBT,TRAN,NBLI,LIENS,FIN};
  
	static int etat(NBL); // etat de base
	static int i(0), total(0);
	int numid,popmax;
	double posx,posy;
  unsigned int uid1,uid2;

	switch(etat) 
	{
	case NBL: 
		if(!(data >> total)) cout<<"err nblogement"<<endl; 
		else i=0 ;
		if(total==0) etat=NBP; 
    else{ 
      etat=LOGE ; 
      quartiers.reserve(quartiers.size()+total);
    }
	    break;

	case LOGE: 
		if(!(data >> numid >> posx >> posy >> popmax)) cout<<"err logement"<<endl;
    else{
      quartiers.push_back(new Noeud(numid,posx,posy,popmax,LOGEMENT)); 
      ++i;
    }
		if(i == total) etat=NBP ;
	  break;

	case NBP: 
		if(!(data >> total)) cout<<"err nbproduction"<<endl; 
		else i=0 ;
		if(total==0) etat=NBT; 
    else{
      etat=PROD ; 
      quartiers.reserve(quartiers.size()+total);
    }
	  break;

	case PROD: 
		if( !(data >> numid >> posx >> posy >> popmax)) cout<<"err production"<<endl;
    else{
      quartiers.push_back(new Noeud(numid,posx,posy,popmax,PRODUCTION));
       ++i;
    }  
		if(i == total) etat=NBT ;
	  break;

	case NBT: 
		if(!(data >> total)) cout<<"err nbTransport"<<endl; 
		else i=0;
		if(total==0) etat=NBLI; 
    else{
      etat=TRAN ; 
      quartiers.reserve(quartiers.size()+total);
    }
	  break;

	case TRAN: 
		if( !(data >> numid >> posx >> posy >> popmax)) cout<<"err transport"<<endl;
    else{
      quartiers.push_back(new Noeud(numid,posx,posy,popmax,TRANSPORT));
       ++i;
    }   
		if(i == total) etat=NBLI ;
	  break;

	case NBLI: 
		if(!(data >> total)) {cout<<"err nbLiens"<<endl;} 
		else i=0;
		if(total==0) etat=FIN; else etat=LIENS ; 
	  break;

	case LIENS: 
		if( !(data >> uid1 >> uid2)) cout<<"err liens"<<endl;
    else{
      Noeud* n1 = trouve_lien(uid1);
      Noeud* n2 = trouve_lien(uid2);

      n1->add_lien(n2);
      n2->add_lien(n1);

      ++i;
    }
		if(i == total) etat=FIN ;
	  break;

	case FIN:  
		break;

	default: cout<<"erreur defaultswitch"<<endl;
    break;

  }
}

Noeud* Ville::trouve_lien(unsigned int uid)
{
  for(auto noeud : quartiers)
  {
    if(uid == noeud->getUid())
    {
      return noeud;
    }
  }
  error::link_vacuum(uid);
  return nullptr;               // bonne facon de faire ? 
}

void Ville::sauvegarde(string file){
  fstream fichier;
  fichier.open("sauvegarde.txt", ios::out | ios::trunc);

if(!fichier.is_open()){
    std::cout<<"impossible d'enregistrer "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
  fichier << nb_type(LOGEMENT) << endl;
  fichier << print_type(LOGEMENT) << endl;

  }

  fichier.close();
}

string Ville::print_type(Type_quartier type)
{
  string bloc(to_string(nb_type(type)));

  for(auto noeud : quartiers){
    if(noeud->getType()==type){
      bloc.append("\t" + noeud->print() + "\n");
    }
  }
  return bloc;
}

unsigned int Ville::nb_type(Type_quartier type)
{
  unsigned int count(0);
  for(auto noeud : quartiers){
    if(noeud->getType()==type) count++;
  }
  return count;
}

int Ville::redondance_uid() {		// verifie la non-duplicite des uids
	size_t sizetab = quartiers.size();

  for (size_t i = 0; i < sizetab ; i++) //parcours le tableau
  {
    for (size_t j = i+1; j < sizetab ; j++) //parcour le reste du tableau
    {
      if(quartiers[i] == quartiers[j]) error::identical_uid(quartiers[i]->getUid());
    }
  }
	return 0;
}
