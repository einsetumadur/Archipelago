/**\
 * \name    ville.cc
 * \author  Regamey Gilles & Zakeeruddin Sufyan 
 * \date    Mai 2020
 * \version Rendu 2 - Architecture 11 b1
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "ville.h"
#include "noeud.h"
#include "error.h"
#include "constantes.h"

using namespace std;

namespace
{
	Ville ville(true);
}

constexpr char nb[] = " # nb";
constexpr char nb_link[] = "# nb link";

Ville::Ville(bool val): 
	chargement_verif(val), 
	msg_error(NO_ERROR), 
	error_param_un(0), 
	error_param_deux(0), 
	nbp_nbL(0), 
	nbp_nbP(0), 
	nbp_nbT(0)
{
	grid.resize(1000,vector<tile>(1000));
	cout<<"gridsize set to "<<grid.size()<<endl;
}

Ville::~Ville()
{
}

void Ville::reset()
{
	if(not(quartiers.empty()))
	{
		for(auto nd : quartiers) 
		{
			nd->reset_tab_liens();
		}
		
		for(auto noeud : quartiers) 
		{
			delete noeud;
		}
		quartiers.clear();
	}
	chargement_verif = true;
	msg_error = NO_ERROR;
	error_param_un = 0; 
	error_param_deux = 0;
	nbp_nbL = 0; 
	nbp_nbP = 0;
	nbp_nbT = 0;
}

void Ville::chargement(char* nom_fichier)
{	
	string line;
	ifstream fichier(nom_fichier); 
	if(nom_fichier == nullptr)  cout << "no file selected" << endl; 
	else if(!fichier.fail()) 
	{	
		int etat = NBL; 
		while(getline(fichier >> ws,line) and chargement_verif) 
		{
			if(line[0] == '#')  continue;  
			decodage(line, etat);			
		}
		if(chargement_verif)  cout << error::success() << endl; 

		cout<<"[";
		for (size_t i = 0; i < occupied_uids.size() ; i++)
		{
			cout<<occupied_uids[i]<<",";
		}
		cout<<"]"<<endl;
		
	}
	else 	
	{
		cout << "unable to open file." << endl;
		chargement_verif = false;
	}
}

double Ville::mta()
{
	unsigned int nb_l = nb_type(logement);
	unsigned int nb_p = nb_type(production);
	unsigned int nb_t = nb_type(transport);

	if(quartiers.size() == 0 or nb_l == 0) 
	{
		cmt_mta = 0;
		return 0;
	}
	else 
	{
		cmt_mta = short_path(quartiers, nb_p, nb_t);
		return cmt_mta/nb_l;
	}
}

double Ville::enj()
{
	if(quartiers.size() == 0)  return 0;
	else  return (nbp_nbL - (nbp_nbT + nbp_nbP))/(nbp_nbL + nbp_nbP + nbp_nbT);
}

double Ville::ci()
{
	return cout_infra(quartiers);
}

///////////////////////////// Lecture fichier //////////////////////////////////////

void Ville::decodage(string line, int& etat)
{
	istringstream data(line);
	static int i(0);
	static int total(0);
	unsigned int uid1;
	unsigned int uid2;

	switch(etat) 
	{
		case NBL: 
			if(!(data >> total))  cout << "wrong input format" << endl; 
			else  i = 0 ;
			if(total == 0)  etat = NBT; 
			else  etat = LOGE ;    
			break;
		case LOGE: 	
			ajout_noeud(data,i, LOGE);
			if(i == total)  etat = NBT ;
			break;
		case NBP: 
			if(!(data >> total))  cout << "wrong input format" << endl; 
			else  i = 0;
			if(total == 0)  etat = NBLI; 
			else  etat = PROD ; 
			break;
		case PROD: 
			ajout_noeud(data, i, PROD);
			if(i == total)  etat = NBLI ; 
			break;
		case NBT:
			if(!(data >> total))  cout << "wrong input format" << endl; 
			else i = 0;
			if(total==0)  etat = NBP; 
			else  etat = TRAN ; 
			break;
		case TRAN: 
			ajout_noeud(data, i, TRAN);
			if(i == total)  etat = NBP; 
			break;
		case NBLI: 
			if(!(data >> total))  cout << "wrong input format" << endl; 
			else  i = 0;
			if(total == 0)  etat = FIN; 
			else  etat = LIENS ; 
			break;
		case LIENS: 
			if(!(data >> uid1 >> uid2))  cout << "wrong input format" << endl; 
			else
			{
				creation_lien(uid1, uid2);
				++i;
			}
			if(i == total)  etat = FIN ; 
			break;
		case FIN:  
			break;
		default:
			cout << "defaultswitch error." << endl;
			break;
	}
}

void Ville::ajout_noeud(istringstream& param,int& counter, Etat_lecture type)
{
	unsigned int numid;
	unsigned int popmax;
	double posx;
	double posy;
	
	if(!(param >> numid >> posx >> posy >> popmax))  cout << "wrong input format" 
														  << endl;
	else
	{
		switch(type)
		{
			case(LOGE):{
				Noeud* tmpl = new Logement(numid,posx,posy,popmax);
				check_load_noeud(tmpl);
				nbp_nbL += popmax;
				++counter;
				break;}
			case(TRAN):{
				Noeud* tmpt = new Transport(numid,posx,posy,popmax);
				check_load_noeud(tmpt);
				nbp_nbT += popmax;
				++counter;
				break;}
			case(PROD):{
				Noeud* tmpp = new Production(numid,posx,posy,popmax);
				check_load_noeud(tmpp);	
				nbp_nbP += popmax;	
				++counter;
				break;}
		}
	}
}

void Ville::check_load_noeud(Noeud* newNoeud)
{
	error_noeud(newNoeud);
	collis_noeuds(newNoeud);
	obstruction_lien(newNoeud);
	if(get_chargement_verif())
	{
		quartiers.push_back(newNoeud);
		load_in_tile(newNoeud);
		load_uid(newNoeud->getUid());
	}
}

void Ville::load_in_tile(Noeud* node)
{
	// load the node in the corresponding tile of the infinite grid.
	/*cout<<"loading in tile index["<<get_tile_index(node->getX())<<":"
		<<get_tile_index(node->getY())<<"]"<<endl;*/
	grid[get_tile_index(node->getX())][get_tile_index(node->getY())].push_back(node);
}

unsigned int Ville::get_tile_index(double coord)
{
	unsigned int tileindex = abs(coord/sqrt(max_capacity));
	tileindex = 2*tileindex;
	if(coord < 0 ) tileindex++;
	return tileindex;
}

void Ville::error_noeud(Noeud* const nd)
{
	if(nd->test_uid() and chargement_verif)
	{
		chargement_verif = false;
		msg_error = RES_U;
		error_param_un = nd->getUid();
	}
	
	Type_error val = nd->test_nbp();
	if(val != NO_ERROR and chargement_verif)	
	{
		chargement_verif = false;
		msg_error = val;
		error_param_un = nd->getNbp();
	}
	
	if(chargement_verif)  redondance_uid(nd->getUid());
}
	
void Ville::creation_lien(unsigned int uid_a, unsigned int uid_b) 
{
	if(chargement_verif and uid_a == uid_b)
	{
		cout << error::self_link_node(uid_a);
		chargement_verif = false;
		msg_error = SELF_L_N;
		error_param_un = uid_a;
	}
	
	Noeud* a = trouve_lien(uid_a); 
	Noeud* b = trouve_lien(uid_b);

	if(a == nullptr and chargement_verif)
	{
		cout << error::link_vacuum(uid_a);
		chargement_verif = false;
		msg_error = L_VAC;
		error_param_un = uid_a;
	}
	if(b == nullptr and chargement_verif)
	{
		if(chargement_verif)
		{
			cout << error::link_vacuum(uid_b);
			chargement_verif = false;
			msg_error = L_VAC;
			error_param_un = uid_b;
		}
	}
	
	if(a != nullptr and b != nullptr and chargement_verif)
	{
		error_lien(a, b);
		a->ajout_lien(b); 
		b->ajout_lien(a);
	}
}

void Ville::error_lien(Noeud* const a, Noeud* const b)
{
	unsigned int uid_a = a->getUid();
	unsigned int uid_b = b->getUid();
	if (a->multiple_link(b))
	{
			cout << error::multiple_same_link(uid_a, uid_b);
			chargement_verif = false;
			msg_error = MULT_S_L;
			error_param_un = uid_a;
			error_param_deux = uid_b;
	}
	// Pour les noeuds LOGE :
	if (a->maxi_link() and chargement_verif)
	{
		cout << error::max_link(uid_a);
		chargement_verif = false;
		msg_error = MAX_L;
		error_param_un = uid_a;
	}
	if (b->maxi_link() and chargement_verif)
	{
		cout << error::max_link(uid_b);
		chargement_verif = false;
		msg_error = MAX_L;
		error_param_un = uid_b;
	}
	for(size_t i(0) ; i < quartiers.size() ; ++i) 
	{
		if(quartiers[i]->collis_lien_quartier(a, b))
		{
			if(chargement_verif)
			{
				cout << error::node_link_superposition(quartiers[i]->getUid());
				chargement_verif = false;
				msg_error = N_L_SUP;
				error_param_un = quartiers[i]->getUid();
			}
		}	
	}
}

Noeud* Ville::trouve_lien(unsigned int uid) const
{
	for(size_t i(0) ; i <  quartiers.size() ; ++i)
	{
		if(uid == quartiers[i]->getUid())  return quartiers[i];
	}

	return nullptr;
}

void Ville::redondance_uid(unsigned int numid)
{
	if(!quartiers.empty())
	{
		for (unsigned int i = 0; i < occupied_uids.size() ; i++) 
		{
			if(occupied_uids[i] == numid)
			{
				cout << error::identical_uid(numid);
				chargement_verif = false;
				msg_error = ID_U;
				error_param_un = numid;
			}	
		}
	}
}

void Ville::collis_noeuds(Noeud* node)
{	
	if(chargement_verif)
	{	
		for(size_t i(0) ; i < quartiers.size() ; ++i) 
		{
			if(!(quartiers[i] == node))
			{
				if(collision_cercle(quartiers[i]->getBatiment(), 
									node->getBatiment(), dist_min))
				{										   
						chargement_verif = false;
						cout << error::node_node_superposition
								(quartiers[i]->getUid(), node->getUid());
						msg_error = N_N_SUP;
						error_param_un = quartiers[i]->getUid();
						error_param_deux =  node->getUid();
				}
			}
		}
	}
}

///////////////////////////// Section Dessin ///////////////////////////////////////

void Ville::setPaint(size_t indice, Couleur paint) 
{
	quartiers[indice]->updatePaint(paint);
}
void Ville::draw_ville() const
{	
	draw_liens();
	draw_quartiers();
}

void Ville::draw_liens() const
{
	for(auto nd : quartiers) 
	{
		nd->updateIn(true);
	}
	for(auto nd : quartiers) 
	{
		for(auto nd_lien : nd->getLiens()) 
		{
			if(nd_lien->getIn() == true)  draw_ligne(nd->getCentre(),
													 nd_lien->getCentre(),
													 NOIR);
		}
		nd->updateIn(false);
	}
}

void Ville::draw_quartiers() const
{
	for(auto nd : quartiers) 
	{
		nd->draw_noeud();
	}
}

void Ville::draw_short_path(Couleur paint, Noeud* nd) const
{
	for(auto noeud : quartiers)
	{
		noeud->updatePaint(NOIR);
	}
	nd->updatePaint(ROUGE);
	nd->draw_noeud();
	nd->draw_path(paint);
}

///////////////////////////// Sauvegarde fichier ///////////////////////////////////

void Ville::sauvegarde(string file) const
{
	fstream fichier;
	fichier.open(file, ios::out | ios::trunc);
	if(!fichier.is_open())  cout << "unable to save file" << file << endl;
	else
	{
		fichier << nb+logement << endl;
		fichier << to_string(nb_type(logement)) << endl;
		fichier << "#"+logement+":" << endl;
		fichier << print_type(logement) << endl;
		fichier << nb+transport << endl;
		fichier << to_string(nb_type(transport)) << endl;
		fichier << "#"+transport+":" << endl;
		fichier << print_type(transport) << endl;
		fichier << nb+production << endl;
		fichier << to_string(nb_type(production)) << endl;
		fichier << "#"+production+":" << endl;
		fichier << print_type(production) << endl;
		fichier << nb_link << endl;
		string bloc(to_string(nb_liens()));
		bloc.append("\n");
		for(auto nd : quartiers) 
		{
			nd->updateIn(true);
		}
		for(auto noeud : quartiers) 
		{
			for(auto nd_lien : noeud->getLiens()) 
			{
				if(nd_lien->getIn()) bloc.append("\t"+noeud->print_lien(nd_lien)
												 +"\n");
			}
			noeud->updateIn(false);
		}
		fichier << bloc << endl;
	}
	fichier.close();
}

string Ville::print_type(string type) const
{
	string bloc("");
	for(auto noeud : quartiers) 
	{
		if(noeud->getType()==type)  bloc.append("\t" + noeud->print() + "\n");
	}
	
	return bloc;
}

unsigned int Ville::nb_type(string type) const
{
	unsigned int count(0);
	for(auto noeud : quartiers) 
	{
		if(noeud->getType() == type)  count++;
	}
	
	return count;
}

unsigned int Ville::nb_liens() const
{
	for(auto nd : quartiers) 
	{
		nd->updateIn(true);
	}
	unsigned int cmt(0);
	for(auto nd : quartiers) 
	{
		for(auto nd_liens : nd->getLiens()) 
		{
			if(nd_liens->getIn() == true)  cmt++;
		}
		nd->updateIn(false);
	}
	
	return cmt;
}

/////////////////////////////////Modification//////////////////////////////////////////

void Ville::ajout_noeud(double x, double y, Type_noeud type)
{
	unsigned int currentUid = get_free_uid();
	
	switch (type)
	{
	case LOGEMENT:
		check_load_noeud(new Logement(currentUid,x,y,min_capacity));
		if(chargement_verif)  nbp_nbL+=min_capacity;
		break;
	case TRANSPORT:
		check_load_noeud(new Transport(currentUid,x,y,min_capacity));
		if(chargement_verif)  nbp_nbT+=min_capacity;
		break;
	case PRODUCTION:
		check_load_noeud(new Production(currentUid,x,y,min_capacity));
		if(chargement_verif)  nbp_nbP+=min_capacity;
		break;
	}
}

void Ville::retire_noeud(Noeud* node)
{
	unload_in_tile(node);
	unload_quartier(node);
	unload_uid(node->getUid());
	string type = node->getType();
	
	if(type == "Logement")
			nbp_nbL -= node->getNbp();
	if(type == "Transport")
			nbp_nbT -= node->getNbp();
	if(type == "Production")
			nbp_nbP -= node->getNbp();
}

void Ville::unload_in_tile(Noeud* node)
{
	unsigned int m = get_tile_index(node->getX()); //get position in grid.
	unsigned int n = get_tile_index(node->getY());

	for(uint i = 0 ; i < grid[m][n].size() ; i++){
		if(grid[m][n][i] == node)	clean_vector_erase(grid[m][n],i);
	}
}

void Ville::unload_quartier(Noeud* node)
{
	for(uint i = 0 ; i < quartiers.size() ; i++){
		if(quartiers[i] == node)	clean_vector_erase(quartiers,i);
	}
}

void Ville::load_uid(unsigned int uid) //remplace la test uid ?
{
	if(occupied_uids.empty())	occupied_uids.push_back(uid);
	else
	{
		if(uid > occupied_uids.back())	occupied_uids.push_back(uid);
		else
		{
			for (unsigned int i = 0; i < occupied_uids.size(); i++)
			{
				if(uid < occupied_uids[i])
				{
					occupied_uids.push_back(occupied_uids.back());
					for (unsigned int j = occupied_uids.size()-1; j >= i ; j--)
					{
						occupied_uids[j+1] = occupied_uids[j];
					}
					occupied_uids[i] = uid;
					break;
				}
			}
		}
	}
}

void Ville::unload_uid(unsigned int uid)
{
	//could use bissection algo
	for (uint i = 0; i < occupied_uids.size(); i++)
	{
		if(uid == occupied_uids[i])
		{

			for (uint j = i; j < occupied_uids.size()-1; j++)
			{
				occupied_uids[j] = occupied_uids[j+1];
			}
			occupied_uids.pop_back();
		}
	}
}

unsigned int Ville::get_free_uid()
{
	if(occupied_uids.empty())	return 1;

	for (unsigned int i = 0; i < occupied_uids.size()-1 ; i++)
	{
		if(!(occupied_uids[i]+1 == occupied_uids[i+1]))	return occupied_uids[i]+1;
	}

	return occupied_uids.back()+1;
}

void Ville::set_ville_color(Couleur color)
{
	for(auto node : quartiers)
	{
		node->updatePaint(color);
	}
}

void Ville::deplace_noeud(Noeud* node, Point p)
{
	Point prev_point = node->getCentre();

	node->setCentre(p);
	obstruction_lien(node);
	if(chargement_verif)	collis_noeuds(node);

	if(!chargement_verif)	node->setCentre(prev_point);

}

void Ville::update_node_paint(Noeud* node, Couleur color)
{
	if(!(node == nullptr))
	{
		node->updatePaint(color);
	}
}

void Ville::edit_lien(Noeud* node1, Noeud* node2)
{
	if(node1->is_connected_to(node2))
	{
		node1->disconnect(node2);
		node2->disconnect(node1);
	} 
	else 
	{
		error_lien(node1, node2);
		if(chargement_verif)  creation_lien(node1->getUid(), node2->getUid());
	}
}

void Ville::set_chargement_verif(bool c)
{
	chargement_verif = c;
}

Noeud* Ville::trouve_noeud(double x, double y)
{
	double tilesize = sqrt(max_capacity);

	for(int i = -1 ; i <=1 ;i++){
		for(int j = -1 ; j<=1 ; j++){
			for(auto node : grid[get_tile_index(x+i*tilesize)]
								[get_tile_index(y+j*tilesize)]){
				if(node->is_under(x,y))	return node;
			}
		}
	}
	return nullptr;
	
	//pas optimisée
	/*
	for(auto node : quartiers)
	{
		if(node->is_under(x,y))	return node;
	}
	return nullptr;
	*/
}

void Ville::resize_node(Noeud* node, Point p1, Point p2)
{
	//figures out the delta of distances from center of node
	double centx = node->getX();
	double centy = node->getY();
	Vecteur vectInt = {p1.pos_x-centx,p1.pos_y-centy};
	Vecteur vectFin = {p2.pos_x-centx,p2.pos_y-centy};
	double delta = norme(vectFin)-norme(vectInt);

	double prev_nbp = node->getNbp();
	double prev_rayon = sqrt(prev_nbp);

	node->setNBP((prev_rayon + delta)*(prev_rayon + delta));

	if(!(NO_ERROR == node->test_nbp())) chargement_verif = false;
	obstruction_lien(node);
	collis_noeuds(node);

	if(!chargement_verif)	node->setNBP(prev_nbp);
}

void Ville::obstruction_lien(Noeud* node)
{
	for(auto noeud : quartiers)
	{
		noeud->updateIn(true);
	}

	for(auto noeud : quartiers)
	{
		vector<Noeud*> linkedList = noeud->getLiens();
		for(auto linked : linkedList)
		{
			if(linked->getIn())
			{
				if(node->collis_lien_quartier(noeud,linked))
				{
				cout << error::node_link_superposition(node->getUid());
				chargement_verif = false;
				msg_error = N_L_SUP;
				error_param_un = node->getUid();
				}
			}	
		}
		noeud->updateIn(false);
	}
	
}