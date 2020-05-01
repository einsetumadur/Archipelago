/**\
 * \name    graphics.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version Rendu 2 - Architecture 11 b1
 */

#include "graphic_gui.h"

constexpr double rapport_vertical(8);
constexpr double rapport_horizon(0.75);

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
		case(BLANC):
			(*ptcr)->set_source_rgb(1.0, 1.0, 1.0);
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
	choix_couleur(BLANC);
	(*ptcr)->fill_preserve ();
	choix_couleur(paint);
	(*ptcr)->stroke();	
}

void rectangle(double c_x, double c_y, double rayon, Couleur paint)
{

	(*ptcr)->set_line_width(epaisseur_trait);
	choix_couleur(paint);
	(*ptcr)->move_to(c_x - (rapport_horizon*rayon), c_y + rayon/rapport_vertical);
	(*ptcr)->line_to(c_x + (rapport_horizon*rayon), c_y + rayon/rapport_vertical);
	(*ptcr)->line_to(c_x + (rapport_horizon*rayon), c_y - rayon/rapport_vertical);
	(*ptcr)->line_to(c_x - (rapport_horizon*rayon), c_y - rayon/rapport_vertical);
	(*ptcr)->line_to(c_x - (rapport_horizon*rayon), c_y + rayon/rapport_vertical);
	(*ptcr)->line_to(c_x + (rapport_horizon*rayon), c_y + rayon/rapport_vertical);
	(*ptcr)->stroke();
	
}
