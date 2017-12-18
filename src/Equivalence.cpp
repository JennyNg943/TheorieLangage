//******************************************************************************

#include <iostream>
#include <cassert>

#include "Accept.h"
#include "Equivalence.h"
#include "ToGraph.h"

////////////////////////////////////////////////////////////////////////////////

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

	std::string sr = "";

	/*
	sAutoNDE at2;
	etat_t etatInitial(0);
	etat_t etatFinal(0);
	etatset_t etat;
	etat.insert(at.initial);
	at2.epsilon[at.initial].insert(etatInitial);


	for(size_t i = 0 ; i != at.trans.size() ; ++i ){
			for(size_t c = 0 ; c < at.trans[i].size() ; ++c ){
				for(etatset_t::iterator it = at.trans[i][c].begin() ; it != at.trans[i][c].end() ; ++it ){
					at.trans[i][c+ASCII_A].insert(*it);
					std::cout << i << *it;
			}
		}
	}

	for(size_t i = 0 ; i != at.trans.size() ; ++i ){
		for(size_t c = 0 ; c < at.trans[i].size() ; ++c ){
			sr += std::string(1,(char)(c + ASCII_A));
			for(etatset_t::iterator it = at.trans[i][c].begin() ; it != at.trans[i][c].end() ; ++it ){
				if(*it == i){
					sr +=  "*" ;
				}
				for(etatset_t::iterator it_et = at.finaux.begin() ; it_et != at.finaux.end() ; it_et++ ){
					if(*it == *it_et){
						sr += "|" + sr;
					}
				}
			}
		}
	}*/

	//std::cout << sr ;
  return sr;
}

////////////////////////////////////////////////////////////////////////////////

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max) {
  //TODO définir cette fonction

  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
  //TODO définir cette fonction

  return true;
}

//******************************************************************************
