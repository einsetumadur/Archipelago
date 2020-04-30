/**\
 * \name    tools.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */
 
#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include "graphic_color.h"

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

// calculs :
double prod_scal(Vecteur a, Vecteur b);
double norme(Vecteur a);
Vecteur scalaire_vecteur(double k, Vecteur v);
Vecteur projection(Vecteur a, Vecteur b);
Vecteur vecteur_orthogonal(Point p, Seg_droite d);
double distance_vecteurs(Vecteur a, Vecteur b);
bool collision_cercle(Cercle c, Cercle e, double dist_min);
bool collision_droite_cercle(Cercle c, Seg_droite d, double dist_min);
bool champ(Cercle a, Cercle b, Cercle c);
// dessins :
void draw_ligne(Point c_a, Point c_b, Couleur paint);
void draw_rectangle(Point c, double rayon, Couleur paint);
void draw_cercle(Point c, int rayon, Couleur paint);

#endif
