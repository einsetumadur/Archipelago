#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
using namespace std;

class Noeud{
    public:
        Noeud(unsigned int id,double x, double y, unsigned int capacite): uid(id),nbp(capacite),posx(x),posy(y){}
        unsigned int capacite(){return nbp;}
        string print(){return to_string(uid)+" "+to_string(posx)+" "+to_string(posy)+" "+to_string(nbp);}
    private: 
        unsigned int uid;
        unsigned int nbp;
        double posx,posy;
};  

class Lien{
    public:
        Lien(int a,int b)
        {
          no1 = a;
          no2 = b;
        }
    private:
        int no1;
        int no2;
};

class Ville{
  public:
    Ville(char* file){
        cout<<"entrée constructeur"<<endl;
        chargement(file);
        sauvegarde(file);
        }

  private:
    void decodage(string line);
    void chargement(char * nom_fichier);
    void sauvegarde(string file);
    //void verification();

    string fichier;
    vector<Noeud> logement;
    vector<Noeud> production;
    vector<Noeud> transport;
    vector<Lien> ponts;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
    cout<<"départ"<<endl;
    Ville Lausanne(argv[1]);
    cout<<"fin de programme"<<endl;
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// traite le fichier ligne par ligne.  
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
       
			decodage_ligne(line);
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
      logement.push_back(Noeud(numid,posx,posy,popmax)); 
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
      production.push_back(Noeud(numid,posx,posy,popmax));
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
      transport.push_back(Noeud(numid,posx,posy,popmax));
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
		if( !(data >> posx >> posy)) cout<<"err liens"<<endl;
    else{
      //ponts.push_back(Lien(posx,posy)); 
      ++i;
    }
		if(i == total) etat=FIN ;
	  break;

	case FIN: cout<<"erreur format"<<endl; 
		break;

	default: cout<<"err defaultswitch"<<endl;
	}	
}

void Ville::sauvegarde(string file){
  fstream fichier;
  fichier.open("sauvegarde.txt", ios::out | ios::trunc);

if(!fichier.is_open()){
    std::cout<<"impossible d'enregistrer "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
    fichier<<"#fichier de sauvegarde Archipelago de "<<file<<endl;
    fichier<<logement.size()<<endl;
    for (auto noeud : logement)
    {
      fichier<<"\t"<<noeud.print()<<endl;
    }
    fichier<<production.size()<<endl;
    for (auto noeud : production)
    {
      fichier<<"\t"<<noeud.print()<<endl;
    }
    fichier<<transport.size()<<endl;
    for (auto noeud : transport)
    {
      fichier<<"\t"<<noeud.print()<<endl;
    }
  /*fichier<<ponts.size()<<endl;
    for (auto lien : ponts)
    {
      fichier<<lien.print()<<endl;
    } */
  }

  fichier.close();
}