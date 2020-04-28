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
#include <cairomm/context.h>

using namespace std;

Dessin::Dessin(): empty(false)
{
    cadre.zoom = 1;
    cadre.size = 1;
}

Dessin::~Dessin()
{}

void Dessin::set_ville(Ville* ville)
{
    maVille = ville;
}

void Dessin::refresh()
{
    auto win = get_window();
    if(win)
    {
        Gdk::Rectangle rect(0,0,get_allocation().get_width(),get_allocation().get_height());
        win->invalidate_rect(rect,false);
    }
}

void Dessin::projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr,Cadre cadre)
{   
    encadre();
    double factor = cadre.size/get_allocation().get_width();
    cr->set_identity_matrix();
	cr->translate(get_allocation().get_width()/2 + get_allocation().get_x(),
                  get_allocation().get_height()/2);
    cr->scale(factor,-factor);
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(not empty)
    {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();
        graphic_set_context(cr);

        cr->set_line_width(2);
        cr->set_source_rgb(1,1,1);
        cr->move_to(0,0);
        cr->line_to(width,0);
        cr->line_to(width,height);
        cr->line_to(0,height);
        cr->line_to(0,0);
        cr->stroke();

        projectionOrtho(cr,cadre);

        if(!(maVille == nullptr)) maVille->draw_ville(ROUGE);

        cr->set_source_rgb(1,0,0);
        cr->move_to(0,0);
        cr->line_to(20,0);
        cr->stroke();

        cr->set_source_rgb(0,0,1);
        cr->move_to(0,0);
        cr->line_to(0,20);
        cr->stroke();
    }
    return true;
}

void Dessin::encadre(Cadre x)
{
    cadre = x;
}

void Dessin::encadre()
{
    if(cadre.zoom <= 0.0) cadre.zoom = 1;
    cadre.size = get_allocation().get_width()/cadre.zoom;
    cadre.xMax = cadre.size/2;
    cadre.xMin = -cadre.yMin;
    cadre.yMin = cadre.xMin;
    cadre.yMax = cadre.xMax;
}

//initialisation de la fenetre 
MaFenetre::MaFenetre()
{

}

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
    editButton("edit link"),
    housing(type,"housing"),
    transport(type,"transport"),
    production(type,"production"),
    generalFrame("General"),
    displayFrame("Display"),
    editorFrame("Editor"),
    infoFrame("Information")
{
    maVille = new Ville(true);
    maVille->chargement(fichier);

    if(maVille->get_chargement_verif()) 
    {
        ENJ.set_label("ENJ :" + d_to_sci(maVille->enj()));
        CI.set_label("CI : "+ d_to_sci(maVille->ci()));
        MTA.set_label("MTA : "  + d_to_sci(maVille->mta()));
    } else {
        ENJ.set_label("ENJ : no file");
        CI.set_label("CI : no file");
        MTA.set_label("MTA : no file");
        maVille = nullptr;
    }

    housing.set_active(true);

    set_title("Archipelago");
    set_icon_from_file("Archicon.png"); //haha
    set_border_width(5);

    graph.set_size_request(800,800);
    graph.set_ville(maVille);

    add(mainWindow);

    mainWindow.pack_start(leftPanel);
    mainWindow.pack_start(rightPanel);

    general.set_border_width(10);
    general.pack_start(exitButton,false,false);
    general.pack_start(newButton,false,false);
    general.pack_start(openButton,false,false);
    general.pack_start(saveButton,false,false);

    display.set_border_width(10);
    display.pack_start(shortPathButton,false,false);
    display.pack_start(zoominButton,false,false);
    display.pack_start(zoomoutButton,false,false);
    display.pack_start(zoomresetButton,false,false);
    display.pack_start(currentZoom);

    editor.set_border_width(10);
    editor.pack_start(editButton,false,false);
    editor.pack_start(housing,false,false);
    editor.pack_start(transport,false,false);
    editor.pack_start(production,false,false);

    information.set_border_width(10);
    information.pack_start(ENJ);
    information.pack_start(CI);
    information.pack_start(MTA);

    leftPanel.set_border_width(10);
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
}

MaFenetre::~MaFenetre()
{}

void MaFenetre::on_button_clicked_exit()
{
    close();
}

void MaFenetre::on_button_clicked_new()
{
    if(maVille == nullptr) maVille = new Ville(true);
    else 
    {
        delete maVille;
        maVille = new Ville(true);
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
            cout << "File selected: " <<  filename << std::endl;
            if(maVille==nullptr) maVille = new Ville(true);
            maVille->chargement(&filename[0]);
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
    if(maVille == nullptr) cout<<"nothing to save"<<endl;
    else 
    {
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
                if(maVille==nullptr) break;
                maVille->sauvegarde(filename);
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
    graph.cadre.zoom += 0.1;
    graph.refresh();
    cout<<"zoom in : "<<graph.cadre.zoom<<endl;
}

void MaFenetre::on_button_clicked_zoomOut()
{
    graph.cadre.zoom -= 0.1;
    graph.refresh();
    cout<<"zoom out : "<<graph.cadre.zoom<<endl;
}

void MaFenetre::on_button_clicked_zoomReset()
{
    graph.cadre.zoom = 1;
    graph.refresh();
    cout<<"zoom reset : "<<graph.cadre.zoom<<endl;
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
			
			if(event->button == 1) // Left click
			{
				cout<<"mouse left clicked"<<endl;
			}
			else if(event->button == 3) // Right click
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
			
			if(event->button == 1) // Left click release
			{
				cout<<"mouse left released"<<endl;
			}
			else if(event->button == 3) // Right click release
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
			case 'a':
				cout<<"typed a"<<endl;
				break;
			case 'w':
				cout<<"typed w"<<endl;
				break;
            case 's':
				cout<<"typed s"<<endl;
				break;
			case 'd':
				cout<<"typed d"<<endl;
				break;
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}

