/**\
 * \name   tools.cc
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include<iostream>
#include<math.h>
#include"tools.h"
#include"constantes.h"

double distance_vecteurs(Vecteur a, Vecteur b) 
{
    Vecteur v = {a.x-b.x, a.y-b.y};
	
    return norme(v);
}

bool collision_cercle(Cercle c, Cercle e) 
{
    Vecteur a = {c.centre.pos_x, c.centre.pos_y};
    Vecteur b = {e.centre.pos_x, e.centre.pos_y};
    double dist = distance_vecteurs(a, b);
    double somme_rayon = (c.rayon + e.rayon);
    
    return ( dist <= (somme_rayon + dist_min) );
}

double prod_scal(Vecteur a, Vecteur b) 
{
	
  return a.x*b.x + a.y*b.y;
}

double norme(Vecteur a) 
{
	
  return a.x*a.x + a.y*a.y;
}

Vecteur projection(Vecteur a, Vecteur b) 
{
  double k = prod_scal(a,b)/prod_scal(b,b);
  Vecteur v_proj = { k*b.x, k*b.y };
	
  return v_proj;
}

Vecteur vecteur_orthogonal(Point p, Seg_droite d) 
{
	Vecteur v_point_droite = { p.pos_x - d.debut.pos_x, p.pos_y - d.debut.pos_y };
	Vecteur v_proj = projection(v_point_droite, d.directeur);
	Vecteur v_orthog = { v_point_droite.x - v_proj.x, v_point_droite.y - v_proj.y };
	
	return v_orthog;
}

bool collision_droite_cercle(Cercle c, Seg_droite d) 
{
	Vecteur v = vecteur_orthogonal(c.centre, d);
	Vecteur a = { d.debut.pos_x - c.centre.pos_x, d.debut.pos_y - c.centre.pos_y };
	Vecteur b = { d.debut.pos_x + d.directeur.x - c.centre.pos_x, d.debut.pos_y 
													+ d.directeur.y - c.centre.pos_y };
	double v_a = prod_scal(v,a);
	double v_b = prod_scal(v,b);

	return ( norme(v) <= (c.rayon + dist_min) and ( (v_a > 0 and v_b < 0)
													or (v_a < 0 and v_b > 0) ) );
}
