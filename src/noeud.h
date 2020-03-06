#include<iostream>
#include<string>

#ifndef NOEUD_H
#define NOEUD_H

class Lien{
    public:
        Lien(Noeud* nd1, Noeud* nd2):noeud1(nd1),noeud2(nd2){}
    private:
        Noeud* noeud1;
        Noeud* noeud2;
        double longeur;
};
class Noeud{
    public:
        Noeud(unsigned int id,double x, double y, unsigned int capacite);
        unsigned int capacite();
        std::string print();
        double getx();
        double gety();
    private: 
        unsigned int uid;
        unsigned int nbp;
        double posx,posy;
};

double distance(Noeud nd1, Noeud nd2);

#endif NOEUD_H