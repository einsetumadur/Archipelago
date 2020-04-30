/**\
 * \name    graphics.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */

#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "graphic_color.h"

void ligne(double a_x, double a_y, double b_x, double b_y, Couleur paint);
void cercle(double c_x, double c_y, double rayon, Couleur paint);
void rectangle(double c_x, double c_y, double rayon, Couleur paint);
void choix_couleur(Couleur paint);

#endif 



