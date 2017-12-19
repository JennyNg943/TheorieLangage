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
  r = Determinize(at);
  vector<int> tab(r.trans.size(),1);
  for(size_t i = 0 ; i != r.trans.size() ; ++i ){
    for(etatset_t::iterator it_et = r.finaux.begin() ; it_et != r.finaux.end() ; it_et++ ){
      if(i == *it_et){
        tab[i] = 0;
      }
    }

  }
  int nbClasse = 1;
  for(size_t i = 0; i < tab.size();i++){
    for(size_t j = i+1; j < tab.size();j++){

        int memeEtat = 0; //On compare si les deux sommets ont les meme transitions
        for(size_t c = 0; c != r.trans[i].size();c++){
          if(tab[*r.trans[j][c].begin()] != tab[*r.trans[i][c].begin()]){
            memeEtat = 1;
          }
          cout << tab[*r.trans[j][c].begin()] << tab[*r.trans[i][c].begin()]  << " " << i << " "<< j << " " << c << endl;
        }
        if(memeEtat == 0){
          tab[j] = tab[i];
          cout << "e1 " << j <<" "<< tab[j] << endl;
        }else{
          tab[j] = nbClasse +1;
          nbClasse++;
          cout << "e2 " << j <<" "<< tab[j] << endl;
        }
      }
      cout << i << " "<< tab[i] << endl;
    }


  return r;
}

//******************************************************************************
