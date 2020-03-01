#include<iostream>
#include"tools.h"

Cercle::Cercle(){}

// dans l'énoncé c'est demandé explicitement de faire le code de tools.cc en struct, et non en class

double Cercle::getx(){
  return coordx;
}

double Cercle::gety(){
  return coordy;
}

double Cercle::getRayon(){
  return rayon;
}
