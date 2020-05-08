/**\
 * \name    prog.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */

#include <iostream>
#include "ville.h"
#include "gui.h"

int main(int argc, char** argv)
{
	auto app = Gtk::Application::create();

	MaFenetre eventWindow(argv[1]);
	eventWindow.set_default_size(1000,800);
	eventWindow.set_resizable(true);
	return app->run(eventWindow);

}

