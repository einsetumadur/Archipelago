/**\
 * \name    noeud.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version Rendu 2
 */

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "noeud.h"
#include "tools.h"
#include "error.h"
#include "constantes.h"

using namespace std;

Noeud::Noeud(unsigned int id, double x, double y, unsigned int capacite): 
             uid(id), batiment{ {x,y}, sqrt(capacite)}, nbp(capacite)
{
} 

Noeud::~Noeud()
{
}

Logement::Logement(unsigned int id,double x, double y, unsigned int capacite):
				   Noeud(id,x,y,capacite), speed(default_speed)
{
}

Logement::~Logement()
{
}
			
Transport::Transport(unsigned int id,double x, double y, unsigned int capacite):
					 Noeud(id,x,y,capacite), speed(fast_speed)
{
}

Transport::~Transport()
{
}

Production::Production(unsigned int id,double x, double y, unsigned int capacite):
					   Noeud(id,x,y,capacite), speed(default_speed)
{
}

Production::~Production()
{
}

string Noeud::print()
{
	return to_string(uid)+" "+to_string(getX())+" "+to_string(getY())+" "+
		   to_string(nbp);
}
    
string Logement::getType() const 
{
	return logement;
}
string Production::getType() const 
{
	return production;
}
string Transport::getType() const 
{
	return transport;
}

unsigned int Noeud::getNbp() const
{
    return nbp;
}

double Noeud::getX() const 
{
	return batiment.centre.pos_x;
}
double Noeud::getY() const 
{
	return batiment.centre.pos_y;
}
double Noeud::getRayon() const 
{
	return batiment.rayon;
}
Cercle Noeud::getBatiment() const  
{
	return batiment;
}
unsigned int Noeud::getUid() const 
{
	return uid;
}

vector<Noeud*> Noeud::getLiens() const
{
	return tab_liens;
}

double Noeud::getAccess() const 
{
	return access;
}

unsigned int Noeud::getParent() const
{
	return parent;
}

bool Noeud::getIn() const
{
	return in;
}

double Production::getSpeed() const
{
	return speed;
}

double Logement::getSpeed() const
{
	return speed;
}

double Transport::getSpeed() const
{
	return speed;
}

void Noeud::updateIn(bool b)
{
	in = b;
}

void Noeud::ajout_lien(Noeud* b) 
{
	tab_liens.push_back(b);
}

/* fonctions errors */
bool Noeud::test_uid() const
{
	if(uid == no_link)
    {
		cout << error::reserved_uid();
		return true;
	}	
	else 	return false;
}

bool Noeud::test_nbp() const
{
	if(nbp < min_capacity) 
	{
		cout << error::too_little_capacity(nbp);
		return true;
	}
	else if(nbp > max_capacity)
	{
		cout << error::too_much_capacity(nbp);
		return true;
	}
	
	return false;
}

bool Noeud::multiple_link(Noeud* b) const
{
	for(size_t i(0) ; i < tab_liens.size() ; ++i) 
	{
		if(tab_liens[i] == b)	return true;
	}
	
	return false;
}

bool Noeud::maxi_link() const
{
	return false;
}

bool Logement::maxi_link() const
{
	if(tab_liens.size() == max_link)		return true;
	else 									return false;
}

bool Noeud::collis_lien_quartier(Noeud* lien_a, Noeud* lien_b) const
{	
	if(not(this == lien_a) and not(this == lien_b)) 
	{
		if(champ(lien_a->batiment, lien_b->batiment, this->batiment)) 
		{
			Point p = {lien_a->getX(), lien_a->getY()};
			Seg_droite d = {p, {lien_b->getX() - lien_a->getX(), 
								lien_b->getY() - lien_a->getY()}};
			if(collision_droite_cercle(this->batiment, d))	return true;
		}
	}
	
	return false;
} 

/* CI */
double cout_infra(const vector<Noeud*>& tn)
{
	double cmt(0);
	
	if(tn.size() == 0)	return 0;
	
	for(auto nd : tn) {
		nd->updateIn(true);
	}
	
	for(auto nd : tn) {
		for(auto nd_lien : nd->getLiens()) {
			if(nd_lien->getIn() == true) 	nd->calcul_ci(nd_lien, cmt);
		}
		nd->updateIn(false);
	}
	
	return cmt;	
}

void Noeud::calcul_ci(Noeud* nd_lien, double& cmt)	const
{
	double dist = norme({this->getX() - nd_lien->getX(), 
						 this->getY() - nd_lien->getY()});

	if(this->nbp < nd_lien->nbp)
	{
		if(this->getSpeed() == fast_speed and 
		   nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*this->nbp;
		else 	cmt += dist*default_speed*(this->nbp);
	}
	else if (this->nbp > nd_lien->nbp)
	{
		if(this->getSpeed() == fast_speed and 
		   nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*nd_lien->nbp;
		else 	cmt += dist*default_speed*(nd_lien->nbp);
	}
	else
	{
		if(this->getSpeed() == fast_speed and 
		   nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*this->nbp;
		else 	cmt += dist*default_speed*(this->nbp);
	}
}

/* MTA */
double short_path(const vector<Noeud*>& tn)
{
	double cmt(0);
	int scenario(scen_aleatoire);
	int compteur(0);
	vector<int> queue(tn.size());
	
	for(size_t i(0) ; i < tn.size() ; ++i) {
		scenario = scen_aleatoire;
		compteur = 0;
		if(tn[i]->getType() == logement) 		tn[i]->diff_cas(queue, tn, scenario, 
																i, compteur, cmt);		
	}
	
	return (cmt);
}

void Noeud::diff_cas(vector<int>& queue, const vector<Noeud*>& tn,  
					 int& scenario, size_t i, int& compteur, double& cmt) 
{
}

// distingue les cas selon le type du noeud renvoyé par Djikstra
// la variable int scenario impose quel type de noeud on cherche dans Djikstra
void Logement::diff_cas(vector<int>& queue, const vector<Noeud*>& tn,  
						int& scenario, size_t i, int& compteur, double& cmt) 
{
	init_queue(queue, tn, i);
	sort_queue(queue, tn);

	unsigned int nd(0);
	nd = djikstra(queue, tn, scenario, compteur);

	if(nd != no_link and tn[nd]->getType() == transport)
	{
		cmt += tn[nd]->getAccess();
		path_tran(tn, nd);
		
		scenario = scen_production;
		nd = djikstra(queue, tn, scenario, compteur);
		if(not(nd == no_link))	
		{ 
			cmt += tn[nd]->getAccess(); 
			path_prod(tn, nd);
		}
	}
	else if(nd != no_link and tn[nd]->getType() == production)
	{
		cmt +=  tn[nd]->getAccess();
		path_prod(tn, nd);
		
		scenario = scen_transport;
		nd = djikstra(queue, tn, scenario, compteur);
		if(not(nd == no_link))	
		{
			cmt+= tn[nd]->getAccess(); 
			path_tran(tn, nd);
		}
	}							
}

// applique l'algorithme de Djikstra : noeud le plus proche
unsigned int Noeud::djikstra(vector<int>& queue, const vector<Noeud*>& tn, 
							 int& scenario, int& compteur)
{
	unsigned int nd_min(0);
	while(compteur < tn.size())
	{
		nd_min = find_min_access(queue, tn);
		if(tn[nd_min]->getType() != logement)
		{
			if(not(scenario))
			{
				tn[nd_min]->in = false;
				if(tn[nd_min]->getType() != production)		tn[nd_min]->
															recherche_voisins
															(queue, tn, nd_min);
				return nd_min;
			}
			switch(scenario)
			{
				case scen_production: 
					if(tn[nd_min]->getType() == production)
					{
						tn[nd_min]->in = false;
						return nd_min;
					}
					break;
				case scen_transport: 
					if(tn[nd_min]->getType() == transport)
					{
						tn[nd_min]->in = false;
						return nd_min;
					}
					break;
			}
		}
		tn[nd_min]->in = false;
		compteur++;

		if(tn[nd_min]->getType() != production)		tn[nd_min]->recherche_voisins
																(queue, tn, nd_min);
	}
	
	return no_link;
}

// update les valeurs des voisins du noeud, s'ils sont plus proches
void Noeud::recherche_voisins(vector<int>& queue, const vector<Noeud*>& tn, 
							  unsigned int nd_min)
{
	double alt(0);
	for(auto lien : tab_liens) {
		if(lien->in) 
		{
			alt = getAccess() + temps_lien(lien);					
			if(lien->access > alt) 
			{
				lien->access = alt;
				lien->parent = nd_min;
				sort_queue(queue, tn);
			}
		}
	}
}

void Noeud::init_queue(vector<int>& queue, const vector<Noeud*>& tn, size_t i)
{
	for(size_t j(0) ; j < tn.size(); ++j) {
		tn[j]->access = infinite_time;
		tn[j]->parent = no_link;
		tn[j]->in = true;
		queue[j] = j;
	}
	
	tn[i]->access = 0;							
	tn[i]->in = true;
	tn[i]->parent = no_link;
	queue[i] = i;	
}

void Noeud::sort_queue(vector<int>& queue, const vector<Noeud*>& tn)
{
	vector<double> tab_access;

	for(size_t k(0) ; k < tn.size() ; ++k) {
		tab_access.push_back(tn[queue[k]]->access);
	}

	bool swap(true);
	double tmp(0);
	double mem(0);
	for(size_t k(0) ; k < tab_access.size() - 1 and swap; ++k) {
		if(tab_access[k+1] < tab_access[k])
		{
			while(tab_access[k+1] < tab_access[k]) {
				tmp = tab_access[k+1];
				mem = queue[k+1];
				
				tab_access[k+1] = tab_access[k];
				queue[k+1] = queue[k];
				
				tab_access[k] = tmp;
				queue[k] = mem;
				if(k != 0 )		--k;
			}
			swap = false;
		}
	}
}

// trouve le nd le plus proche à true
unsigned int Noeud::find_min_access(const vector<int>& queue, 
									const vector<Noeud*>& tn)
{
	for(size_t k(0) ; k < tn.size() ; ++k) {
		if(tn[queue[k]]->in)	return queue[k];
	}
}

//	ajoute les noeuds du chemin le plus court
		// vers un noeud production
void Logement::path_prod(const vector<Noeud*>& tn, unsigned int nd)
{
	for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) {
		short_path_prod.push_back(p);
	}	
}
		// vers un noeud transport
void Logement::path_tran(const vector<Noeud*>& tn, unsigned int nd)
{
	for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) {
		short_path_tran.push_back(p);
	}
}

// temps de connexion d'un lien
double Noeud::temps_lien(Noeud* b) const
{
	double dist = norme({getX() - b->getX(), getY() - b->getY()});
							
	if ((getSpeed() == fast_speed) and 
		(b->getSpeed() == fast_speed))	return dist/fast_speed;
	else 								return dist/default_speed;
		
}

bool Noeud::operator==(const Noeud& nd) const 
{
	return (uid == nd.uid); 
}
