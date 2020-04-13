/**\
 * \name    gui.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 2.0
 */

#include <gtkmm.h>

#ifndef GUI_H
#define GUI_H
class Dessin : public Gtk::DrawingArea
{
public:
    Dessin();
    virtual ~Dessin();
    void affiche();

protected: 
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
    bool empty;
    void refresh();
};

class MaFenetre : public Gtk::Window
{
public :
    MaFenetre();
    virtual ~MaFenetre();

protected:
    void on_button_clicked_test();

    Gtk::Box mainWindow,leftPanel,rightPanel;
    Dessin graph;
    Gtk::Button testbutton;

private:
    void affiche();
};

#endif