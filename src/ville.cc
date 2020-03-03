#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include"ville.h"


Ville::Ville(string file){
  fichier = file;
  //besoin de construire les tableaux avant chargement ?
  chargement(file);
}

Ville::~Ville(){
  sauvegarde(fichier);
}

void Ville::chargement(string file){
  fstream fichier;
  fichier.open(file, ios::out);
  if(!fichier.is_open()){
    std::cout<<"impossible d'ouvrir "<<file<<std::endl;
    //todo: mettre fonction d'erreure.
  }else{
    string ligne;
    while(getline(fichier,ligne)){ //peut etre une mauvaise idée pour prendre des donées une par une.
      std::cout<<ligne<<std::endl;
    }
    /*
    while(!eof){
    
    }
    */
  }
  string ligne;
  fichier>>ligne

  fichier.close();
}

void Ville::sauvegarde(string file){
  fstream fichier;
  fichier.open(file, ios::in | ios::trunc);

  fichier.close();
}

void Ville::verification(){

}
