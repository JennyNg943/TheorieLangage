//******************************************************************************

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include "Determinize.h"
#include "Minimize.h"

using namespace std;
////////////////////////////////////////////////////////////////////////////////
//Rajout d'une transition quand determinisation de R
sAutoNDE Minimize(const sAutoNDE& at){
  //TODO définir cette fonction

  sAutoNDE r;
  r = Determinize(at);
  vector<char> tab(r.trans.size(),'B');
  for(size_t i = 0 ; i != r.trans.size() ; ++i ){
    for(etatset_t::iterator it_et = r.finaux.begin() ; it_et != r.finaux.end() ; it_et++ ){
      if(i == *it_et){
        tab[i] = 'A';
      }
    }
    cout << i << " "<< tab[i] << endl;
  }

  for(size_t i = 0; i < tab.size();i++){
    for(size_t j = i+1; j < tab.size();j++){

        int memeEtat = 0;
        for(size_t c = 0; c != r.trans[i].size();c++){
          if(tab[*r.trans[j][c].begin()] != tab[*r.trans[i][c].begin()]){
            memeEtat = 1;
          }

        }
        if( memeEtat == 0){
          tab[j] = tab[i];
        }
      }
    }


  return r;
}

//******************************************************************************
