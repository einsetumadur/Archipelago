/**\
 * \name    gui.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 2.0
 */

#include <iostream>
#include "gui.h"
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
    }
    return true;
}

MaFenetre::MaFenetre():
    mainWindow(Gtk::ORIENTATION_HORIZONTAL,10),
    leftPanel(Gtk::ORIENTATION_VERTICAL,10),
    rightPanel(Gtk::ORIENTATION_VERTICAL,10),
    testbutton("test")
{
    set_title("Archipelago");
    set_border_width(5);

    add(mainWindow);

    mainWindow.pack_start(leftPanel);
    mainWindow.pack_start(rightPanel);

    leftPanel.pack_start(testbutton,false,false);

    graph.set_size_request(400,400);
    rightPanel.pack_start(graph);

    show_all_children();
}

MaFenetre::~MaFenetre()
{}

void MaFenetre::on_button_clicked_test()
{
    graph.affiche();
    cout<<"hein?"<<endl;
}

void Dessin::affiche()
{
    empty = false;
    refresh();
}


