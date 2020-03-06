#include<iostream>
#include<string>
#include"noeud.h"
using namespace std;


Noeud::Noeud(unsigned int id,double x, double y, unsigned int capacite): uid(id),nbp(capacite),posx(x),posy(y)
{

}
    
unsigned int Noeud::capacite()
{
    return nbp;
}

string Noeud::print()
{
    return to_string(uid)+" "+to_string(posx)+" "+to_string(posy)+" "+to_string(nbp);
}

double Noeud::getx(){return posx;}
double Noeud::gety(){return posy;}
