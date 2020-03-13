
#include<iostream>
#include<string>
#include <vector>
#include"tools.h"
#include"error.h"
using namespace std;

#ifndef NOEUD_H
#define NOEUD_H

enum type_quartier {production,transport,logements};
class Noeud{
    public:
        Noeud(unsigned int id,double x, double y, unsigned int capacite, type_quartier type);
        string print();
        // getters
        unsigned int get_nbp();
        double getx() const ;
        double gety() const ;
        double getrayon() const ;
        Cercle getQuartier() const ;
        unsigned int getUid() const ;
        type_quartier getType() const;
        // setters
        void add_lien(Noeud* B);
        
        bool operator==(const Noeud& nd) const ;

        int verif_uid_quartier(vector<Noeud> ensemble);
        int verif_capacite(Noeud node);
        string test_nbp(unsigned int nbp, double rayon);
        int verif_uid_interquartier(vector<Noeud> logement, vector<Noeud> production, vector<Noeud> transport);


    private:
        Cercle quartier;
        unsigned int uid;
        unsigned int nbp;
        type_quartier type;
        vector<Noeud*> liens;
};


bool test_lien_quartier(Noeud A, Noeud C, Noeud B) {	
string test_coll_quartier(vector<Noeud> ensN);
string test_nbp(unsigned int nbp, double rayon);


#endif

