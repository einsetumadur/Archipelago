/**\
 * \name   prog.cc
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */

#include <iostream>
#include "ville.h"
#include "gui.h"

int main(int argc, char** argv)
{
	main_ville(argv[1]);

	auto app = Gtk::Application::create(argc,argv,"Archipelago");

	MaFenetre eventWindow;
	eventWindow.set_default_size(400,400);
	eventWindow.set_resizable(false);

	return app->run(eventWindow);
}
