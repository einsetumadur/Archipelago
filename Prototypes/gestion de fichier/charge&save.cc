#include<iostream>
#include<fstream>
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

vector<string> parse(string str){
  vector<string> words;
  str.shrink_to_fit();
  str.resize(str.find_first_of("#") - 1);
  if(str[0] ==' '|| str[0] =='\n'||str[0] =='\t'||str[0] =='\r')
  {
    while(str.empty())
  }else{

  }
  
  
  str.find(' '||'\n'||'\t'||'\r')

}
class Ville{
  public:
    Ville(string file){
        cout<<"entrée constructeur"<<endl;
        chargement(file);
        sauvegarde(file);
        }
    //~Ville();
    //double ENJ();
    //double CI();
    //double MTA();

  private:
    void chargement(string file);
    void sauvegarde(string file);
    //void verification();

    string fichier;
    vector<Noeud> logement;
    vector<Noeud> production;
    vector<Noeud> transport;
    //vector<Lien> ponts;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){
    cout<<"départ"<<endl;
    Ville Lausanne(argv[1]);
    cout<<"fin de programme"<<endl;
    return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Ville::chargement(string file){
  fstream fichier;
  fichier.open(file, ios::in);
  if(!fichier.is_open()){
    std::cout<<"impossible d'ouvrir "<<file<<std::endl;
  }else{
      cout<<"fichier ouvert sans problème "<<file<<endl;
    string ligne;
    size_t compte =0 ;
    while(getline(fichier,ligne)){ //peut etre une mauvaise idée pour prendre des donées une par une.
      if(ligne[0] == '#' || ligne[0]=='\n' || ligne[0]=='\t'){}
      else{
          int newsize,uid,cap;
          double x,y;
        fichier>>newsize;
        cout<<newsize<<endl;
        switch (compte)
        {
        case 0:
            for (size_t i = 0; i < newsize; i++)
            {
                fichier>>uid;
                fichier>>x;
                fichier>>y;
                fichier>>cap;
                Noeud tmp(uid,x,y,cap);
                logement.push_back(tmp);
                cout<<"new logement node"<<endl;
            }
            
            break;
        
        case 1:
            for (size_t i = 0; i < newsize; i++)
            {
                fichier>>uid;
                fichier>>x;
                fichier>>y;
                fichier>>cap;
                Noeud tmp(uid,x,y,cap);
                production.push_back(tmp);
                cout<<"new production node"<<endl;
            }
            
            break;

        case 2:
            for (size_t i = 0; i < newsize; i++)
            {
                fichier>>uid;
                fichier>>x;
                fichier>>y;
                fichier>>cap;
                Noeud tmp(uid,x,y,cap);
                transport.push_back(tmp);
                cout<<"new transport node"<<endl;
            }
            
            break;

        default:
            break;
        }
    
        compte++;
        if(compte >= 100){cout<<"fichier trop grand"<<endl;break;}
      }
    }
    
  }

  fichier.close();
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