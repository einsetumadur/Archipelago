/**\
 * \name    noeud.h
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 3 - Architecture 11 b1
 */
 
#ifndef NOEUD_H
#define NOEUD_H

#include <iostream>
#include "tools.h"
#include <string>
#include <vector>
#include <cmath>

enum Type_error {NO_ERROR, RES_U, LITTLE_NBP, BIG_NBP, SELF_L_N, L_VAC, MULT_S_L, 
				 MAX_L, N_L_SUP, ID_U, N_N_SUP};
enum Scenario_djikstra {scen_aleatoire, scen_production, scen_transport};
enum Type_noeud {LOGEMENT,TRANSPORT,PRODUCTION};
enum Type_modif {NO_ACTION, ADD_ND};

const std::string logement = "Logement";
const std::string production  = "Production";
const std::string transport = "Transport";

class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite);
	virtual ~Noeud() = 0;
	
	std::string print() const;
	std::string print_lien(Noeud* nd_lien) const;
	void ajout_lien(Noeud* b);
	void reset_tab_liens();
	void disconnect(Noeud* node);
	void clean_vector_erase_path_prod(unsigned int index);
	void clean_vector_erase_path_tran(unsigned int index);
		
	// getters & setters 
	unsigned int getNbp() const { return nbp; }
	double getX() const { return batiment.centre.pos_x; }
	double getY() const { return batiment.centre.pos_y; }
	Point getCentre() const { return batiment.centre; }
	double getRayon() const { return batiment.rayon; }
	Cercle getBatiment() const { return batiment; }
	unsigned int getUid() const { return uid; }
	std::vector<Noeud*> getLiens() const { return tab_liens; }
	double getAccess() const { return access; }
	unsigned int getParent() const { return parent; }
	bool getIn() const { return in; }
	Couleur getPaint() const { return paint; }
	std::vector<Noeud*> get_short_path_prod() const { return short_path_prod; }
	std::vector<Noeud*> get_short_path_tran() const { return short_path_tran; }
	virtual double getSpeed() const = 0;
	virtual std::string getType() const = 0;
	void updateIn(bool b);	
	void updatePaint(Couleur c);
	void setNBP(double met);
	void setCentre(Point p);
	
	// fonctions de tests
	bool test_uid() const;
	Type_error test_nbp() const;
	bool multiple_link(Noeud* b)const;
	virtual bool maxi_link() const;
	bool collis_lien_quartier(Noeud* lien_a, Noeud* lien_b, double dist = 0) const; 
	bool is_connected_to(Noeud* node) const;
	bool is_under(double x, double y) const;
	
	// fonctions de dessins
	virtual void draw_noeud() const = 0;
	virtual void draw_path(Couleur paint) const;
	
	// MTA
	virtual void controle_djikstra(std::vector<int>& queue, 
								   const std::vector<Noeud*>& tn,  
								   Scenario_djikstra& scenario, size_t i, 
								   int& cmt_tab, double& cmt_mta, unsigned int nb_p,
								   unsigned int nb_t);
	unsigned int scenario_no_link(unsigned int nb_t, unsigned int nb_p, 
								  Scenario_djikstra& scenario);
	unsigned int scenario_nd_min(const std::vector<Noeud*>& tn, unsigned int nd_min,
								 Scenario_djikstra& scenario);
	
	// CI
	void calcul_ci(Noeud* nd_lien, double& cmt) const;
	
	bool operator==(const Noeud& nd) const;

protected:
	Cercle batiment;
	unsigned int uid;
	unsigned int nbp;
	std::vector<Noeud*> tab_liens;
	double access;
	bool in;
	unsigned int parent;
	Couleur paint;
	
	std::vector<Noeud*> short_path_tran;
	std::vector<Noeud*> short_path_prod;
		
	// MTA
	void init_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn, size_t i);
	void sort_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn);
	unsigned int djikstra(std::vector<int>& queue, const std::vector<Noeud*>& tn, 
						  Scenario_djikstra& scenario, int& cmt_tab, 
						  unsigned int nb_p, unsigned int nb_t);
	void recherche_voisins(std::vector<int>& queue, const std::vector<Noeud*>& tn, 
						   unsigned int nd_min);
	double temps_lien(Noeud* b) const;	
	unsigned int find_min_access(const std::vector<int>& queue, 
							     const std::vector<Noeud*>& tn);
};

class Logement : public Noeud 
{
	
public:
	Logement(unsigned int id,double x, double y, unsigned int capacite);
	~Logement() override;
	bool maxi_link() const override;
	double getSpeed() const override { return speed; }
	std::string getType() const override { return logement; }
	
	void controle_djikstra(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
						   Scenario_djikstra& scenario, size_t i, int& cmt_tab, 
						   double& cmt_mta, unsigned int nb_p, unsigned int nb_t) 
						   override;
	void controle_for_tran(unsigned int& nd, std::vector<int>& queue, 
						   const std::vector<Noeud*>& tn, Scenario_djikstra& scenario, 
						   size_t i, int& cmt_tab, double& cmt_mta, unsigned int nb_p, 
						   unsigned int nb_t);
	void controle_for_prod(unsigned int& nd, std::vector<int>& queue, 
						   const std::vector<Noeud*>& tn, Scenario_djikstra& scenario,
						   size_t i, int& cmt_tab, double& cmt_mta, unsigned int nb_p, 
						   unsigned int nb_t);
						   
	void draw_noeud() const override;
	void draw_path(Couleur paint) const override;
	void draw_path_type(std::vector<Noeud*> tab, Couleur paint_f) const;
	
	void path_prod(const std::vector<Noeud*>& tn, unsigned int nd);
	void path_tran(const std::vector<Noeud*>& tn, unsigned int nd);
	
private:
	double speed;

};

class Transport : public Noeud
{
public:
	Transport(unsigned int id,double x, double y, unsigned int capacite);
	~Transport() override;
	double getSpeed() const override { return speed; }
	std::string getType() const override { return transport; }
	void draw_noeud() const override;

private:
	double speed;
};

class Production : public Noeud
{
public:
	Production(unsigned int id,double x, double y, unsigned int capacite);
	~Production() override;
	double getSpeed() const override { return speed; }
	std::string getType() const override { return production; }
	void draw_noeud() const override;

private:
	double speed;

};

double short_path(const std::vector<Noeud*>& tn,unsigned int nb_p, 
				  unsigned int nb_t);			  // MTA pour être appelé par ville
double cout_infra(const std::vector<Noeud*>& tn); // CI pour être appelé par ville

void clean_vector_erase(std::vector<Noeud*>& list, unsigned int index);

#endif
