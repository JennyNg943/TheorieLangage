//******************************************************************************
#include <fstream>
#include <iostream>
#include <cassert>
#include <queue>

#include "Equivalence.h"
#include "Accept.h"
#include "Determinize.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction
	sAutoNDE at2;
	at2 = Determinize(at);
	vector<etatset_t> prec, prece;
	at2.nb_etats = at2.nb_etats + 2;
	etat_t ini = 0, fin = at2.nb_etats - 2;
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






	//On ouvre un fichier correspondant au path. S'il en existe deja un on le vide.
	string path = "test.txt";
	ofstream f(path, ios::trunc);

	//On remplit le fichier .gv
	f << "digraph finite_state_machine {"<< endl;
	f << "	rankdir=LR;"<<endl;
	f << "  	size=\"10,10\""<<endl << endl;
	f << "	node [shape = doublecircle]; ";

	//On parcourt les etats finaux pour les ajouter au .gv
	for(etatset_t::iterator it = at2.finaux.begin() ; it != at2.finaux.end() ; it++ ){
		//cout << *it << endl;
		f << *it << " ";
	}

	f << ";";
	f << endl;
	f << "	node [shape = point ]; q;" << endl;
	f << "	node [shape = circle]; " << endl << endl;
	f << "	q -> " << at2.initial << ";" << endl;

	//On ajoute toutes les transitions au .gv
	for(size_t i = 0 ; i != at2.trans.size() ; ++i ){
		for(size_t c = 0 ; c < at2.trans[i].size() ; ++c ){
			for(etatset_t::iterator it = at2.trans[i][c].begin() ; it != at2.trans[i][c].end() ; ++it ){
				f <<"	"<< i << " -> " <<  *it << " [label = \"" << (char)(c + ASCII_A) << "\"];" <<  std::endl;
			}
		}
	}

	f << std::endl;

	//On ajoute toutes les e-transitions au .gv
	for(size_t i = 0 ; i < at2.epsilon.size() ; ++i ){
		for(etatset_t::iterator it = at2.epsilon[i].begin() ; it != at2.epsilon[i].end() ; ++it ){
			f <<"	"<< i << " -> " <<  *it << " [label = \"ε\"];" << std::endl;
		}
	}
	f << std::endl;
	f << "}";
	f << std::endl;

	string name = path.substr(0, path.size()-3);
	//On crée une image au format png avec le .gv que l'on vient de créer.
	string command = "dot -Tpng " + path + " -o " + name + ".png";
	system(command.c_str());


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

	for(size_t c =0; c<a1.nb_symbs; c++) liste_mots.insert(string(1,(char)(c+ASCII_A)));
	for(unsigned int i = 0; i < word_size_max; i++){
		for(set<string>::iterator it = liste_mots.begin(); it!=liste_mots.end(); it++){

			if((*it).size()<i){
				for(size_t c =0; c<a1.nb_symbs; c++){
					m = *it + string(1,(char)(c+ASCII_A));
					if(Accept(a1, m) != Accept(a2, m)) return false;
					liste_mots.insert(m);
				}
			}
		}
	}

	return true;
}////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
	//TODO définir cette fonction

	return true;
}//******************************************************************************
