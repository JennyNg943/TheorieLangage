//******************************************************************************

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include "Determinize.h"
#include "Minimize.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
//Classe 0 -> etat finaux
sAutoNDE Minimize(const sAutoNDE& at){
  //TODO définir cette fonction

  sAutoNDE r;
  vector<vector<etatset_t>> trans;
  r = Determinize(at);
  set<int> classes;
  vector<int> tab(r.trans.size(),1); //Initialisation d'un tableau ayant pour taille le nombre d'état du graphe initial
  for(size_t i = 0 ; i != r.trans.size() ; ++i ){
    for(etatset_t::iterator it_et = r.finaux.begin() ; it_et != r.finaux.end() ; it_et++ ){
      if(i == *it_et){
        tab[i] = 0; // Les etats finaux sont regroupés
      }
    }

  }

  //On compare pour chaque sommet si on peut les regroupé dans des même classes
  int nbClasse = 1;
  for(size_t i = 0; i < tab.size();i++){
    for(size_t j = i+1; j < tab.size();j++){

        int memeEtat = 0;
        for(size_t c = 0; c != r.trans[i].size();c++){
          if(tab[*r.trans[j][c].begin()] != tab[*r.trans[i][c].begin()]){
            memeEtat = 1; //En cas de transition différente on met le flag à 1.
          }
        }
        if(tab[j] == 0){    //On vérifie que l'on ne regroupe pas des etats finaux avec de simple états
          if(tab[i] == 0){
            if(memeEtat == 1){
              tab[j] = nbClasse +1;
              nbClasse++;
            }
          }
        }else if(tab[i] == tab[j]){
          if(memeEtat == 0 && tab[i] != 0){
            tab[j] = tab[i];
          }else{
            tab[j] = nbClasse +1;
            nbClasse++;
          }
        }
      }
    }

  for(size_t i =0; i < tab.size(); i++){
    classes.insert(tab[i]);
  }

  //On cree un vecteur contenant les differentes classes de l'automate minimise et les les etats que contiennent chacune des classses
  vector<etatset_t> new_etats(classes.size());
  for(size_t i =0; i < tab.size(); i++){
    new_etats[tab[i]].insert((etat_t) i);
  }

  //Creation de la table de transition de l'automate minimise
  trans.resize(new_etats.size());
  for(size_t i = 0; i < new_etats.size(); i++){
    trans[i].resize(at.nb_symbs);
  }


    //Remplissage la table de transition du nouvel automate (l'automate minimise)
    for(size_t in = 0; in < new_etats.size(); in++){
      for(set<etat_t>::iterator ite = new_etats[in].begin() ; ite != new_etats[in].end() ; ite++){
        for(size_t c = 0 ; c < at.nb_symbs; c++){
          for(size_t i = 0 ; i < new_etats.size(); i++){
            for(set<etat_t>::iterator it = r.trans[*ite][c].begin() ; it != r.trans[*ite][c].end() ; it++){
              if(new_etats[i].find(*it) != new_etats[i].end()){
                trans[in][c].insert((etat_t)i);
              }
            }
          }
        }
      }
    }

  r.initial = tab[r.initial];
  etatset_t etatFinaux;

  //On definit les classes contenant un etat final de at comme etant finales de l'automate minimise
  for(etatset_t::iterator it_et = r.finaux.begin() ; it_et != r.finaux.end() ; it_et++ ){
    etatFinaux.insert(tab[*it_et]);
  }

  r.finaux = etatFinaux;

r.trans = trans;
  return r;
}

//******************************************************************************
