#include<iostream>
#include<math.h>
#include"tools.h"

double distance(Vecteur a, Vecteur b) {
    double diffx = a.x - b.x;
    double diffy = a.y - b.y;
	
    return sqrt(diffx*diffx + diffy*diffy) ;
}

bool collision_cercle(Cercle c, Cercle e, double dist_min) {
    Vecteur a_1 = {c.centre.pos_x, e.centre.pos_y};
    Vecteur b_1 = {c.centre.pos_x, e.centre.pos_y};
    double dist = distance(a_1, b_1);
    double somme = (c.rayon + e.rayon);
    
    return ( dist <= (somme + dist_min) );
}

double prod_scal(Vecteur a, Vecteur b) {
	
  return a.x*b.x + a.y*b.y;
}

Vecteur projection(Vecteur a, Vecteur b) {
  double k = prod_scal(a,b)/prod_scal(b,b);
  Vecteur res = { k*b.x, k*b.y };
	
  return res;
}

double norme(Vecteur a) {
  return sqrt(a.x*a.x + a.y*a.y);
}

double dist_point_ligne(Point p, Seg_droite d) {
	Vecteur test = { d.debut.pos_x - p.pos_x, d.debut.pos_y - p.pos_y };
	Vecteur proj = projection(test, d.directeur);
	Vecteur res = { test.x - proj.x, test.y - proj.y };
	
	return norme(res);
}

bool collision_droite_cercle(Cercle c, Seg_droite d, double dist_min) {
	
	return (dist_point_ligne(c.centre, d) <= (c.rayon + dist_min) );
}
