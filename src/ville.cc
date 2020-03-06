#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"ville.h"
#include"error.h"
#include"constantes.h"
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
    sumL =+ noeud.capacite();
  }
  for(auto noeud : production){
    sumTP =+ noeud.capacite();
  }
  for(auto noeud : transport){
    sumTP =+ noeud.capacite();
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

