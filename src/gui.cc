/**\
 * \name    gui.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 2.0
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
{}

void Dessin::set_ville(Ville* ville)
{
    maVille = ville;
}

Ville* Dessin::get_ville_ptr()
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
        if(space.zoom < max_zoom) space.zoom += delta_zoom;
        break;
    case ZOUT:
        if(space.zoom > min_zoom + epsil_zero) space.zoom -= delta_zoom;
        break;
    default:
        cout<<"Zooming error"<<endl;
        break;
    }
}

double Dessin::get_zoom_ind()
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
        win->invalidate_rect(rect,false);
    }
}

void Dessin::projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr, Space space)
{   
    encadre();
    double factor = get_allocation().get_height()/space.size;
    
    cr->set_identity_matrix();
	cr->translate(get_allocation().get_width()/2 +      //centre le dessin
                  get_allocation().get_x(),             
                  get_allocation().get_height()/2);
    cr->scale(factor,-factor);                          //système euclidien
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(not empty)
    {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        graphic_set_context(cr);    //pour dessiner ville

        cr->set_line_width(epaisseur_trait);
        cr->set_source_rgb(1,1,1);
        cr->move_to(0,0);
        cr->line_to(width,0);
        cr->line_to(width,height);
        cr->line_to(0,height);
        cr->line_to(0,0);
        cr->paint();
        cr->stroke();

        projectionOrtho(cr,space);
        
        if(!(maVille == nullptr))
        {  
            maVille->draw_ville(NOIR);
        } 
    }
    return true;
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
    exitButton("exit"),
    newButton("new"),
    openButton("open"),
    saveButton("save"),
    shortPathButton("shortest path"),
    zoominButton("zoom in"),
    zoomoutButton("zoom out"),
    zoomresetButton("zoom reset"),
    currentZoom("zoom : ?"),
    MTA("MTA : 0"),
    CI("CI : 0"),
    ENJ("ENJ : 0"),
    editButton("edit link"),
    housing(type,"housing"),
    transport(type,"transport"),
    production(type,"production"),
    generalFrame("General"),
    displayFrame("Display"),
    editorFrame("Editor"),
    infoFrame("Information")
{
    graph.set_ville(new Ville(true));
    graph.get_ville_ptr()->chargement(fichier);

    set_title("Archipelago");
    set_icon_from_file("Archicon.png"); 
    set_border_width(5);

    graph.set_size_request(default_drawing_size,default_drawing_size);
    currentZoom.set_label("zoom : " + to_string(graph.get_zoom_ind()));

    add(mainWindow);

    mainWindow.pack_start(leftPanel);
    mainWindow.pack_start(rightPanel);

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

    //signal Handlers
    exitButton.signal_clicked().connect( sigc::mem_fun(*this,
                &MaFenetre::on_button_clicked_exit));
    openButton.signal_clicked().connect(sigc::mem_fun(*this,
                &MaFenetre::on_button_clicked_open));
    newButton.signal_clicked().connect( sigc::mem_fun(*this,
                &MaFenetre::on_button_clicked_new));
    saveButton.signal_clicked().connect(sigc::mem_fun(*this,
                &MaFenetre::on_button_clicked_save));

    shortPathButton.signal_clicked().connect( sigc::mem_fun(*this,
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

    rightPanel.pack_start(graph);

    show_all_children();
    update();
}

MaFenetre::~MaFenetre()
{}

void MaFenetre::on_button_clicked_exit()
{
	cout << "Exit" << endl;
    close();
}

void MaFenetre::on_button_clicked_new()
{
    if(graph.get_ville_ptr() == nullptr)	graph.set_ville(new Ville(true));
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
            
            if(graph.get_ville_ptr()==nullptr) graph.set_ville(new Ville(true));
            
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
    if(graph.get_ville_ptr() == nullptr)	cout<<"nothing to save"<<endl;
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
    cout<<"edit link ";

    if(housing.get_active())          cout<<"for housing"<<endl;
    else if(transport.get_active())   cout<<"for transport"<<endl;
    else if(production.get_active())  cout<<"for production"<<endl;
    else cout<<"*no type selected*"<<endl;
}

void MaFenetre::on_button_clicked_housing()
{
    
}

void MaFenetre::on_button_clicked_transport()
{
    
}

void MaFenetre::on_button_clicked_production()
{
    
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
		
		if(clic_x >= dessin_x && clic_x <= dessin_x + width &&
		   clic_y >= dessin_y && clic_y <= dessin_y + height)
		{ 
			//double[2] p = {clic_x - dessin_x, clic_y - dessin_y};
			
			if(event->button == left_click) 
			{
				cout<<"mouse left clicked"<<endl;
			}
			else if(event->button == right_click) 
			{
				cout<<"mouse right clicked"<<endl;
			}
		}
	}
	return true;
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
			case a:
				cout<<"typed a"<<endl;
				break;
			case w:
				cout<<"typed w"<<endl;
				break;
            case s:
				cout<<"typed s"<<endl;
				break;
			case d:
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
    } else {
        graph.get_ville_ptr()->reset();
        ENJ.set_label("ENJ : 0");
        CI.set_label("CI : 0");
        MTA.set_label("MTA : 0");
    }
    graph.refresh();
}
