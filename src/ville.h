/**\
 * \name    ville.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */

#ifndef VILLE_H
#define VILLE_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "noeud.h"

enum Etat_lecture {NBL,LOGE,NBP,PROD,NBT,TRAN,NBLI,LIENS,FIN};

class Ville
{
public:
	Ville(bool val);
	~Ville();
	
	double mta();	
	double enj();
	double ci();
	void chargement(char* nom_fichier);
	
	bool get_chargement_verif() const { return chargement_verif; }
	void set_chargement_verif(bool c);
	unsigned int get_error_param_un() const { return error_param_un; }
	unsigned int get_error_param_deux() const { return error_param_deux; }
	std::vector<Noeud*> getQuartiers() const { return quartiers; }
	void reset();
	
	// sauvegarde du fichier :
	void sauvegarde(std::string file) const;
	std::string print_type(std::string type) const;
	unsigned int nb_type(std::string type) const;
	unsigned int nb_liens() const;
	
	// dessins
	void setPaint(size_t indice, Couleur paint);
	void draw_ville() const;
	void draw_liens() const;
	void draw_quartiers() const;
	void draw_short_path(Couleur paint, Noeud* nd) const;

	//modification
	Noeud* trouve_noeud(double x, double y);
	void ajout_noeud(double x, double y, Type_noeud type);
	void retire_noeud(Noeud* node);
	void resize_node(Noeud* node, Point p1, Point p2);
	void deplace_noeud(Noeud* node, Point p);
	void update_node_paint(Noeud* node, Couleur color);
	void edit_lien(Noeud* node1, Noeud* node2);
	
private:	
	// Lecture et création de la ville
	void decodage(std::string line, int& etat);
	void ajout_noeud(std::istringstream& param,int& counter, Etat_lecture type);
	void error_noeud(Noeud* const nd);
	void creation_lien(unsigned int uid_a, unsigned int uid_b);
	void error_lien(Noeud* const a, Noeud* const b);
	void obstruction_lien(Noeud* node);
	Noeud* trouve_lien(unsigned int uid) const; 
	void redondance_uid(unsigned int numid);
	void collis_noeuds(Noeud* node);
	void check_load_noeud(Noeud* newNoeud);
	void load_in_tile(Noeud* node);
	unsigned int get_tile_index(double coord);
	void unload_quartier(Noeud* node);
	void unload_in_tile(Noeud* node);
	void load_uid(unsigned int uid);
	void unload_uid(unsigned int uid);

	bool chargement_verif;
	Type_error msg_error;
	unsigned int error_param_un;
	unsigned int error_param_deux;
	std::vector<Noeud*> quartiers;	
	std::vector<std::vector<tile>> grid;
	std::vector<unsigned int> occupied_uids;
	double nbp_nbL;
	double nbp_nbT;
	double nbp_nbP;
	double cmt_mta;
};

#endif

