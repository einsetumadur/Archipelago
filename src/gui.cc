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

bool Dessin::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
    if(not empty)
    {
        Gtk::Allocation allocation = get_allocation();
        const int width = allocation.get_width();
        const int height = allocation.get_height();

        cr->set_line_width(5);
        cr->set_source_rgb(0,1,1);

        cr->move_to(0,0);
        cr->line_to(width,0);
        cr->line_to(width,height);
        cr->line_to(0,height);
        cr->line_to(0,0);

        cr->stroke();
    }
    return true;
}

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
    ENJ("ENJ : "),
    CI("CI : "),
    MTA("MTA : "),
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

    general.pack_start(exitButton,false,false);
    general.pack_start(newButton,false,false);
    general.pack_start(openButton,false,false);
    general.pack_start(saveButton,false,false);

    display.pack_start(shortPathButton,false,false);
    display.pack_start(zoominButton,false,false);
    display.pack_start(zoomoutButton,false,false);
    display.pack_start(zoomresetButton,false,false);

    editor.pack_start(editButton,false,false);
    editor.pack_start(housing,false,false);
    editor.pack_start(transport,false,false);
    editor.pack_start(production,false,false);

    information.pack_start(ENJ);
    information.pack_start(CI);
    information.pack_start(MTA);

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

    graph.set_size_request(400,400);
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
    cout<<"OPEN"<<endl;
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
