#include<iostream>
#include<math.h>
#include"tools.h"

double distance_vecteurs(Vecteur a, Vecteur b) 
{
    Vecteur v = {a.x-b.x, a.y-b.y};
	
    return norme(v);
}

bool collision_cercle(Cercle c, Cercle e, double dist_min) 
{
    Vecteur a_1 = {c.centre.pos_x, c.centre.pos_y};
    Vecteur b_1 = {e.centre.pos_x, e.centre.pos_y};
    double dist = distance_vecteurs(a_1, b_1);
    double somme = (c.rayon + e.rayon);
    
    return ( dist <= (somme + dist_min) );
}

double prod_scal(Vecteur a, Vecteur b) 
{
	
  return a.x*b.x + a.y*b.y;
}

double norme(Vecteur a) {
	
  return a.x*a.x + a.y*a.y;
}

Vecteur projection(Vecteur a, Vecteur b) 
{
  double k = prod_scal(a,b)/prod_scal(b,b);
  Vecteur v = { k*b.x, k*b.y };
	
  return v;
}

Vecteur vecteur_orthogonal(Point p, Seg_droite d)                     // créé un vecteur v_2 orthogonal à d, passant par p
{
	Vecteur v_1 = { d.debut.pos_x - p.pos_x, d.debut.pos_y - p.pos_y };
	Vecteur proj = projection(v_1, d.directeur);
	Vecteur v_2 = { v_1.x - proj.x, v_1.y - proj.y };
	
	return v_2;
}

bool collision_droite_cercle(Cercle c, Seg_droite d, double dist_min) 
{
  Vecteur v = vecteur_orthogonal(c.centre, d);
  Vecteur a = { d.debut.pos_x - c.centre.pos_x, d.debut.pos_y - c.centre.pos_y };
  Vecteur b = { d.debut.pos_x + d.directeur.x - c.centre.pos_x, d.debut.pos_y + d.directeur.y - c.centre.pos_y };
  double v_a = prod_scal(v,a);
  double v_b = prod_scal(v,b);

	return ( norme(v) <= (c.rayon + dist_min) && ( (v_a > 0 && v_b < 0) || (v_a < 0 && v_b > 0) ) );
}
