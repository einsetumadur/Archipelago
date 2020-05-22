/**\
 * \name    noeud.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 3 - Architecture 11 b1
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

constexpr int no_action(-5);  // nbr différent de no_link choisi aléatoirement
constexpr double angle_transport(sqrt(2)/2);

Noeud::Noeud(unsigned int id, double x, double y, unsigned int capacite): 
	uid(id), 
	batiment{{x,y}, sqrt(capacite)}, 
	nbp(capacite),
	paint(NOIR)
{
} 

Noeud::~Noeud()
{
}

Logement::Logement(unsigned int id,double x, double y, unsigned int capacite):
	Noeud(id,x,y,capacite), 
	speed(default_speed)
{
}

Logement::~Logement()
{
}
			
Transport::Transport(unsigned int id,double x, double y, unsigned int capacite):
	Noeud(id,x,y,capacite), 
	speed(fast_speed)
{
}

Transport::~Transport()
{
}

Production::Production(unsigned int id,double x, double y, unsigned int capacite):
	Noeud(id,x,y,capacite), 
	speed(default_speed)
{
}

Production::~Production()
{
}

void Noeud::reset_tab_liens() 
{
	tab_liens.clear();
}

string Noeud::print() const
{
	return to_string(uid)+" "+to_string(getX())+" "+to_string(getY())+" "+
		   to_string(nbp);
}

string Noeud::print_lien(Noeud* nd_lien) const 
{
	return to_string(uid)+" "+to_string(nd_lien->getUid());
}

void Noeud::updateIn(bool b)
{
	in = b;
}

void Noeud::updatePaint(Couleur c)
{
	paint = c;
}

void Noeud::setNBP(double met)
{
	batiment.rayon = sqrt(met); 
	nbp = met;
}

void Noeud::setCentre(Point p)
{
	batiment.centre = p;
}
	
void Noeud::ajout_lien(Noeud* b) 
{
	tab_liens.push_back(b);
}

void Noeud::disconnect(Noeud* node)
{
	for(unsigned int i = 0 ; i < tab_liens.size(); i++)
	{
		if(node == tab_liens[i])
		{
			clean_vector_erase(tab_liens,i);
			break;
		}
	}
}

///////////////////////////// Fonctions error //////////////////////////////////////
bool Noeud::test_uid() const
{
	if(uid == no_link)
    {
		cout << error::reserved_uid();
		return true;
	}	
	else  return false;
}

Type_error Noeud::test_nbp() const
{
	if(nbp < min_capacity) 
	{
		cout << error::too_little_capacity(nbp);
		return LITTLE_NBP;
	}
	else if(nbp > max_capacity)
	{
		cout << error::too_much_capacity(nbp);
		return BIG_NBP;
	}
	
	return NO_ERROR;
}

bool Noeud::multiple_link(Noeud* b) const
{
	for(size_t i(0) ; i < tab_liens.size() ; ++i) 
	{
		if(tab_liens[i] == b)  return true;
	}
	
	return false;
}

bool Noeud::maxi_link() const
{
	return false;
}

bool Logement::maxi_link() const
{
	if(tab_liens.size() == max_link)  return true;
	else  return false;
}

bool Noeud::collis_lien_quartier(Noeud* lien_a, Noeud* lien_b, double dist_min) const
{	
	if(not(this == lien_a) and not(this == lien_b)) 
	{
		if(champ(lien_a->batiment, lien_b->batiment, this->batiment)) 
		{
			Point p = {lien_a->getX(), lien_a->getY()};
			Seg_droite d = {p, {lien_b->getX() - lien_a->getX(), 
								lien_b->getY() - lien_a->getY()}};
			if(collision_droite_cercle(this->batiment, d, dist_min))  return true;
		}
	}
	
	return false;
} 

bool Noeud::is_connected_to(Noeud* node) const
{
	for(auto noeud : tab_liens)
	{
		if(noeud == node)  return true;
	}
	return false;
}

bool Noeud::is_under(double x, double y) const
{
	if(is_in_circle(x,y,batiment))	return true;
	else return false;
}

///////////////////////////// Section Dessin ///////////////////////////////////////

void Logement::draw_noeud() const 
{
	draw_cercle(getCentre(), getRayon(), paint);
}

void Transport::draw_noeud() const
{
	draw_cercle(getCentre(), getRayon(), paint);
	
	draw_ligne(getCentre(), {getX() + getRayon()*angle_transport, 
							 getY() + getRayon()*angle_transport}, paint);
	draw_ligne(getCentre(), {getX() + getRayon()*angle_transport, 
							 getY() - getRayon()*angle_transport}, paint);
	draw_ligne(getCentre(), {getX() - getRayon()*angle_transport, 
							 getY() - getRayon()*angle_transport}, paint);
	draw_ligne(getCentre(), {getX() - getRayon()*angle_transport, 
							 getY() + getRayon()*angle_transport}, paint);
	
	draw_ligne(getCentre(), {getX(), getY() + getRayon()}, paint);
	draw_ligne(getCentre(), {getX(), getY() - getRayon()}, paint);
	draw_ligne(getCentre(), {getX() + getRayon(), getY()}, paint);
	draw_ligne(getCentre(), {getX() - getRayon(), getY()}, paint);
}

void Production::draw_noeud() const
{
	draw_cercle(getCentre(), getRayon(), paint);
	draw_rectangle(getCentre(), getRayon(), paint);
}

void Noeud::draw_path(Couleur paint) const
{
}

void Logement::draw_path(Couleur paint_f) const
{
	// chemin le plus court pour production :
	for(auto nd : short_path_prod) 
	{
		nd->updatePaint(paint_f);
	}
	draw_path_type(short_path_prod, paint_f);
	
	// chemin le plus court pour transport : 
	for(auto nd : short_path_tran) 
	{
		nd->updatePaint(paint_f);
	}
	draw_path_type(short_path_tran, paint_f);
}

void Logement::draw_path_type(vector<Noeud*> tab, Couleur paint_f) const
{
	unsigned int p_size = tab.size();
	if(p_size == 1)
	{
		draw_ligne(tab[0]->getCentre(), getCentre(), VERT);
		tab[0]->draw_noeud();
		draw_noeud();
	}
	else if (p_size > 0)
	{
		draw_ligne(tab.back()->getCentre(), getCentre(), paint_f);
		tab.back()->draw_noeud();
		draw_noeud();
		for(size_t i(0) ; p_size != 0 && i != p_size - 1; ++i) 
		{
			draw_ligne(tab[i]->getCentre(), tab[i+1]->getCentre(), paint_f);
			tab[i]->draw_noeud();
			tab[i+1]->draw_noeud();
		}
	}	
}

///////////////////////////// Section CI ///////////////////////////////////////////

double cout_infra(const vector<Noeud*>& tn)
{
	double cmt(0);
	
	if(tn.size() == 0) return 0;
	
	for(auto nd : tn) 
	{
		nd->updateIn(true);
	}
	
	for(auto nd : tn) 
	{
		for(auto nd_lien : nd->getLiens()) 
		{
			if(nd_lien->getIn() == true)  nd->calcul_ci(nd_lien, cmt);
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
		   nd_lien->getSpeed() == fast_speed)  cmt += dist*fast_speed*this->nbp;
		else  cmt += dist*default_speed*(this->nbp);
	}
	else if (this->nbp > nd_lien->nbp)
	{
		if(this->getSpeed() == fast_speed and 
		   nd_lien->getSpeed() == fast_speed)  cmt += dist*fast_speed*nd_lien->nbp;
		else  cmt += dist*default_speed*(nd_lien->nbp);
	}
	else
	{
		if(this->getSpeed() == fast_speed and 
		   nd_lien->getSpeed() == fast_speed)  cmt += dist*fast_speed*this->nbp;
		else  cmt += dist*default_speed*(this->nbp);
	}
}

///////////////////////////// Section MTA //////////////////////////////////////////

double short_path(const vector<Noeud*>& tn, unsigned int nb_p, unsigned int nb_t)
{
	double cmt_mta(0);
	Scenario_djikstra scenario(scen_aleatoire);
	int cmt_tab(0);
	vector<int> queue(tn.size());
	
	for(size_t i(0) ; i < tn.size() ; ++i) 
	{
		scenario = scen_aleatoire;
		cmt_tab = 0;
		if(tn[i]->getType() == logement)  tn[i]->controle_djikstra(queue, tn, 
																   scenario, i, 
																   cmt_tab, cmt_mta, 
																   nb_p, nb_t);		
	}
	
	return (cmt_mta);
}

void Noeud::controle_djikstra(vector<int>& queue, const vector<Noeud*>& tn,  
					 Scenario_djikstra& scenario, size_t i, int& cmt_tab, 
					 double& cmt_mta, unsigned int nb_p, unsigned int nb_t) 
{
}

// distingue les cas selon le type du noeud renvoyé par Djikstra
// la variable int scenario impose quel type de noeud on cherche dans Djikstra
void Logement::controle_djikstra(vector<int>& queue, const vector<Noeud*>& tn,  
						Scenario_djikstra& scenario, size_t i, int& cmt_tab,
						double& cmt_mta, unsigned int nb_p, unsigned int nb_t) 
{
	init_queue(queue, tn, i);
	sort_queue(queue, tn);
	unsigned int nd(0);
	if(nb_p == 0 and nb_t == 0)  cmt_mta += infinite_time + infinite_time;
	else
	{		
		nd = djikstra(queue, tn, scenario, cmt_tab, nb_p, nb_t);
		if(tn[nd]->getType() == transport)
		{
			cmt_mta += tn[nd]->getAccess();
			path_tran(tn, nd);
			scenario = scen_production;
			nd = djikstra(queue, tn, scenario, cmt_tab, nb_p, nb_t);
			if(nd != no_link)	
			{ 
				cmt_mta += tn[nd]->getAccess(); 
				path_prod(tn, nd);
			}
			else  cmt_mta += infinite_time;
		}
		else if(tn[nd]->getType() == production)
		{
			cmt_mta +=  tn[nd]->getAccess();
			path_prod(tn, nd);
			scenario = scen_transport;
			nd = djikstra(queue, tn, scenario, cmt_tab, nb_p, nb_t);
			if(nd != no_link)	
			{
				cmt_mta += tn[nd]->getAccess(); 
				path_tran(tn, nd);
			}
			else  cmt_mta += infinite_time;
		}
		else  cmt_mta += infinite_time + infinite_time;
	}							
}

// applique l'algorithme de Djikstra : noeud le plus proche
unsigned int Noeud::djikstra(vector<int>& queue, const vector<Noeud*>& tn, 
							 Scenario_djikstra& scenario, int& cmt_tab, 
							 unsigned int nb_p, unsigned int nb_t)
{
	if(scenario_no_link(nb_t, nb_p, scenario) == no_link)  return no_link;
	
	unsigned int nd_min(0);
	while(cmt_tab < tn.size())
	{
		nd_min = find_min_access(queue, tn);
		if(tn[nd_min]->getType() != logement)
		{
			if(not(scenario))
			{
				tn[nd_min]->in = false;
				if(tn[nd_min]->getType() != production)	 tn[nd_min]->
														 recherche_voisins
														 (queue, tn, nd_min);
				return nd_min;
			}
			if (scenario_nd_min(tn, nd_min, scenario) == nd_min)  return nd_min;
		}
		tn[nd_min]->in = false;
		cmt_tab++;
		if(tn[nd_min]->getType() != production)  tn[nd_min]->recherche_voisins
															 (queue, tn, nd_min);
	}
	return no_link;
}

unsigned int Noeud::scenario_no_link(unsigned int nb_t, unsigned int nb_p, 
									 Scenario_djikstra& scenario)
{
	switch(scenario)
	{
		case scen_production: 
			if(nb_p == 0)  return no_link;
			break;
		case scen_transport: 
			if(nb_t == 0)  return no_link;
			break;
	}
	
	return no_action;
}
unsigned int Noeud::scenario_nd_min(const vector<Noeud*>& tn, unsigned int nd_min,
									Scenario_djikstra& scenario)
{
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
	
	return no_action;
}

// update les valeurs des voisins du noeud, s'ils sont plus proches
void Noeud::recherche_voisins(vector<int>& queue, const vector<Noeud*>& tn, 
							  unsigned int nd_min)
{
	double alt(0);
	for(auto lien : tab_liens) 
	{
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
	for(size_t j(0) ; j < tn.size(); ++j) 
	{
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
	vector<double> tab_access(tn.size());

	for(size_t k(0) ; k < tn.size() ; ++k) 
	{
		tab_access[k] = tn[queue[k]]->access;
	}

	bool swap(true);
	double tmp(0);
	double mem(0);
	for(size_t k(0) ; k < tab_access.size() - 1 and swap; ++k) 
	{
		if(tab_access[k+1] < tab_access[k])
		{
			while(tab_access[k+1] < tab_access[k]) 
			{
				tmp = tab_access[k+1];
				mem = queue[k+1];
				
				tab_access[k+1] = tab_access[k];
				queue[k+1] = queue[k];
				
				tab_access[k] = tmp;
				queue[k] = mem;
				if(k != 0 )  --k;
			}
			swap = false;
		}
	}
}

// trouve le nd le plus proche à true
unsigned int Noeud::find_min_access(const vector<int>& queue, 
									const vector<Noeud*>& tn)
{
	for(size_t k(0) ; k < tn.size() ; ++k) 
	{
		if(tn[queue[k]]->in)  return queue[k];
	}
}

//	ajoute les noeuds du chemin le plus court
		// vers un noeud production
void Logement::path_prod(const vector<Noeud*>& tn, unsigned int nd)
{
	if(short_path_prod.size() != 0 or nd == no_link)  
	{
		short_path_prod.clear();
	}
	if(nd != no_link)
	{
		vector<Noeud*> tab(tn.size(), nullptr);
		size_t i(0);
		for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) 
		{ 
			tab[i] = p;
			++i;
		}
		short_path_prod.resize(i);
		size_t j(0);
		while(j < i)
		{
			short_path_prod[j] = tab[j];
			++j;
		}
	}
}
		// vers un noeud transport
void Logement::path_tran(const vector<Noeud*>& tn, unsigned int nd)
{
	if(short_path_tran.size() != 0 or nd == no_link)  
	{
		short_path_tran.clear();
	}
	if(nd != no_link)
	{
		vector<Noeud*> tab(tn.size(), nullptr);
		size_t i(0);
		for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) 
		{ 
			tab[i] = p;
			++i;
		}
		short_path_tran.resize(i);
		size_t j(0);
		while(j < i)
		{
			short_path_tran[j] = tab[j];
			++j;
		}
	}
}
// temps de connexion d'un lien
double Noeud::temps_lien(Noeud* b) const
{
	double dist = norme({getX() - b->getX(), getY() - b->getY()});
							
	if ((getSpeed() == fast_speed) and 
		(b->getSpeed() == fast_speed))  return dist/fast_speed;
	else  return dist/default_speed;
}

bool Noeud::operator==(const Noeud& nd) const 
{
	return (uid == nd.uid); 
}

void clean_vector_erase(std::vector<Noeud*>& list , unsigned int index)
{
	//swap node to be erased with the last one in the array then pop back
	if(!( index == list.size()-1)) 
	{
		Noeud* tmpN = list.back();
		list.back() = list[index];
		list[index] = tmpN;
	}
	list.pop_back();
}
