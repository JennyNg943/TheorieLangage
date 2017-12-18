//******************************************************************************

#include <iostream>
#include <algorithm>

#include "Accept.h"
#include "FA_tools.h"

////////////////////////////////////////////////////////////////////////////////

bool EstDeterministe(const sAutoNDE& at){
	//TODO définir cette fonction
	//Mise en place d'un compteur de symbole par état
	int cpt = 0;
	for(size_t i = 0 ; i != at.trans.size() ; ++i ){
		for(size_t c = 0 ; c < at.trans[i].size() ; ++c ){
			for(etatset_t::iterator it = at.trans[i][c].begin() ; it != at.trans[i][c].end() ; ++it ){
				cpt++;
				if(at.trans[i][c].size() > 1){	
					// On vérifie qu'il n'y ait pas 2 fois le même symbole
					if(at.trans[i][c].begin() != at.trans[i][c].end()){	
						//En cas de symbole identique, on vérifie que l'état d'arrivé n'est pas le même
						return false;
					}
				}
				
			}
		}
		
		if((unsigned)cpt < at.nb_symbs || (unsigned)cpt > at.nb_symbs){ 
			//Si le nombre de symbole de l'état est inférieur ou supérieur à celui de l'automate on retourne false
			return false;
		}		
		
		cpt = 0;
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// (1, ε, 3)(3, b, 3) -> (1, b, 3)
void Fermeture(const sAutoNDE& at, etatset_t& e){
	//TODO définir cette fonction
	
	//S'il n'y a qu'un seul element dans e
	if(e.begin()==e.end()) {
		etatset_t::iterator ite = e.begin();
		//On verifie qu'il y ait bien des e-transitions a partir de l'etat *ite
		if(!at.epsilon[*ite].empty()){
			unsigned int i = at.epsilon[*ite].size();
			for(etatset_t::iterator it = at.epsilon[*ite].begin() ; it != at.epsilon[*ite].end() ; ++it ){
				e.insert(*it);
				//On a du utiliser cette condition pour sortir de la boucle for car avec un itterateur la boucle continuait a l'infini (on ne sait pas pourquoi)
				if(*it == i)break;
			}
		}
	}
	//Sinon on ajoute a e les etats atteignables par des e-transition depuis l'ensemble des etats de depart de e
	else{
		for(etatset_t::iterator ite = e.begin() ; ite != e.end() ; ++ite ){
			for(etatset_t::iterator it = at.epsilon[*ite].begin() ; it != at.epsilon[*ite].end() ; ++it ){
				e.insert(*it);
			}
		}
	}

	
}
////////////////////////////////////////////////////////////////////////////////

etatset_t Delta(const sAutoNDE& at, const etatset_t& e, symb_t c){
	//TODO définir cette fonction

	etatset_t r, as, f;
	f.clear();
	bool trans = true;
	//Enregistre les etats de e dans r pour pouvoir avoir les etats de e dans fermeture
	r=e;
	//as est une liste des etats presents dans e au depart pour les supprimer de r si besoin
	as = e;
	//Ajoute a r tous les etats pouvant etre atteints depuis l'etat e avec la lettre c (et les e-transitions)
	for(etatset_t::iterator it = e.begin() ; it != e.end() ; ++it ){
		for(etatset_t::iterator ite = at.trans[*it][c].begin() ; ite != at.trans[*it][c].end() ; ++ite ){
			r.insert(*ite);
			if(as.find(*ite)!=as.end()){
				f.insert(*ite);
				Fermeture(at, f);
				//Supprime les etats de as si ceux-ci sont atteignables depuis l'etat actuel
				for(etatset_t::iterator itf = f.begin() ; itf != f.end() ; ++itf ){if(as.find(*itf)!=as.end()) as.erase(as.find(*itf));}
			}
		}
		
	}
	Fermeture(at, r);
	
	//Supprime les etats presents dans as
	if(trans) {
		for(etatset_t::iterator ite = as.begin() ; ite != as.end() ; ++ite ){
			r.erase(r.find(*ite));
		}
	}
	return r;
}

////////////////////////////////////////////////////////////////////////////////
bool Accept(const sAutoNDE& at, std::string str){
	//TODO définir cette fonction
	
	//Enregistre l'etat precedant l'état actuel pour l'affichge de l'erreur dans etatprec
	etatset_t etat, etatprec; 
	etat.insert(at.initial);
	bool accept = true;
	
	for(std::string::size_type l = 0; l < str.length(); ++l){ 
		char s = str[l]; 
		//Verifie que toutes les lettres font partie de l'alphabet
		if(s-ASCII_A>at.nb_symbs-1) {std::cout << "La lettre " << s <<" n'est pas dans l'alphabet " << std::endl; return false;}
		etatprec = etat; 
		//Retourne les transition possibles depuis l'etat actuel avec la prochaine lettre
		etat = Delta(at, etat, s-ASCII_A);
		if(etat.size() == 0) {std::cout << "Pas de transition possible depuis l'etat " << etatprec << " avec la lettre " << s <<std::endl; return false;}
	}
	
	for(etatset_t::iterator it = at.finaux.begin() ; it != at.finaux.end() ; it++ ){
		for(etatset_t::iterator ite = etat.begin() ; ite != etat.end() ; ite++){
			//Verifie que l'etat actuel fait partie des etats finaux
			if(*ite == *it) return true;
			else accept = false;
		}
	}
	
	return accept;
}
//******************************************************************************
