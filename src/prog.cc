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
	auto app = Gtk::Application::create();

	if(argc > 1) MaFenetre eventWindow(argv[1]);
	MaFenetre eventWindow;

	eventWindow.set_default_size(400,400);
	eventWindow.set_resizable(false);

	return app->run(eventWindow);
}
