/**\
 * \name   tools.h
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */
 
#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>

struct Point 
{
	double pos_x;
	double pos_y;
};

struct Cercle 
{
	Point centre;
	double rayon;
}; 

struct Vecteur 
{
	double x;
	double y;
};

struct Seg_droite 
{
	Point debut;
	Vecteur directeur;
};

double distance_vecteurs(Vecteur a, Vecteur b);
bool collision_cercle(Cercle c, Cercle e);
double prod_scal(Vecteur a, Vecteur b);
double norme(Vecteur a);
Vecteur projection(Vecteur a, Vecteur b);
Vecteur vecteur_orthogonal(Point p, Seg_droite d);
bool collision_droite_cercle(Cercle c, Seg_droite d);
Vecteur scalaire_vecteur(int k, Vecteur v);
bool champ(Cercle A, Cercle B, Cercle C);

#endif
