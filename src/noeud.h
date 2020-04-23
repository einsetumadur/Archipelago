/**\
 * \name   noeud.h
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version Rendu 2
 */
 
#ifndef NOEUD_H
#define NOEUD_H

#include <iostream>
#include "tools.h"
#include <string>
#include <vector>

const std::string logement("Logement");
const std::string production("Production");
const std::string transport("Transport");
constexpr int scen_aleatoire(0);
constexpr int scen_production(1);
constexpr int scen_transport(2);

enum Type_error {NO_ERROR, RES_U, LITTLE_NBP, BIG_NBP, SELF_L_N, L_VAC, MULT_S_L, 
				 MAX_L, N_L_SUP, ID_U, N_N_SUP};
				 
class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite);
	virtual ~Noeud() = 0;
	std::string print();
	// getters & setters 
	unsigned int getNbp() const;
	double getX() const ;
	double getY() const ;
	double getRayon() const ;
	Cercle getBatiment() const;
	unsigned int getUid() const;
	std::vector<Noeud*> getLiens() const;
	double getAccess() const ;
	unsigned int getParent() const ;
	bool getIn() const ;
	virtual double getSpeed() const = 0;
	virtual std::string getType() const = 0;
	void updateIn(bool b);	
	void ajout_lien(Noeud* b);
	// fonctions de tests
	bool test_uid() const;
	Type_error test_nbp() const;
	bool multiple_link(Noeud* b)const;
	virtual bool maxi_link() const;
	bool collis_lien_quartier(Noeud* lien_a, Noeud* lien_b) const; 
	// MTA
	virtual void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
						  int& scenario, size_t i, int& cmt_tab, double& cmt_mta,
						   unsigned int nb_p, unsigned int nb_t);
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
	
	// MTA
	void init_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn, 
					size_t i);
	void sort_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn);
	unsigned int djikstra(std::vector<int>& queue, const std::vector<Noeud*>& tn, 
						  int& scenario, int& cmt_tab, unsigned int nb_p, 
						  unsigned int nb_t);
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
	double getSpeed() const override;
	std::string getType() const override;
	void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
				  int& typ, size_t i, int& cmt_tab, double& cmt_mta, 
				  unsigned int nb_p, unsigned int nb_t) override;
	void path_prod(const std::vector<Noeud*>& tn, unsigned int nd);
	void path_tran(const std::vector<Noeud*>& tn, unsigned int nd);

protected:
	double speed;
	std::vector<Noeud*> short_path_tran;
	std::vector<Noeud*> short_path_prod;
		
};

class Transport : public Noeud
{
public:
	Transport(unsigned int id,double x, double y, unsigned int capacite);
	~Transport() override;
	double getSpeed() const override;
	std::string getType() const override;

private:
	double speed;
};

class Production : public Noeud
{
public:
	Production(unsigned int id,double x, double y, unsigned int capacite);
	~Production() override;
	double getSpeed() const override;
	std::string getType() const override;
	
private:
	double speed;

};

double short_path(const std::vector<Noeud*>& tn,unsigned int nb_p, 
				  unsigned int nb_t); // MTA pour être appelé par ville
double cout_infra(const std::vector<Noeud*>& tn); // CI pour être appelé par ville

#endif
