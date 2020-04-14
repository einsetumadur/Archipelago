/**\
 * \name    noeud.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    march 2020
 * \version 1.0
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
    test_nbp();
    if(uid == no_link)
    {
		cout << error::reserved_uid();
		exit(-1);
	}		
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
    
string Logement::getType() const {
	
	return "LOGE";
}
string Production::getType() const {
	
	return "PROD";
}
string Transport::getType() const {
	
	return "TRAN";
}

unsigned int Noeud::getNbp()
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
	return liens;
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
	
void Noeud::updateAccess(double a)
{
	access = a;
}

void Noeud::updateIn(bool b)
{
	in = b;
}

void Noeud::updateParent(unsigned int p)
{
	parent = p;
}

void Logement::setShort_path_prod(Noeud* nd)
{
	short_path_prod.push_back(nd);
}

void Logement::setShort_path_tran(Noeud* nd)
{
	short_path_tran.push_back(nd);
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

void Noeud::test_nbp() 
{
	if(nbp < min_capacity) 
	{
		cout << error::too_little_capacity(nbp);
		exit(-1);
	}
	else if(nbp > max_capacity)
	{
		cout << error::too_much_capacity(nbp);
		exit(-1);
	}
}

void Noeud::ajout_lien(Noeud* b) 
{
	liens.push_back(b);
}

bool Noeud::multiple_link(Noeud* b) 
{
	for(size_t i(0) ; i < liens.size() ; ++i) 
	{
		if(liens[i] == b)	return true;
	}
	
	return false;
}

bool Noeud::maxi_link()
{
	return false;
}

bool Logement::maxi_link()
{
	if(liens.size() == max_link)		return true;
	else 								return false;
}

bool Noeud::collis_lien_quartier(Noeud* lien_a, Noeud* lien_b) 
{	
	if( not(this == lien_a) and not(this == lien_b) ) 
	{
		if(champ(lien_a->batiment, lien_b->batiment, this->batiment)) 
		{
			Point p = { lien_a->getX(), lien_a->getY() };
			Seg_droite d = { p, {lien_b->getX() - lien_a->getX(), 
								lien_b->getY() - lien_a->getY()}};
			if(collision_droite_cercle(this->batiment, d))	return true;
		}
	}
	
	return false;
} 

double cout_infra(const vector<Noeud*>& tn)
{
	double cmt(0);
	
	if(tn.size() == 0)	return 0;
	
	for(auto nd : tn) {
		nd->updateIn(true);
	}
	
	for(auto nd : tn) {
		for(auto nd_lien : nd->getLiens()) {
			if(nd_lien->getIn() == true) 	nd->calcul(nd_lien, cmt);
		}
		nd->updateIn(false);
	}
	
	return cmt;	
}

void Noeud::calcul(Noeud* nd_lien, double& cmt)
{
	double dist = norme({this->getX() - nd_lien->getX(), this->getY() - nd_lien->getY()});

				if(this->nbp < nd_lien->nbp)
				{
					if(this->getSpeed() == fast_speed and nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*this->nbp;
					else 	cmt += dist*default_speed*(this->nbp);
				}
				else if (this->nbp > nd_lien->nbp)
				{
					if(this->getSpeed() == fast_speed and nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*nd_lien->nbp;
					else 	cmt += dist*default_speed*(nd_lien->nbp);
				}
				else
				{
					if(this->getSpeed() == fast_speed and nd_lien->getSpeed() == fast_speed)	cmt += dist*fast_speed*this->nbp;
					else 	cmt += dist*default_speed*(this->nbp);
				}
}

void Transport::diff_cas(vector<int>& queue, const vector<Noeud*>& tn,  
						int& typ, size_t i, int& compteur, double& cmt) 
{
}

void Production::diff_cas(vector<int>& queue, const vector<Noeud*>& tn,  
						int& typ, size_t i, int& compteur, double& cmt) 
{
}

void Logement::diff_cas(vector<int>& queue, const vector<Noeud*>& tn,  
						int& typ, size_t i, int& compteur, double& cmt) 
{
	init_queue(queue, tn, i);
	sort_queue(queue, tn);

	unsigned int nd(0);
	nd = djikstra(queue, tn, typ, compteur);

	if( nd != no_link and tn[nd]->getType() == "TRAN")
	{
		cmt += tn[nd]->getAccess();
		compute_t(tn, nd);
		
		typ = 1;
		nd = djikstra(queue, tn, typ, compteur);
		if(not(nd == no_link))	
		{ 
			cmt += tn[nd]->getAccess(); 
			compute_p(tn, nd);
		}
		
	}
	else if(nd != no_link and tn[nd]->getType() == "PROD")
	{
		cmt +=  tn[nd]->getAccess();
		compute_p(tn, nd);
		
		typ = 2;
		nd = djikstra(queue, tn, typ, compteur);
		if(not(nd == no_link))	
		{
			cmt+= tn[nd]->getAccess(); 
			compute_t(tn, nd);
		}
			
	}							
}

double short_path(const vector<Noeud*>& tn)
{
	double cmt(0);
	int typ(0);
	int compteur(0);
	vector<int> queue(tn.size());
	
	for(size_t i(0) ; i < tn.size() ; ++i) {
		typ = 0;
		compteur = 0;
		if(tn[i]->getType() == "LOGE") 		tn[i]->diff_cas(queue, tn, typ, i, compteur, cmt);		
	}
	
	return (cmt);
}

unsigned int Logement::djikstra(vector<int>& queue, const vector<Noeud*>& tn, int& typ, int& compteur)
{
	unsigned int nd_min(0);
	while(compteur < tn.size())
	{
		nd_min = find_min_access(queue, tn);
		if(tn[nd_min]->getType() != "LOGE")
		{
			if(not(typ))
			{
				tn[nd_min]->updateIn(false);
				if(tn[nd_min]->getType() != "PROD")		tn[nd_min]->voisins(queue, tn, nd_min);
				return nd_min;
			}
			switch(typ)
			{
				case 1: if(tn[nd_min]->getType() == "PROD")
						{
							tn[nd_min]->updateIn(false);
							return nd_min;
						}
						break;
				case 2: if(tn[nd_min]->getType() == "TRAN")
						{
							tn[nd_min]->updateIn(false);
							return nd_min;
						}
						break;
			}
		}
		tn[nd_min]->updateIn(false);
		compteur++;

		if(tn[nd_min]->getType() != "PROD")		tn[nd_min]->voisins(queue, tn, nd_min);
	}
	
	return no_link;
}

void Noeud::voisins(vector<int>& queue, const vector<Noeud*>& tn, unsigned int nd_min)
{
	double alt(0);
	for(auto lien : getLiens()) {
		if(lien->getIn()) 
		{
			alt = getAccess() + temps_lien(lien);					
			if(lien->getAccess() > alt) 
			{
				lien->updateAccess(alt);
				lien->updateParent(nd_min);
				sort_queue(queue, tn);
			}
		}
	}
}

void Noeud::init_queue(vector<int>& queue, const vector<Noeud*>& tn, size_t i)
{
	for(size_t j(0) ; j < tn.size(); ++j) {
		if(j == i)	continue;
		tn[j]->updateAccess(infinite_time);
		tn[j]->updateParent(no_link);
		tn[j]->updateIn(true);
		queue[j] = j;
	}
	
	tn[i]->updateAccess(0);							
	tn[i]->updateIn(true);
	tn[i]->updateParent(no_link);
	queue[i] = i;	
}

void sort_queue(vector<int>& queue, const vector<Noeud*>& tn)
{
	vector<double> accesses;

	for(size_t k(0) ; k < tn.size() ; ++k) {
		accesses.push_back(tn[queue[k]]->getAccess());
	}

	bool swap(true);
	double tmp(0);
	double mem(0);
	for(size_t k(0) ; k < accesses.size() - 1 and swap; ++k) {
		if(accesses[k+1] < accesses[k])
		{
			while(accesses[k+1] < accesses[k]) {
				tmp = accesses[k+1];
				mem = queue[k+1];
				
				accesses[k+1] = accesses[k];
				queue[k+1] = queue[k];
				
				accesses[k] = tmp;
				queue[k] = mem;
				if(k != 0 )		--k;
			}
			swap = false;
		}
	}
}

unsigned int find_min_access(const vector<int>& queue, const vector<Noeud*>& tn)
{
	for(size_t k(0) ; k < tn.size() ; ++k) {
		if(tn[queue[k]]->getIn())	return queue[k];
	}
}

void Logement::compute_p(const vector<Noeud*>& tn, unsigned int nd)
{
	for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) {
		setShort_path_prod(p);
	}
		
}

void Logement::compute_t(const vector<Noeud*>& tn, unsigned int nd)
{
	for(Noeud* p = tn[nd] ; p->getParent() != no_link ; p = tn[p->getParent()]) {
		setShort_path_tran(p);
	}
	
}

double Noeud::temps_lien(Noeud* b) const
{
	double dist = norme({getX() - b->getX(),
							getY() - b->getY()});
							
	if ((getSpeed() == fast_speed) and (b->getSpeed() == fast_speed))	return dist/fast_speed;
	else 	return dist/default_speed;
		
}

bool Noeud::operator==(const Noeud& nd) const 
{
	return (getX() == nd.getX() and getY() == nd.getY() and
			getRayon() == nd.getRayon() and uid == nd.uid); 
}
