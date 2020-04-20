/**\
 * \name    gui.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 2.0
 */

#include <iostream>
#include "gui.h"
#include "ville.h"
#include <cairomm/context.h>

using namespace std;

Dessin::Dessin(): empty(false)
{}

Dessin::~Dessin()
{}

void Dessin::refresh()
{
    auto win = get_window();
    if(win)
    {
        Gdk::Rectangle rect(0,0,get_allocation().get_width(),get_allocation().get_height());
        win->invalidate_rect(rect,false);
    }
}

void projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr,Cadre cadre)
{
    cr->set_identity_matrix();
	// déplace l'origine au centre de la fenêtre
	cr->translate(cadre.width/2, cadre.height/2);
  
	// normalise la largeur et hauteur aux valeurs fournies par le cadrage
	// ET inverse la direction de l'axe Y

	//cr->scale(cadre.width/(cadre.xMax - cadre.xMin),-cadre.height/(cadre.yMax - cadre.yMin));
  
	// décalage au centre du cadrage
	cr->translate(-(cadre.xMin + cadre.xMax)/2, -(cadre.yMin + cadre.yMax)/2);
}

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(not empty)
    {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        projectionOrtho(cr,cadre);

        cr->set_line_width(2);
        cr->set_source_rgb(1,1,1);

        cr->move_to(-10,-10);
        cr->line_to(10,10);
        cr->move_to(-10,10);
        cr->line_to(10,-10);

        cr->stroke();
    }
    return true;
}

void Dessin::ligne(double x1, double x2, double y1, double y2)
{
    //dessine une ligne entre (x1,y1) et (x2,y2) via cairo context
}

void Dessin::encadre(Cadre x)
{
    cadre = x;
}

void Dessin::encadre(double xMin, double xMax, double yMin, 
                 double yMax, int height, int width)
{
    cadre.xMin = xMin;
    cadre.xMax = xMax;
    cadre.xMin = xMin;
    cadre.xMax = xMax;
    cadre.asp = (xMax-xMin) / (yMax-yMin);
    cadre.height = height;
    cadre.width = width;
}

double Dessin::getAsp()
{
    return cadre.asp;
}
//initialisation de la fenetre 
MaFenetre::MaFenetre():
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
    editButton("edit"),
    housing("housing"),
    transport("transport"),
    production("production"),
    ENJ("ENJ : !"),
    CI("CI : !"),
    MTA("MTA : !"),
    generalFrame("General"),
    displayFrame("Display"),
    editorFrame("Editor"),
    infoFrame("Information")
{
    set_title("Archipelago");
    set_icon_from_file("Archicon.png"); //haha
    set_border_width(5);

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

    graph.set_size_request(800,800);
    graph.encadre(-10,10,-10,10,800,800);
    rightPanel.pack_start(graph);

    show_all_children();
}

MaFenetre::~MaFenetre()
{}

void MaFenetre::on_button_clicked_exit()
{
    cout<<"EXIT"<<endl;
}

void MaFenetre::on_button_clicked_new()
{
    cout<<"NEW"<<endl;
}

void MaFenetre::on_button_clicked_open()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);
    
    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            std::cout << "Open clicked." << std::endl;
            std::string filename = dialog.get_filename();
            //maVille->checkdata();
            std::cout << "File selected: " <<  filename << std::endl;
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
    cout<<"SAVE"<<endl;
}

void MaFenetre::on_button_clicked_shortestPath()
{
    cout<<"ShortestPath"<<endl;
}

void MaFenetre::on_button_clicked_zoomIn()
{
    cout<<"zoom in"<<endl;
}

void MaFenetre::on_button_clicked_zoomOut()
{
    cout<<"zoom out"<<endl;
}

void MaFenetre::on_button_clicked_zoomReset()
{
    cout<<"zoom reset"<<endl;
}

void MaFenetre::on_button_clicked_edit()
{
    cout<<"edit link"<<endl;
}
