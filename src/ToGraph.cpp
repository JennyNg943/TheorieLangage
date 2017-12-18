//******************************************************************************

#include "ToGraph.h"
#include <fstream>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////
using namespace std;
bool ToGraph(sAutoNDE& at, string path){

	//TODO définir cette fonction

	//On ouvre un fichier correspondant au path. S'il en existe deja un on le vide.
	ofstream f(path, ios::trunc);

	//On remplit le fichier .gv
	f << "digraph finite_state_machine {"<< endl;
	f << "	rankdir=LR;"<<endl;
	f << "  	size=\"10,10\""<<endl << endl;
	f << "	node [shape = doublecircle]; ";

	//On parcourt les etats finaux pour les ajouter au .gv
	for(etatset_t::iterator it = at.finaux.begin() ; it != at.finaux.end() ; it++ ){
		f << *it << " ";
	}

	f << ";";
	f << endl;
	f << "	node [shape = point ]; q;" << endl;
	f << "	node [shape = circle]; " << endl << endl;
	f << "	q -> " << at.initial << ";" << endl;

	//On ajoute toutes les transitions au .gv
	for(size_t i = 0 ; i != at.trans.size() ; ++i ){
		for(size_t c = 0 ; c < at.trans[i].size() ; ++c ){
			for(etatset_t::iterator it = at.trans[i][c].begin() ; it != at.trans[i][c].end() ; ++it ){
				f <<"	"<< i << " -> " <<  *it << " [label = \"" << (char)(c + ASCII_A) << "\"];" <<  std::endl;
			}
		}
	}

	f << std::endl;

	//On ajoute toutes les e-transitions au .gv
	for(size_t i = 0 ; i < at.epsilon.size() ; ++i ){
		for(etatset_t::iterator it = at.epsilon[i].begin() ; it != at.epsilon[i].end() ; ++it ){
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

	return true;

}

//******************************************************************************
