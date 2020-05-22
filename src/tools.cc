/**\
 * \name    tools.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 3 - Architecture 11 b1
 */

#include <iostream>
#include <math.h>
#include "tools.h"
#include "graphics.h"

double prod_scal(Vecteur a, Vecteur b) 
{
	return a.x*b.x + a.y*b.y;
}

double norme(Vecteur a) 
{
	return sqrt(prod_scal(a,a));
}

Vecteur scalaire_vecteur(double k, Vecteur v) 
{		
	return {k*v.x, k*v.y};
}

Vecteur projection(Vecteur a, Vecteur b) 
{
	return	scalaire_vecteur(prod_scal(a,b)/prod_scal(b,b), b);
}

Vecteur vecteur_orthogonal(Point p, Seg_droite d) 
{
	Vecteur v_point_droite = { p.pos_x - d.debut.pos_x, p.pos_y - d.debut.pos_y };
	Vecteur v_proj = projection(v_point_droite, d.directeur);
	Vecteur v_orthog = { v_point_droite.x - v_proj.x, v_point_droite.y - v_proj.y };
	
	return v_orthog;
}

double distance_vecteurs(Vecteur a, Vecteur b) 
{
    Vecteur v = {a.x-b.x, a.y-b.y};
	
    return norme(v);
}

bool collision_cercle(Cercle c, Cercle e, double dist_min) 
{
    Vecteur a = {c.centre.pos_x, c.centre.pos_y};
    Vecteur b = {e.centre.pos_x, e.centre.pos_y};
    double dist = distance_vecteurs(a, b);
    double somme_rayon = (c.rayon + e.rayon);
    
    return (dist <= ((somme_rayon) + dist_min));
}

bool collision_droite_cercle(Cercle c, Seg_droite d, double dist_min) 
{
	Vecteur v = vecteur_orthogonal(c.centre, d);
	Vecteur a = {c.centre.pos_x - d.debut.pos_x, c.centre.pos_y - d.debut.pos_y};

	return  (norme(v) <= ((c.rayon) + dist_min) and 
			(norme(projection(a, d.directeur)) <= norme(d.directeur)));
}

bool champ(Cercle a, Cercle b, Cercle c) 
{
	Point p = { a.centre.pos_x, a.centre.pos_y };
	Seg_droite d = { p, {b.centre.pos_x - a.centre.pos_x, 
						 b.centre.pos_y - a.centre.pos_y} }; 
	Vecteur ac = { c.centre.pos_x - a.centre.pos_x, 
				   c.centre.pos_y - a.centre.pos_y };
	Vecteur bc = { c.centre.pos_x - b.centre.pos_x, 
				   c.centre.pos_y - b.centre.pos_y };
	
	return (prod_scal(d.directeur, ac) >= 0 and 
			prod_scal(bc, scalaire_vecteur(-1, d.directeur)) >= 0);
}

bool is_in_circle(double x, double y, Cercle circle)
{
	double dist_to_center = sqrt((x - circle.centre.pos_x)*(x - circle.centre.pos_x)+
								 (y - circle.centre.pos_y)*(y - circle.centre.pos_y));
	if(dist_to_center <= circle.rayon)  return true;
	else  return false;
}

void draw_ligne(Point c_a, Point c_b, Couleur paint)
{
	ligne(c_a.pos_x, c_a.pos_y, c_b.pos_x, c_b.pos_y, paint);
}

void draw_cercle(Point c, int rayon, Couleur paint)
{
	cercle(c.pos_x, c.pos_y, rayon, paint);
}

void draw_rectangle(Point c, double rayon, Couleur paint)
{
	rectangle(c.pos_x, c.pos_y, rayon, paint);
}

