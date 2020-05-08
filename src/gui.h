/**\
 * \name    gui.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 : Architecture 11 b1
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
	double get_zoom_ind() const;
	Ville* get_ville_ptr() const;
	void set_cursor(double x,double y);
protected: 
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
	void projectionOrtho(const Cairo::RefPtr<Cairo::Context>& cr, Space space);
	bool empty;
	Space space;
	double currentZoom;
	Ville* maVille;
	double mouseCursor[2];
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

	Gtk::Box mainWindow,leftPanel,rightPanel,general,display,editor,information;
	Dessin graph;
	Gtk::Button exitButton,newButton,openButton,saveButton;
	Gtk::Button shortPathButton,zoominButton,zoomoutButton,zoomresetButton;
	Gtk::ToggleButton editButton;
	Gtk::RadioButtonGroup type;
	Gtk::RadioButton housing,transport,production;
	Gtk::Frame generalFrame,displayFrame,editorFrame,infoFrame;
	Gtk::Label ENJ,CI,MTA,currentZoom;

private:
	void connect_signal_handlers();
	void construct_scaffolding();
};

std::string d_to_sci(double num);

#endif
