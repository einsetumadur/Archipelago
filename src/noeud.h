#include<iostream>
#include<string>
#include <vector>
#include"tools.h"
#include"error.h"
using namespace std;

#ifndef NOEUD_H
#define NOEUD_H

class Noeud{
    public:
        Noeud(unsigned int id,double x, double y, unsigned int capacite, vector<Noeud*> liens);
        string print();
        // getters
        unsigned int get_nbp();
        double getx() const ;
        double gety() const ;
        double getrayon() const ;
        Cercle getQuartier() const ;
        unsigned int getUid() const ;
        
        bool operator==(const Noeud& nd) const ;

        int verif_uid_quartier(vector<Noeud> ensemble);
        int verif_capacite(Noeud node);
	string test_nbp(unsigned int nbp, double rayon);
	int verif_uid_interquartier(vector<Noeud> logement, vector<Noeud> production, vector<Noeud> transport);
	string test_duplicite_lien();


    protected: 
        Cercle quartier;
        unsigned int uid;
        unsigned int nbp;
        vector<Noeud*> liens;
};

class Lien {
	
	private:
		Noeud* noeud1;
		Noeud* noeud2;
		Seg_droite trait;
	public: 
		Lien(Noeud* nd1, Noeud* nd2);
	
		//getters
		Seg_droite getTrait();
};

/*class Lien{
    public:
        Lien(Noeud nd1, Noeud nd2);
    private:
        Noeud* noeud1;
        Noeud* noeud2;
       // Seg_droite d;
}; */ // J'enleve ça pcq : je peux pas utiliser la class Lien comme attribut de Noeud, vu que c'est déclaré après. Ca fout le bordel pr la suite du projet, donc j'ai juste déclaré un vector de ptr de Noeud comme attribut


class Logement : public Noeud {
	public: 
		Logement(unsigned int id, double x, double y, unsigned int capacite, vector<Noeud*> liens);
		bool test_max_link(size_t nb_liens);

};

class Production : public Noeud {

};

class Transport : public Noeud {
	
};

string test_lien_quartier(vector<Lien> ensL, vector<Noeud> ensN);
string test_coll_quartier(vector<Noeud> ensN);
string test_nbp(unsigned int nbp, double rayon);


#endif
