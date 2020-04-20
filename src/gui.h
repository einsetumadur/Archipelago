/**\
 * \name    gui.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 2.0
 */

#include <gtkmm.h>
#include "ville.h"

#ifndef GUI_H
#define GUI_H

struct Cadre
{
    double xMin; 
	double xMax;
	double yMin;
	double yMax;
	double asp; 
	int height;  
	int width;   
};
class Dessin : public Gtk::DrawingArea
{
public:
    Dessin();
    virtual ~Dessin();
    void affiche();
    void clear();
    void ligne(double x1, double x2, double y1, double y2);
    void encadre(Cadre x);
    void encadre(double xMin, double xMax, double yMin, 
                 double yMax, int height, int width);
    double getAsp();

protected: 
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    bool empty;
    void refresh();
    Cadre cadre;
};

class MaFenetre : public Gtk::Window
{
public :
    MaFenetre();
    virtual ~MaFenetre();

protected:
    void on_button_clicked_exit();
    void on_button_clicked_new();
    void on_button_clicked_open();
    void on_button_clicked_save();
    void on_button_clicked_shortestPath();
    void on_button_clicked_zoomIn();
    void on_button_clicked_zoomOut();
    void on_button_clicked_zoomReset();
    void on_button_clicked_edit();

    Gtk::Box mainWindow,leftPanel,rightPanel,general,display,editor,information;
    Dessin graph;
    Gtk::Button exitButton,newButton,openButton,saveButton;
    Gtk::Button shortPathButton,zoominButton,zoomoutButton,zoomresetButton;
    Gtk::ToggleButton editButton;
    Gtk::CheckButton housing,transport,production;
    Gtk::Frame generalFrame,displayFrame,editorFrame,infoFrame;
    Gtk::Label ENJ,CI,MTA;

private:
    void affiche();
    Ville *maVille;
};

#endif