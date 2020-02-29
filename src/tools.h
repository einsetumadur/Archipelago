#include<iostream>

#ifndef TOOLS_H
#define TOOLS_H

class Cercle { //ou structure ? a reflechir
public:
  Cercle();
  double getx();
  double gety();
  double getRayon();
private:
  double coordx,coordy;
  double rayon;

};

#endif TOOLS_H
