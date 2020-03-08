#include<iostream>

#ifndef TOOLS_H
#define TOOLS_H

struct Point {
  double pos_x, pos_y;
};

struct Cercle {
  Point centre;
  double rayon;
}; 

struct Vecteur {
  double x, y;
};

struct Seg_droite {
  Point debut;
  Vecteur directeur;
};

double distance_vecteurs(Vecteur a, Vecteur b);
bool collision_cercle(Cercle c, Cercle e, double dist_min = 0);
double prod_scal(Vecteur a, Vecteur b);
Vecteur projection(Vecteur a, Vecteur b);
double dist_point_ligne(Point p, Seg_droite d);
bool collision_droite_cercle(Cercle c, Seg_droite d, double dist_min = 0);
double norme(Vecteur a);

#endif 
