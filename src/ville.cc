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
  //besoin de construire les tableaux avant chargement ?
  chargement(file);
}

Ville::~Ville(){
  sauvegarde(fichier);
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
				 
	enum Etat_lecture {NBL,LOGEMENT,NBP,PRODUCTION,NBT,TRANSPORT,NBLI,LIENS,FIN};
  
	static int etat(NBL); // de base
	static int i(0), total(0);
	int numid,popmax;
	double posx,posy;
  unsigned int uid1,uid2;

	switch(etat) 
	{
	case NBL: 
		if(!(data >> total)) cout<<"err nblogement"<<endl; 
		else i=0 ;
		if(total==0) etat=NBP; else etat=LOGEMENT ; 
	    break;

	case LOGEMENT: 
		if(!(data >> numid >> posx >> posy >> popmax)) cout<<"err logement"<<endl;
    else{
      quartiers.push_back(Noeud(numid,posx,posy,popmax)); 
      ++i;
    }
		if(i == total) etat=NBP ;
	  break;

	case NBP: 
		if(!(data >> total)) cout<<"err nbproduction"<<endl; 
		else i=0 ;
		if(total==0) etat=NBT; else etat=PRODUCTION ; 
	    break;

	case PRODUCTION: 
		if( !(data >> numid >> posx >> posy >> popmax)) cout<<"err production"<<endl;
    else{
      quartiers.push_back(Noeud(numid,posx,posy,popmax));
       ++i;
    }  
		if(i == total) etat=NBT ;
	  break;

	case NBT: 
		if(!(data >> total)) cout<<"err nbTransport"<<endl; 
		else i=0;
		if(total==0) etat=NBLI; else etat=TRANSPORT ;
	     break;

	case TRANSPORT: 
		if( !(data >> numid >> posx >> posy >> popmax)) cout<<"err transport"<<endl;
    else{
      quartiers.push_back(Noeud(numid,posx,posy,popmax));
       ++i;
    }   
		if(i == total) etat=NBLI ;
	  break;

	case NBLI: 
		if(!(data >> total)) {cout<<"err nbLiens"<<endl;} 
		else i=0;
		if(total==0) etat=FIN; else etat=LIENS ; 
		ponts.resize(total); 
	  break;

	case LIENS: 
		if( !(data >> uid1 >> uid2)) cout<<"err liens"<<endl;
    else{
      ponts.push_back(Lien(trouve_lien(uid1),trouve_lien(uid2))); 
      ++i;
    }
		if(i == total) etat=FIN ;
	  break;

	case FIN: cout<<"erreur format"<<endl; 
		break;

	default: cout<<"err defaultswitch"<<endl;
	}	
}

Noeud* Ville::trouve_lien(unsigned int uid)
{
  for(auto noeud : quartiers)
  {
    if(uid == noeud.getUid()){
      return &noeud;
    }
  }
  cout << "noeud non trouvé pour uid# = "<<uid<<endl;
  exit(0);
}

void Ville::sauvegarde(string file){
  fstream fichier;
  fichier.open("sauvegarde.txt", ios::out | ios::trunc);

if(!fichier.is_open()){
    std::cout<<"impossible d'enregistrer "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
  fichier << nb_type(logements) << endl;
  fichier << print_type(logements) << endl;

  }

  fichier.close();
}

void Ville::print_type(type_quartier type)
{
  for(auto noeud : quartiers){
    if(noeud.getType()==type) 
  }
}

unsigned int Ville::nb_type(type_quartier type)
{
  unsigned int count(0);
  for(auto noeud : quartiers){
    if(noeud.getType()==type) count++;
  }
  return count;
}
