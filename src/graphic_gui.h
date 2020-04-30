/**\
 * \name    graphic_gui.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */
 
#ifndef GRAPHIC_GUI_H
#define GRAPHIC_GUI_H

#include <gtkmm.h>
#include "graphics.h"

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

#endif 
