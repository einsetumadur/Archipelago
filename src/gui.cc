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

using namespace std;

///////////////////////////// Section Dessin //////////////////////////////////////////

Dessin::Dessin(): empty(false)
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

Ville* Dessin::get_ville_ptr() const 
{
	return maVille;
}

void Dessin::encadre()
{
	space.size = 2*dim_max/space.zoom;
	space.xMin = -space.size/2;
	space.xMax = space.size/2;
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

double Dessin::get_zoom_ind() const
{
	return space.zoom;
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
	encadre();
	double factor = get_allocation().get_height()/space.size;

	cr->set_identity_matrix();
	cr->translate(get_allocation().get_height()/2 +	//centre le dessin
				  get_allocation().get_x(),
				  get_allocation().get_height()/2);
				  		  
	cr->scale(factor,-factor);	//système euclidien
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if(not empty)
	{
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		graphic_set_context(cr);    //pour dessiner ville

		projectionOrtho(cr,space);

		cr->set_line_width(epaisseur_trait);
		cr->set_source_rgb(1,1,1);
		cr->move_to(0,0);
		cr->line_to(width,0);
		cr->line_to(width,height);
		cr->line_to(0,height);
		cr->line_to(0,0);
		cr->paint();
		cr->stroke();		
		
		if(centre.size()>0) 
		{
			cercle(centre[0].pos_x, centre[0].pos_y, 200, ROUGE);
			centre.clear();
		}
						
		if(!(maVille == nullptr))  maVille->draw_ville();
	}
	return true;
}

void Dessin::set_cursor(double x,double y)
{
	mouseCursor[0] = x;
	mouseCursor[1] = y;
}

void Dessin::add_point(Point p)
{
	centre.push_back(p);
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
	nd_actif(nullptr),
	nd_button(LOGEMENT)
{
	graph.set_ville(new Ville(true));
	graph.get_ville_ptr()->chargement(fichier);

	set_title("Archipelago");
	set_icon_from_file("Archicon.png"); 
	set_border_width(5);

	graph.set_size_request(default_drawing_size,default_drawing_size);
	currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));
	graph.set_cursor(0,0);
   
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
	mainWindow.pack_start(rightPanel, Gtk::PACK_SHRINK);

	general.set_border_width(epaisseur_bord);
	general.pack_start(exitButton,false,false, Gtk::PACK_SHRINK);
	general.pack_start(newButton,false,false, Gtk::PACK_SHRINK);
	general.pack_start(openButton,false,false, Gtk::PACK_SHRINK);
	general.pack_start(saveButton,false,false, Gtk::PACK_SHRINK);

	display.set_border_width(epaisseur_bord);
	display.pack_start(shortPathButton,false,false, Gtk::PACK_SHRINK);
	display.pack_start(zoominButton,false,false, Gtk::PACK_SHRINK);
	display.pack_start(zoomoutButton,false,false, Gtk::PACK_SHRINK);
	display.pack_start(zoomresetButton,false,false, Gtk::PACK_SHRINK);
	display.pack_start(currentZoom);

	editor.set_border_width(epaisseur_bord);
	editor.pack_start(editButton,false,false, Gtk::PACK_SHRINK);
	editor.pack_start(housing,false,false, Gtk::PACK_SHRINK);
	editor.pack_start(transport,false,false, Gtk::PACK_SHRINK);
	editor.pack_start(production,false,false, Gtk::PACK_SHRINK);

	information.set_border_width(epaisseur_bord);
	information.pack_start(ENJ);
	information.pack_start(CI);
	information.pack_start(MTA);

	leftPanel.set_border_width(epaisseur_bord);
	leftPanel.pack_start(generalFrame);
	leftPanel.pack_start(displayFrame);
	leftPanel.pack_start(editorFrame);
	leftPanel.pack_start(infoFrame);
	rightPanel.pack_start(graph);

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

	shortPathButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_shortestPath));
	zoominButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomIn));
	zoomoutButton.signal_clicked().connect( sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomOut));
	zoomresetButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_zoomReset));

	editButton.signal_clicked().connect(sigc::mem_fun(*this,
				&MaFenetre::on_button_clicked_edit));
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
	shortPathButton.signal_activate();
	cout<<"ShortestPath"<<endl;
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
	/* if(activeNode == no_link) 
	{
		cout << "Please select first a node to edit !" << endl;
		editButton.set_active(false);
	} */		
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
	strstream << num;
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
		double width = graph.get_allocation().get_width();
		double height= graph.get_allocation().get_height();
		
		double x_d = clic_x - dessin_x;
		double y_d = clic_y - dessin_y;
		double x_m = (x_d/width)*(dim_max - (-1*dim_max)) - dim_max;
		double y_m = dim_max - (dim_max + dim_max)*(y_d/height);
	
		if(clic_x >= dessin_x && clic_x <= dessin_x + width &&
		   clic_y >= dessin_y && clic_y <= dessin_y + height)
		{ 
			graph.set_cursor(x_m, y_m);			
			graph.add_point({x_m, y_m});
			graph.refresh();
				
			if(event->button == left_click)	
			{
				 Noeud* clic_nd = graph.get_ville_ptr()->trouve_noeud(x_m, y_m);
				 if(clic_nd == nullptr)
				 {
					if(nd_actif != nullptr)	
					{
						graph.get_ville_ptr()->update_node_paint(nd_actif, NOIR);
						nd_actif = nullptr; 
						update();
					}
					else
					{
						cout << "HELLO" << endl;
						graph.get_ville_ptr()->ajout_noeud(x_m, y_m, nd_button);			//creer_noeud(...) return l'indice du nouveau noeud dans quartiers, place le nd au bon endroit, et fais les tests nécessaire.
						
					/*	if(not(get_chargement_verif()))
						{	
							switch(get_msg_error()):
								case N_N_SUP: 
									cout << "Collision with existing node ! " << endl;
									changer chargement_verif à true;
									ainsi que les attributs de msg error;
									delete_noeud(new_nd);
									break;
								case N_L_SUP:
									cout << "Collision with existing link ! " << endl;
									changer chargement_verif à true;
									ainsi que les attributs de msg error;
									delete_noeud(new_nd);
									break;
							update();

						}*/
						update();
					}
				}
				else if(clic_nd != nd_actif)
				{
				    if(editButton.get_active())
				    {
						graph.get_ville_ptr()->edit_lien(clic_nd, nd_actif);		//-> si les deux noeuds sont deja reliés, supprimer le lien. Sinon, créer le lien et tester collision
						
						/*if(not(chargement_verif))
						{
							cout << le msg de error.cc pour collision entre lien et quartier << endl;
							changer chargement_verif à true;
							ainsi que les attributs de msg error;
						}*/
						
						update();
					}
				    else
				    {
						if(nd_actif == nullptr)
						{
							graph.get_ville_ptr()->update_node_paint(clic_nd, ROUGE);
							update();
						}
						else
						{
							graph.get_ville_ptr()->update_node_paint(nd_actif, NOIR);						
							graph.get_ville_ptr()->update_node_paint(clic_nd, ROUGE);
							update();
						}
					}
				}
				else if(clic_nd == nd_actif and nd_actif != nullptr)
				{
					if(editButton.get_active())  cout << "Node cannot connect to itself" << endl;
					else
					{
						graph.get_ville_ptr()->retire_noeud(clic_nd);			// delete quartiers[clic_nd], attention à bien delete les liens et tout aussi !
						update();
					}
				}	
				cout<<"mouse left clicked at ( " <<	 x_m <<" , "<< y_m <<" )"<<endl;
			}
			
			else if(event->button == right_click)
			{
				/*if(nd_actif != nullptr)
				{
					if(not(graph.get_ville_ptr()->deplace_noeud(Noeud*, x_m, y_m)))		// deplace_active_noeud(...) return un bool de si lopération sest bien passé, déplace le noeud en question, refais tous les liens, etc)
					{																	// et dans le cas ou ya erreur, il rechange les coordonné€s de activeNode au ou cétait normalement.			
						switch(graph.get_ville_ptr()->get_msg_error()):
							case N_N_SUP:
								cout << "There is a collision between existing nodes ! " << endl;
								changer chargement_verif à true;
								ainsi que les attributs de msg error;
								update();
								break;
							case N_L_SUP:
								cout << "Some links collide with some nodes ! " << endl;
								changer chargement_verif à true;
								ainsi que les attributs de msg error;
								update();
								break;
					}
					else  update();
				}	*/
				cout<<"mouse right clicked"<<endl;
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
		double width = graph.get_allocation().get_width();
		double height= graph.get_allocation().get_height();
		
		if(clic_x >= dessin_x && clic_x <= dessin_x + width &&
		   clic_y >= dessin_y && clic_y <= dessin_y + height)
		{ 
			//double[2] p = {clic_x - dessin_x, clic_y - dessin_y};
			
			if(event->button == left_click)
			{
				 cout<<"mouse left released"<<endl;
			}
			else if(event->button == right_click) 
			{
				 cout<<"mouse right released"<<endl;
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

	if(graph.get_ville_ptr()->get_chargement_verif())
	{
		ENJ.set_label("ENJ :" + d_to_sci(graph.get_ville_ptr()->enj()));
		CI.set_label("CI : "+ d_to_sci(graph.get_ville_ptr()->ci()));
		MTA.set_label("MTA : "  + d_to_sci(graph.get_ville_ptr()->mta()));
	}
	else
	{
		/* swtich - case avec les msg d'erreur + enlever les cout << msg erreur de ville.cc*/
		graph.get_ville_ptr()->reset();
		ENJ.set_label("ENJ : 0");
		CI.set_label("CI : 0");
		MTA.set_label("MTA : 0");
	}
	graph.refresh();
}

