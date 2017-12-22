//******************************************************************************
#include <fstream>
#include <iostream>
#include <cassert>
#include <queue>

#include "Equivalence.h"
#include "Accept.h"
#include "Determinize.h"
#include "Minimize.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction
	sAutoNDE at2;

	//Creation de l'automate arrange
	at2 = Determinize(at);
	vector<etatset_t> prec, prece;
	at2.nb_etats = at2.nb_etats + 2;
	etat_t ini = 0, fin = at2.nb_etats - 1;
	etatset_t list_tr, list_finaux = at2.finaux, n_final;
	std::vector<etat_t> visites;
	n_final.insert(fin);
	at2.initial = ini;

	at2.trans.resize(at2.nb_etats);
	at2.epsilon.resize(at2.nb_etats);
	prec = at2.trans[0];
	for(size_t i = 1; i < at2.nb_etats-1; i++){

		prece = at2.trans[i];
		at2.trans[i] = prec;
		prec = prece;
	}

	for(size_t i = 0 ; i != at2.trans.size() ; ++i ){
		for(size_t c = 0 ; c < at2.trans[i].size() ; ++c ){
			list_tr.clear();
			for(etatset_t::iterator it = at2.trans[i][c].begin() ; it != at2.trans[i][c].end() ; ++it ){
				list_tr.insert(*it+1);
			}
			at2.trans[i][c] = list_tr;
		}
	}

	//Creation des e-transitions du nouvel etat initial a l'ancien et des anciens etats finaux au nouveau
	for(etatset_t::iterator it = at2.finaux.begin() ; it != at2.finaux.end() ; it++ ){
		for(size_t i = 0 ; i != at2.trans.size() ; ++i ){
			for(size_t c = 0 ; c < at2.trans[i].size() ; ++c ){
				for(etatset_t::iterator ite = at2.trans[i][c].begin() ; ite != at2.trans[i][c].end() ; ++ite ){
					if(*ite == *it + 1){
						at2.epsilon[*ite].insert(fin);
					}
				}
			}
		}
	}

	at2.finaux = n_final;
	at2.trans[0].clear();
	at2.epsilon[0].insert(1);

	//Creation de l'expression rationelle (ne fonctionne pas)
	std::string sri = "", sr = "";

	for(size_t i = 1 ; i != at2.trans.size() ; ++i ){
		sri.clear();
		for(size_t c = 0 ; c < at2.trans[i].size() ; ++c ){
			for(etatset_t::iterator it = at2.trans[i][c].begin() ; it != at2.trans[i][c].end() ; ++it ){
				if (i == *it){
					sri+= (char)(c+ASCII_A);
					sri+="*";
				}
				if(list_finaux.find(i-1) != list_finaux.end()){
					sri+= "|";
					sri+= (char)(c+ASCII_A);
					sri+="*";
				}
				else{
					sri+=(char)(c+ASCII_A);
				}
			}
		}
		sr += sri;
	}

cout << sr << endl;



  return sr;
}////////////////////////////////////////////////////////////////////////////////

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max) {
	//TODO définir cette fonction
	string mot, m;
	set<string> liste_mots;

	//Genere tous les mots de longueur word_size_max ou moins
	for(size_t c =0; c<a1.nb_symbs; c++) liste_mots.insert(string(1,(char)(c+ASCII_A)));
	for(unsigned int i = 0; i < word_size_max; i++){
		for(set<string>::iterator it = liste_mots.begin(); it!=liste_mots.end(); it++){

			if((*it).size()<i){
				for(size_t c =0; c<a1.nb_symbs; c++){
					m = *it + string(1,(char)(c+ASCII_A));
					//Verifie que le mot genere est accepte par les deux automates
					if(Accept(a1, m) != Accept(a2, m)) return false;
					liste_mots.insert(m);
				}
			}
		}
	}

	return true;
}////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
	sAutoNDE at1, at2;
	bool eq = true;
	//Minimise les automates
	at1 = Minimize(a1), at2 = Minimize(a2);
	if(at1.nb_etats == at2.nb_etats){
		//Verifie que les etats finaux soient les memes 
		if(at1.finaux != at2.finaux) eq = false;
		//Verifie que les tables de transition soient les memes
		else{
			for(size_t i = 0; i < at1.trans.size(); i++){
				for(size_t c =0; c < at1.nb_symbs; c++){
					if(at1.trans[i][c] != at2.trans[i][c])  eq = false;
				}
			}
		}
	}
	else eq = false;
	return eq;
}//******************************************************************************
