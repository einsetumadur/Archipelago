/**\
 * \name   tools.cc
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include <iostream>
#include <math.h>
#include "tools.h"
#include "constantes.h"

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
	Vecteur c = {k*v.x, k*v.y};
		
	return c;
}

Vecteur projection(Vecteur a, Vecteur b) 
{
	double k = (prod_scal(a,b)/prod_scal(b,b));
	
	return	scalaire_vecteur(k, b);
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

bool collision_cercle(Cercle c, Cercle e) 
{
    Vecteur a = {c.centre.pos_x, c.centre.pos_y};
    Vecteur b = {e.centre.pos_x, e.centre.pos_y};
    double dist = distance_vecteurs(a, b);
    double somme_rayon = (c.rayon + e.rayon);
    
    return ( dist <= (somme_rayon) );
}

bool collision_droite_cercle(Cercle c, Seg_droite d) 
{
	Vecteur v = vecteur_orthogonal(c.centre, d);
	Vecteur a = {c.centre.pos_x - d.debut.pos_x, c.centre.pos_y - d.debut.pos_y};

	return  (norme(v) <= (c.rayon) and 
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
