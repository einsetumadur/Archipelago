/**\
 * \name   noeud.h
 * \author Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
 */
 
#ifndef NOEUD_H
#define NOEUD_H

#include <iostream>
#include "tools.h"
#include <string>
#include <vector>

class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite);
	virtual ~Noeud() = 0;
	
	unsigned int getNbp();
	double getX() const ;
	double getY() const ;
	double getRayon() const ;
	Cercle getBatiment() const;
	unsigned int getUid() const;
	std::vector<Noeud*> getLiens() const;

	void test_nbp();
	void ajout_lien(Noeud* b);
	bool multiple_link(Noeud* b);
	virtual std::string getType() const = 0;	
	virtual bool maxi_link();
	bool collis_lien_quartier(Noeud* lien_a, Noeud* lien_b); 
	bool operator==(const Noeud& nd) const;
	
	virtual void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
					int& typ, size_t i, int& compteur, double& cmt) = 0;
	void voisins(std::vector<int>& queue, const std::vector<Noeud*>& tn, unsigned int nd_min);
	double temps_lien(Noeud* b) const;	

	double getAccess() const ;
	unsigned int getParent() const ;
	bool getIn() const ;
	virtual double getSpeed() const = 0;
	
	void updateIn(bool b);
	void updateAccess(double a);
	void updateParent(unsigned int p);
		
	void calcul(Noeud* nd_lien, double& cmt);
		
	// setters attention : accès PUBLIC au chgmnt des attributs
	void init_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn, size_t i);

	
protected:
	Cercle batiment;
	unsigned int uid;
	unsigned int nbp;
	std::vector<Noeud*> liens;
	
	double access;
	bool in;
	unsigned int parent;

};


class Logement : public Noeud 
{
	
public:
	Logement(unsigned int id,double x, double y, unsigned int capacite);
	~Logement() override;
	
	bool maxi_link() override;
	double getSpeed() const override;
	void setShort_path_tran(Noeud* nd);
	void setShort_path_prod(Noeud* nd);
	std::string getType() const override;
	void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
				  int& typ, size_t i, int& compteur, double& cmt) override;
	unsigned int djikstra(std::vector<int>& queue, const std::vector<Noeud*>& tn, 
						  int& typ, int& compteur);
	void compute_p(const std::vector<Noeud*>& tn, unsigned int nd);
	void compute_t(const std::vector<Noeud*>& tn, unsigned int nd);

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
	void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
				  int& typ, size_t i, int& compteur, double& cmt) override;
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
	void diff_cas(std::vector<int>& queue, const std::vector<Noeud*>& tn,  
				  int& typ, size_t i, int& compteur, double& cmt) override;
private:
	double speed;

};

double short_path(const std::vector<Noeud*>& tn);
void sort_queue(std::vector<int>& queue, const std::vector<Noeud*>& tn);
unsigned int find_min_access(const std::vector<int>& queue, const std::vector<Noeud*>& tn);

double cout_infra(const std::vector<Noeud*>& tn);

#endif
