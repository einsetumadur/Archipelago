#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"ville.h"
using namespace std;


Ville::Ville(string file){
  fichier = file;
  //besoin de construire les tableaux avant chargement ?
  chargement(file);
}

Ville::~Ville(){
  sauvegarde(fichier);
}

double ENJ(){
  double sumL(0),sumTP(0);

  for(auto noeud : logement){    // pourquoi il arrive pas a trouver Ville.logement ?
    sumL =+ noeud.nbp();
  }
  for(auto noeud : production){
    sumTP =+ noeud.nbp();
  }
  for(auto noeud : transport){
    sumTP =+ noeud.nbp();
  }

return (sumL - sumTP)/(sumL + sumTP);
}

double CI(){

return -1;
}

double MTA(){

return -1;
}

void Ville::chargement(string file){
  fstream fichier;
  fichier.open(file, ios::out);
  if(!fichier.is_open()){
    std::cout<<"impossible d'ouvrir "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
    string ligne;
    unsigned int nbr =0;
    size_t compte =0 ;
    while(getline(fichier,ligne)){ //peut etre une mauvaise idée pour prendre des donées une par une.
      if(ligne[0] == *"#"){break;}
      else{
        fichier>>nbr;
        //logement.size(nbr) par exemple.
        compte++;
      }
      std::cout<<ligne<<std::endl;
    }
    
  }

  fichier.close();
}

void Ville::sauvegarde(string file){
  fstream fichier;
  fichier.open(file, ios::in | ios::trunc);

if(!fichier.is_open()){
    std::cout<<"impossible d'enregistrer "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
    fichier<<"#fichier de sauvegarde Archipelago de "<<file<<endl;
    fichier<<logement.size()<<endl;
    for (auto noeud : logement)
    {
      fichier<<noeud.print()<<endl;
    }
    fichier<<production.size()<<endl;
    for (auto noeud : production)
    {
      fichier<<noeud.print()<<endl;
    }
    fichier<<transport.size()<<endl;
    for (auto noeud : transport)
    {
      fichier<<noeud.print()<<endl;
    }
    fichier<<ponts.size()<<endl;
    for (auto lien : ponts)
    {
      fichier<<lien.print()<<endl;
    }
  }

  fichier.close();
}

void Ville::verification(){

}
