/**\
 * \name   graphics.cc
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include "graphic_gui.h"
#include <iostream>
using namespace std;

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	static bool init(false);

	if(!init)
	{
		ptcr = &cr;
		init = true;
	}
}

void choix_couleur(Couleur paint)
{
	switch(paint)
	{
		case(NOIR):
			(*ptcr)->set_source_rgb(0.0, 0.0, 0.0);
			break;
		case(ROUGE):
			(*ptcr)->set_source_rgb(1.0, 0.0, 0.0);
			break;
		case(VERT):
			(*ptcr)->set_source_rgb(0.0, 1.0, 0.0);
			break;
		default: 
			break;
	}
}
void ligne(double a_x, double a_y, double b_x, double b_y, Couleur paint)
{	
	(*ptcr)->set_line_width(epaisseur_trait);
	choix_couleur(paint);
	(*ptcr)->move_to(a_x, a_y);
	(*ptcr)->line_to(b_x,b_y);
	(*ptcr)->stroke();
}

void cercle(double c_x, double c_y, double rayon, Couleur paint)
{ 
	(*ptcr)->set_line_width(epaisseur_trait);
	(*ptcr)->begin_new_sub_path ();
	(*ptcr)->arc (c_x, c_y, rayon, 0, 2*M_PI);
	(*ptcr)->set_source_rgb (1,1,1);
	(*ptcr)->fill_preserve ();
	choix_couleur(paint);
	(*ptcr)->stroke();	
}

void rectangle(double c_x, double c_y, double rayon, Couleur paint)
{
	(*ptcr)->set_line_width(epaisseur_trait);
	choix_couleur(paint);
	(*ptcr)->move_to(c_x, c_y + rayon/ 4); //MAGIC NUMBER
	(*ptcr)->line_to(c_x - (0.75*rayon), c_y + rayon/4);
	(*ptcr)->move_to(c_x, c_y + rayon/ 4); 
	(*ptcr)->line_to(c_x + (0.75*rayon), c_y + rayon/4);
	(*ptcr)->line_to(c_x + (0.75*rayon), c_y - rayon/4);
	(*ptcr)->line_to(c_x - (0.75*rayon), c_y - rayon/4);
	(*ptcr)->line_to(c_x - (0.75*rayon), c_y + rayon/4);
	(*ptcr)->stroke();
}
