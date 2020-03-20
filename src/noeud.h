#ifndef NOEUD_H
#define NOEUD_H

enum Type_quartier {PRODUCTION, TRANSPORT, LOGEMENT};
class Noeud 
{
public:
	Noeud(unsigned int id,double x, double y, unsigned int capacite, 
												Type_quartier type);
	//FAIRE UN DESTRUCTEUR
	string print();
	unsigned int get_nbp();
	double getx() const ;
	double gety() const ;
	double getrayon() const ;
	Cercle getQuartier() const ;
	unsigned int getUid() const ;
	Type_quartier getType() const;	
	void add_lien(Noeud* B);	
	bool operator==(const Noeud& nd) const;
	string test_max_link();
	int verif_capacite(Noeud node);
	string test_nbp(unsigned int nbp, double rayon);
private:
	Cercle quartier;
	unsigned int uid;
	unsigned int nbp;
	Type_quartier type;
	vector<Noeud*> liens;
};


string test_lien_quartier(Noeud A, Noeud C, Noeud B);	
string test_coll_quartier(vector<Noeud> ensN);
string test_nbp(unsigned int nbp, double rayon);

#endif
