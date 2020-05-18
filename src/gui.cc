/**\
 * \name    gui.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 : Architecture 11b1
 */

#include <iostream>
#include <string>
#include "gui.h"
#include "graphic_color.h"
#include "graphic_gui.h"
#include "ville.h"
#include "constantes.h"
#include <cairomm/context.h>
#include <iomanip>

using namespace std;

///////////////////////////// Section Dessin //////////////////////////////////////////

Dessin::Dessin(): empty(false), nd_actif(nullptr), show_path(false)
{
	space.zoom = 1;
	space.size = dim_max;
}

Dessin::~Dessin()
{
}

void Dessin::set_ville(Ville* ville)
{
	maVille = ville;
}

void Dessin::encadre()
{
	space.size = 2*dim_max/space.zoom;
	space.xMax = space.size/2;
	space.xMin = -space.size/2;
	space.yMin = -space.size/2;
	space.yMax = space.size/2;
}

void Dessin::set_zoom(zAction act)
{
	switch (act)
	{
		case ZR:
			space.zoom = 1;
			break;
		case ZIN:
			if(space.zoom < max_zoom)  space.zoom += delta_zoom;
			break;
		case ZOUT:
			if(space.zoom > min_zoom + epsil_zero)  space.zoom -= delta_zoom;
			break;
		default:
			cout<<"Zooming error"<<endl;
			break;
	}
}

void Dessin::refresh()
{
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle rect(0,0,get_allocation().get_width(),
							get_allocation().get_height());
		win->invalidate_rect(rect,true);
	}
}

void Dessin::projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr, Space space)
{
	cr->translate(space.xc, space.yc);
	cr->scale(space.width/(space.xMax - space.xMin), -space.height/(space.yMax - 
																	space.yMin));
	cr->translate(-(space.xMin + space.xMax)/2, -(space.yMin + space.yMax)/2);
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	Gtk::Allocation allocation = get_allocation();
	space.width = allocation.get_width();
	space.height = allocation.get_height();
	if(allocation.get_width() < allocation.get_height()) 
	{
		space.width = allocation.get_width();
		space.height = allocation.get_width();
	}
	else if(allocation.get_width() > allocation.get_height()) 
	{
		space.width = allocation.get_height();
		space.height = allocation.get_height();
	}
	space.xc = space.width/2;
	space.yc = space.height/2;
	encadre();
	projectionOrtho(cr,space);
	graphic_set_context(cr);    //pour dessiner ville
		
		if(not empty)
		{		
			cr->set_line_width(epaisseur_trait);
			cr->set_source_rgb(1,1,1);
			cr->move_to(0,0);
			cr->line_to(space.width,0);
			cr->line_to(space.width, space.height);
			cr->line_to(0, space.height);
			cr->line_to(0,0);
			cr->paint();
			cr->stroke();		
				
			if(!(maVille == nullptr))
			{
				if(show_path == true && nd_actif != nullptr) 
				{
					maVille->draw_ville();
					maVille->draw_short_path(VERT, nd_actif);
				}
				else maVille->draw_ville();
			}
	}
	return true;
}

void Dessin::set_nd_actif(Noeud* nd)
{
	nd_actif = nd;
}

void Dessin::update_show_path(bool b)
{
	show_path = b;
}

///////////////////////// Section Fenetre /////////////////////////////////////////////

//initialisation de la fenetre 
MaFenetre::MaFenetre(char* fichier):
	mainWindow(Gtk::ORIENTATION_HORIZONTAL,10),
	leftPanel(Gtk::ORIENTATION_VERTICAL,10),
	rightPanel(Gtk::ORIENTATION_VERTICAL,10),
	general(Gtk::ORIENTATION_VERTICAL,10),
	display(Gtk::ORIENTATION_VERTICAL,10),
	editor(Gtk::ORIENTATION_VERTICAL,10),
	information(Gtk::ORIENTATION_VERTICAL,10),
	generalFrame("General"),
	exitButton("exit"),newButton("new"),openButton("open"),saveButton("save"),
	displayFrame("Display"),
	shortPathButton("shortest path"),zoominButton("zoom in"),zoomoutButton("zoom out"),
	zoomresetButton("zoom reset"),currentZoom("zoom : ?"),
	editorFrame("Editor"),
	editButton("edit link"),housing(type,"housing"),
	transport(type,"transport"),production(type,"production"),
	infoFrame("Information"),
	MTA("MTA : 0"),CI("CI : 0"),ENJ("ENJ : 0"),
	nd_button(LOGEMENT),
	update_mta(true), update_ci(true), update_enj(true)
{
	graph.set_ville(new Ville(true));
	graph.get_ville_ptr()->chargement(fichier);

	set_title("Archipelago");
	set_icon_from_file("Archicon.png"); 
	set_border_width(5);
	
	graph.set_size_request(default_drawing_size,default_drawing_size);
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
   
	add(mainWindow);
	construct_scaffolding();
	connect_signal_handlers();

	show_all_children();
	update();
}

MaFenetre::~MaFenetre()
{
}

void MaFenetre::construct_scaffolding()
{
	mainWindow.pack_start(leftPanel, Gtk::PACK_SHRINK);
	mainWindow.pack_start(rightPanel);
	rightPanel.pack_start(m_Frame_Area);
	m_Frame_Area.add(graph);
	
	general.set_border_width(epaisseur_bord);
	general.pack_start(exitButton,false,false);
	general.pack_start(newButton,false,false);
	general.pack_start(openButton,false,false);
	general.pack_start(saveButton,false,false);

	display.set_border_width(epaisseur_bord);
	display.pack_start(shortPathButton,false,false);
	display.pack_start(zoominButton,false,false);
	display.pack_start(zoomoutButton,false,false);
	display.pack_start(zoomresetButton,false,false);
	display.pack_start(currentZoom);

	editor.set_border_width(epaisseur_bord);
	editor.pack_start(editButton,false,false);
	editor.pack_start(housing,false,false);
	editor.pack_start(transport,false,false);
	editor.pack_start(production,false,false);

	information.set_border_width(epaisseur_bord);
	information.pack_start(ENJ);
	information.pack_start(CI);
	information.pack_start(MTA);

	leftPanel.set_border_width(epaisseur_bord);
	leftPanel.pack_start(generalFrame);
	leftPanel.pack_start(displayFrame);
	leftPanel.pack_start(editorFrame);
	leftPanel.pack_start(infoFrame);

	generalFrame.add(general);
	displayFrame.add(display);
	editorFrame.add(editor);
	infoFrame.add(information);
	
}

void MaFenetre::connect_signal_handlers()
{
	exitButton.signal_clicked().connect( sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_exit));
	openButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_open));
	newButton.signal_clicked().connect( sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_new));
	saveButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_save));

	zoominButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomIn));
	zoomoutButton.signal_clicked().connect( sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomOut));
	zoomresetButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomReset));

	editButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_edit));
	shortPathButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_shortestPath));
	housing.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_housing)); 
	transport.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_transport)); 
	production.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_production)); 
}

void MaFenetre::on_button_clicked_exit()
{
	cout << "Exit" << endl;
	close();
}

void MaFenetre::on_button_clicked_new()
{
	if(graph.get_ville_ptr() == nullptr)  graph.set_ville(new Ville(true));
	else 
	{
		graph.get_ville_ptr()->reset();
		update();
	}
}

void MaFenetre::on_button_clicked_open()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			string filename = dialog.get_filename();

			if(graph.get_ville_ptr()==nullptr)  graph.set_ville(new Ville(true));

			graph.get_ville_ptr()->reset();
			graph.get_ville_ptr()->chargement(&filename[0]);
			update_mta = true; update_ci = true; update_enj = true;
			update();
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			std::cout << "Cancel clicked." << std::endl;
			break;
		}
		default:
		{
			std::cout << "Unexpected button clicked." << std::endl;
			break;
		}
	}
}

void MaFenetre::on_button_clicked_save()
{
	if(graph.get_ville_ptr() == nullptr)  cout<<"nothing to save"<<endl;
	else 
	{
		cout << "Save" << endl;
		Gtk::FileChooserDialog dialog("Please choose a file",
									  Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*this);

		dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
		dialog.add_button("_Save", Gtk::RESPONSE_OK);

		int result = dialog.run();

		switch(result)
		{
			case(Gtk::RESPONSE_OK):
			{
				string filename = dialog.get_filename();
				cout << "File selected: " <<  filename << std::endl;
				if(graph.get_ville_ptr()==nullptr)	break;
				graph.get_ville_ptr()->sauvegarde(filename);
				break;
			}
			case(Gtk::RESPONSE_CANCEL):
			{
				std::cout << "Cancel clicked." << std::endl;
				break;
			}
			default:
			{
				std::cout << "Unexpected button clicked." << std::endl;
				break;
			}
		}
	}
}

void MaFenetre::on_button_clicked_shortestPath()
{
	if(shortPathButton.get_active())  on_button_press_shortestPath();
	else on_button_release_shortestPath();
}

void MaFenetre::on_button_press_shortestPath()
{
	graph.update_show_path(true);
	update();
}
void MaFenetre::on_button_release_shortestPath()
{
	graph.update_show_path(false);

	for(auto nd : graph.get_ville_ptr()->getQuartiers())
	{
		graph.get_ville_ptr()->update_node_paint(nd, NOIR);
	}
	if(graph.get_nd_actif() != nullptr)  graph.get_ville_ptr()->update_node_paint
															    (graph.get_nd_actif(), ROUGE);

	update();
}

void MaFenetre::on_button_clicked_zoomIn()
{
	graph.set_zoom(ZIN);
	graph.encadre();
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
	graph.refresh();
	cout<<"zoomed in"<<endl;
}

void MaFenetre::on_button_clicked_zoomOut()
{
	graph.set_zoom(ZOUT);
	graph.encadre();
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
	graph.refresh();
	cout<<"zoomed out"<<endl;
}
void MaFenetre::on_button_clicked_zoomReset()
{
	graph.set_zoom(ZR);
	graph.encadre();
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
	graph.refresh();
	cout<<"zoom reset"<<endl;
}

void MaFenetre::on_button_clicked_edit()
{
	if(editButton.get_active())  on_button_press_edit();
	else on_button_release_edit();
}

void MaFenetre::on_button_press_edit()
{
	if(graph.get_nd_actif() == nullptr) 
	{
		cout << "Please select first a node to edit !" << endl;
		editButton.set_active(false);
	}		
}
void MaFenetre::on_button_release_edit()
{
	cout << "release! " << editButton.get_active() << endl;
}

void MaFenetre::on_button_clicked_housing()
{
	if(housing.get_active())  nd_button = LOGEMENT;
}

void MaFenetre::on_button_clicked_transport()
{
	if(transport.get_active())  nd_button = TRANSPORT;
}

void MaFenetre::on_button_clicked_production()
{
	if(production.get_active())  nd_button = PRODUCTION;
}

string d_to_sci(double num)
{
	ostringstream strstream;
	if(num > 99999)  strstream << setprecision(precision) << scientific << num;
	else  strstream << fixed << setprecision(2*precision) << num;
	return strstream.str();
}

bool MaFenetre::on_button_press_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		double clic_x = event->x ;
		double clic_y = event->y ;
		double dessin_x = graph.get_allocation().get_x();
		double dessin_y = graph.get_allocation().get_y();
		double width = graph.space.width;
		double height= graph.space.height;
		double x_d = clic_x - dessin_x;
		double y_d = clic_y - dessin_y;
		double x_m = (x_d/width)*(graph.space.xMax - graph.space.xMin) + graph.space.xMin;
		double y_m = graph.space.yMax - (graph.space.yMax - graph.space.yMin)*(y_d/height);
		
		if(clic_x >= dessin_x && clic_y >= dessin_y)
		{ 			
			Noeud* clic_nd = graph.get_ville_ptr()->trouve_noeud(x_m, y_m);
			Noeud* nd_actif = graph.get_nd_actif();
			press_click = {x_m, y_m};
			
			if(event->button == left_click)	
			{
				if(clic_nd == nullptr && editButton.get_active() == false)
				{
					if(nd_actif == nullptr)
					{
						graph.get_ville_ptr()->ajout_noeud(x_m, y_m, nd_button);
						graph.get_ville_ptr()->set_chargement_verif(true);
						update_mta = true; update_ci = false; update_enj = true; 
					}	
				}
				else if(clic_nd != nullptr && clic_nd != nd_actif)
				{
						
				    if(editButton.get_active() && nd_actif != nullptr)
				    {
						graph.get_ville_ptr()->edit_lien(clic_nd, nd_actif);
							
						if(graph.get_show_path() && graph.get_ville_ptr()->get_chargement_verif())  
						{
							graph.update_show_path(false);
							for(auto nd : graph.get_ville_ptr()->getQuartiers())
							{
								graph.get_ville_ptr()->update_node_paint(nd, NOIR);
							}
							update();
							graph.update_show_path(true);
						}
						graph.get_ville_ptr()->set_chargement_verif(true);
						update_mta = true; update_ci = true; update_enj = false; 
					}
				    else if(not(editButton.get_active()))
				    {
						if(nd_actif == nullptr)
						{
							graph.get_ville_ptr()->update_node_paint(clic_nd, ROUGE);
							graph.set_nd_actif(clic_nd);
						}
						else
						{
							if(graph.get_show_path() == true)
							{
								graph.update_show_path(false);
								for(auto nd : graph.get_ville_ptr()->getQuartiers())
								{
									graph.get_ville_ptr()->update_node_paint(nd, NOIR);
								}
								update();
								graph.update_show_path(true);
							}
								graph.get_ville_ptr()->update_node_paint(nd_actif, NOIR);						
								graph.get_ville_ptr()->update_node_paint(clic_nd, ROUGE);
								graph.set_nd_actif(clic_nd);
						}
						update_mta = false; update_ci = false; update_enj = false;
					}
				}
				else if(clic_nd == nd_actif and nd_actif != nullptr)
				{
					if(editButton.get_active())  cout << "Node cannot connect to itself" << endl;
					else 
					{
						graph.get_ville_ptr()->retire_noeud(clic_nd);	
						//RAJOUTER LE CAS MTA.
						update_mta = true; update_ci = true; update_enj = true; 
						graph.set_nd_actif(nullptr);
					}
				}
				update();	
			}
			
			else if(event->button == right_click && nd_actif != nullptr)
			{
					/*graph.get_ville_ptr()->deplace_noeud(Noeud*, x_m, y_m)	
					graph.get_ville_ptr()->set_chargement_verif(true);
					update_mta = true; update_ci = true; update_enj = false;
					update();*/
			}
		}
	return true;
	}
}

bool MaFenetre::on_button_release_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_RELEASE)
	{
		
		double clic_x = event->x ;
		double clic_y = event->y ;
		double dessin_x = graph.get_allocation().get_x();
		double dessin_y = graph.get_allocation().get_y();
		double width = graph.space.width;
		double height= graph.space.height;
		
		double x_d = clic_x - dessin_x;
		double y_d = clic_y - dessin_y;
		double x_m = (x_d/width)*(graph.space.xMax - graph.space.xMin) + graph.space.xMin;
		double y_m = graph.space.yMax - (graph.space.yMax - graph.space.yMin)*(y_d/height);
		
		if(clic_x >= dessin_x && clic_y >= dessin_y)
		{ 		
			Noeud* clic_nd = graph.get_ville_ptr()->trouve_noeud(x_m, y_m);
			Noeud* nd_actif = graph.get_nd_actif();
			release_click = {x_m, y_m};

			if(event->button == left_click)
			{
				if(clic_nd == nullptr && nd_actif != nullptr && editButton.get_active() == false)
				{
					if(press_click.pos_x == release_click.pos_x && press_click.pos_y == release_click.pos_y)	
					{
						if(graph.get_show_path() == true)
						{
							for(auto nd : graph.get_ville_ptr()->getQuartiers())
							{
								graph.get_ville_ptr()->update_node_paint(nd, NOIR);
							}
						}
						else  graph.get_ville_ptr()->update_node_paint(nd_actif, NOIR);
						update_mta = false; update_ci = false; update_enj = false;
						graph.set_nd_actif(nullptr); 
					}
					else
					{
						graph.get_ville_ptr()->modif_nbp(nd_actif, press_click, release_click);
						graph.get_ville_ptr()->set_chargement_verif(true);
						update_mta = false; update_ci = true; update_enj = true;
					}
				}
				update();
			}
			else if(event->button == right_click) 
			{
			}
		}
		
	}
	return true;
}

bool MaFenetre::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 'i':
				cout<<"typed i"<<endl;
				on_button_clicked_zoomIn();
				break;
			case 'o':
				cout<<"typed o"<<endl;
				on_button_clicked_zoomOut();
				break;
			case 'r':
				cout<<"typed r"<<endl;
				on_button_clicked_zoomReset();
				break;
			case 'd':
				cout<<"typed d"<<endl;
				break;
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}

void MaFenetre::update()
{
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
	bool update_labels = update_ci && update_mta && update_enj;
	
	if(graph.get_ville_ptr()->get_chargement_verif())
	{
		if(update_labels)
		{
			ENJ.set_label("ENJ :" + d_to_sci(graph.get_ville_ptr()->enj()));
			CI.set_label("CI : "+ d_to_sci(graph.get_ville_ptr()->ci()));
			MTA.set_label("MTA : "  + d_to_sci(graph.get_ville_ptr()->mta()));
		}
		else if(update_enj && update_ci)
		{
			ENJ.set_label("ENJ :" + d_to_sci(graph.get_ville_ptr()->enj()));
			CI.set_label("CI : "+ d_to_sci(graph.get_ville_ptr()->ci()));
		}
		else if(update_enj && update_mta)
		{
			ENJ.set_label("ENJ :" + d_to_sci(graph.get_ville_ptr()->enj()));
			MTA.set_label("MTA : "  + d_to_sci(graph.get_ville_ptr()->mta()));
		}
		else if(update_ci && update_mta)
		{
			CI.set_label("CI : "+ d_to_sci(graph.get_ville_ptr()->ci()));
			MTA.set_label("MTA : "  + d_to_sci(graph.get_ville_ptr()->mta()));
		}
	}
	else
	{
		graph.get_ville_ptr()->reset();
		ENJ.set_label("ENJ : 0");
		CI.set_label("CI : 0");
		MTA.set_label("MTA : 0");
	}
	graph.refresh();
}
