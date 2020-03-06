#include<iostream>

#ifndef TOOLS_H
#define TOOLS_H

typedef double vect[2] ;

struct cercle{
  double posx,posy;
  double rayon;
}; 

double distance(vect a, vect b);
double prod_scal(vect a, vect b);
void projection(vect &res, vect a, vect b);
double norme(vect a);
double dist_point_ligne(vect a,vect b, vect c);

#endif TOOLS_H
