/**\
 * \name    gui.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 3 - Architecture 11 b1
 */

#include <gtkmm.h>
#include "ville.h"
#include <string>
#include <sstream>

#ifndef GUI_H
#define GUI_H

enum zAction {ZR,ZIN,ZOUT};

struct Space
{
	double zoom,size;
	double xMin,xMax;
	double yMin,yMax; 
	double width, height;
	double xc, yc;
};

class Dessin : public Gtk::DrawingArea
{
public:
	Dessin();
	virtual ~Dessin();
	void refresh();
	void clear();
	void encadre();
	void set_ville(Ville* ville);
	void set_zoom(zAction act);
	void update_show_path(bool b);
	double get_zoom_ind() const { return space.zoom; }
	Ville* get_ville_ptr() const { return maVille; }
	bool get_show_path() const { return show_path; }
	Noeud* get_nd_actif() const { return nd_actif; }
	void set_nd_actif(Noeud* nd);
	Space space;
	void distorsion_fenetre();

protected: 
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
	void projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr, Space space);
	bool empty;
	double currentZoom;
	Ville* maVille;
	Noeud* nd_actif;
	bool show_path;
};

class MaFenetre : public Gtk::Window
{
public :
	MaFenetre(char* fichier);
	virtual ~MaFenetre();
	void update();
	
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
	void on_button_clicked_housing();
	void on_button_clicked_transport();
	void on_button_clicked_production();
	bool on_button_press_event(GdkEventButton * event);
	bool on_button_release_event(GdkEventButton * event);
	bool on_key_press_event(GdkEventKey * key_event);
	
	void on_button_press_shortestPath();
	void on_button_release_shortestPath();
	void on_button_press_edit();
	void on_button_release_edit();
	
	void left_press_event(Noeud* clic_nd, Noeud* nd_actif);
	void left_release_event(Noeud* clic_nd, Noeud* nd_actif);
	void detruire_nd(Noeud* clic_nd, Noeud* nd_actif);
	void init_show_path();
	void updateLabels(bool mta, bool ci, bool enj, bool new_doc = false);

	Dessin graph;
	Type_noeud nd_button;
	Point press_click;
	Point release_click;
	bool update_mta;
	bool update_ci;
	bool update_enj;
	bool new_file;
	
	Gtk::Box mainWindow,leftPanel,rightPanel,general,display,editor,information;
	Gtk::Button exitButton,newButton,openButton,saveButton;
	Gtk::Button zoominButton,zoomoutButton,zoomresetButton;
	Gtk::ToggleButton editButton, shortPathButton;
	Gtk::RadioButtonGroup type;
	Gtk::RadioButton housing,transport,production;
	Gtk::Frame generalFrame,displayFrame,editorFrame,infoFrame, m_Frame_Area;
	Gtk::Label ENJ,CI,MTA,currentZoom;

private:
	void connect_signal_handlers();
	void construct_scaffolding();
};

std::string d_to_sci(double num);

#endif
