//******************************************************************************

#include <iostream>
#include <algorithm>
#include "Determinize.h"
#include "FA_tools.h"
#include "Accept.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////

sAutoNDE Determinize(const sAutoNDE& at){
	//TODO définir cette fonction
	sAutoNDE r;
	
	etatset_t etat, etat_poss;
	vector<vector<etatset_t>> trans;
	set<etatset_t> finaux;
	set<etat_t> rfinaux;
	size_t i =0;
	bool ande = false;
	r.nb_symbs = at.nb_symbs;
	r.initial = at.initial;
	
	

	//On regarde si at est un AND ou un ANDE	
	for(size_t i = 0 ; i < at.epsilon.size() ; ++i ){
		if(!at.epsilon[i].empty()) ande = true;
	}  

	//Traitement des automates sans e-transitions
	if(!ande){
		std::set<etatset_t> tout_etats;
		etat.insert(r.initial);
		tout_etats.insert(etat);
		//On regarde si l'etat initial est un etat final
		for(etatset_t::iterator it_et = at.finaux.begin() ; it_et != at.finaux.end() ; it_et++ ){
			if(*it_et == r.initial) rfinaux.insert(r.initial); 
		}
		
		for(std::set<etatset_t>::iterator ite = tout_etats.begin() ; ite != tout_etats.end() ; ite++){
			
			etat = *ite;
			for(size_t c = 0; c<r.nb_symbs; c++){
				//On regardes les etats atteignables depuis l'etat actuel avec le symbole c
				etat_poss = Delta(at, etat, c);
				if(!etat_poss.empty() && tout_etats.find(etat_poss)==tout_etats.end()){
					//Si une transition existe avec cette lettre et que l'etatset n'est pas déja dans la liste des etats on l'ajoute a la liste des etats de r
					tout_etats.insert(etat_poss);				
				}
				
				//On verifie si l'etatset contient un des etats finaux de at
				for(etatset_t::iterator it_et = at.finaux.begin() ; it_et != at.finaux.end() ; it_et++ ){
					for(etatset_t::iterator it_poss = etat_poss.begin() ; it_poss != etat_poss.end() ; it_poss++){
						if(*it_et == *it_poss && finaux.find(etat_poss) == finaux.end() && *it_poss != r.initial) finaux.insert(etat_poss); 
					}
				}
			}
			
		}
		
		//On convertit les etatsset finaux en etats finaux du nouvel automate 
		for(set<etatset_t>::iterator pomme = finaux.begin(); pomme != finaux.end(); pomme++){
			rfinaux.insert(distance(tout_etats.begin(), tout_etats.find(*pomme)));
		}

		r.nb_finaux = finaux.size();
		r.finaux = rfinaux;
		r.nb_etats = tout_etats.size()+1;
		trans.resize(r.nb_etats);
		
		
		//On cree la matrice de transition de r et s'il n'y a pas de transition d'un etat donne avec un symbole donne alors on le relie au puit
		for(set<etatset_t>::iterator it_tout_etat = tout_etats.begin(); it_tout_etat != tout_etats.end(); it_tout_etat++){
			trans[i].resize(r.nb_symbs);
			for(size_t c = 0; c<r.nb_symbs; c++){
				if(!Delta(at, *it_tout_etat, c).empty()){
					etat = Delta(at, *it_tout_etat, c);
					trans[i][c].insert((etat_t)(distance(tout_etats.begin(), tout_etats.find(etat))));
				}
				else{
					trans[i][c].insert((etat_t)(r.nb_etats-1));
				}
			}
			i++;
		}	  
		r.trans = trans;
	}
	
	//Traitement des automates avec e-transition
	else{
		vector<etatset_t> tout_etats;
		etat.insert(r.initial);
		if(!at.epsilon[r.initial].empty()){
			Fermeture(at, etat);
			etat.erase(etat.begin());
			etat.insert(r.initial);
		}
		
		tout_etats.push_back(etat);
		//On regarde si l'etatset initial contient un etat final
		for(etatset_t::iterator it_et = at.finaux.begin() ; it_et != at.finaux.end() ; it_et++){
			for(etatset_t::iterator it_ef = etat.begin() ; it_ef != etat.end() ; it_ef++){
				if(*it_et == *it_ef && finaux.find(etat) == finaux.end()) finaux.insert(etat); 
			}
		}
		
		
		for(unsigned int i = 0; i < tout_etats.size(); i++){
			//On parcourt le vecteur comme ça et non pas avec un itterateur car avec un itterateur on avait une boucle infinie
			etat = tout_etats[i];
			for(size_t c = 0; c<r.nb_symbs; c++){
				//On regardes les etats atteignables depuis l'etat actuel avec le symbole c
				etat_poss = Delta(at, etat, c);
				if(!etat_poss.empty() && find(tout_etats.begin(), tout_etats.end(), etat_poss) == tout_etats.end()){
					//Si une transition existe avec cette lettre et que l'etatset n'est pas déja dans la liste des etats on l'ajoute a la liste des etats de r
					tout_etats.push_back(etat_poss);
				}
				
				//On verifie si l'etatset contient un des etats finaux de at
				for(etatset_t::iterator it_et = at.finaux.begin() ; it_et != at.finaux.end() ; it_et++ ){
					for(etatset_t::iterator it_poss = etat_poss.begin() ; it_poss != etat_poss.end() ; it_poss++){
						if(*it_et == *it_poss && finaux.find(etat_poss) == finaux.end() && (*it_poss != r.initial || etat_poss.size()>1)) finaux.insert(etat_poss); 
					}
				}
			}
		}
		
		//On convertit les etatsset finaux en etats finaux du nouvel automate 
		for(set<etatset_t>::iterator pomme = finaux.begin(); pomme != finaux.end(); pomme++){
			rfinaux.insert(distance(tout_etats.begin(), find(tout_etats.begin(), tout_etats.end(),*pomme)));
		}

		r.nb_finaux = finaux.size();
		r.finaux = rfinaux;
		r.nb_etats = tout_etats.size()+1;
		trans.resize(r.nb_etats);

		//On cree la matrice de transition de r et s'il n'y a pas de transition d'un etat donne avec un symbole donne alors on le relie au puit
		for(vector<etatset_t>::iterator it_tout_etat = tout_etats.begin(); it_tout_etat != tout_etats.end(); it_tout_etat++){
			trans[i].resize(r.nb_symbs);
			for(size_t c = 0; c<r.nb_symbs; c++){
				if(!Delta(at, *it_tout_etat, c).empty()){
					etat = Delta(at, *it_tout_etat, c);
					trans[i][c].insert((etat_t)(distance(tout_etats.begin(), find(tout_etats.begin(), tout_etats.end(),etat))));
				}
				else{
					trans[i][c].insert((etat_t)(r.nb_etats-1));
				}
			}
			i++;
		}	  
		r.trans = trans;
	}

	//Verifie si l'automate determinise possede un puit. Si oui, fait boucler le puit sur lui meme, sinon reduit la matrice de transition de 1
	bool puit = false;
	for(size_t i = 0 ; i != trans.size() ; ++i ){
		for(size_t c = 0 ; c < trans[i].size() ; ++c ){
			for(etatset_t::iterator it = trans[i][c].begin() ; it != trans[i][c].end() ; ++it ){
				if(*it == (etat_t)(r.nb_etats-1)) puit = true;
			}
		}
	}

	if(puit){
		trans[trans.size()-1].resize(r.nb_symbs);
		for(size_t c = 0; c<r.nb_symbs; c++){
			trans[trans.size()-1][c].insert((etat_t)(r.nb_etats-1));
		}
	}
	else trans.resize(trans.size()-1);
	
	r.trans = trans;

	r.nb_etats = trans.size(); 
	return r;
}

//******************************************************************************
