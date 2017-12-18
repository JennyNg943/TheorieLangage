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
/*  r = Determinize(at);
  vector<char> tab(r.trans.size(),'B');

  for(size_t i = 0 ; i != r.trans.size() ; ++i ){
    for(etatset_t::iterator it_et = r.finaux.begin() ; it_et != r.finaux.end() ; it_et++ ){
      if(i == *it_et){
        tab[i] = 'A';
      }
    }
    cout << i << " "<< tab[i] << endl;
  }

  for(size_t i = 0 ; i != r.trans.size() ; ++i ){
    for(size_t c = 0;c != r.trans[i].size();c++){
      for(etatset_t::iterator it = r.trans[i][c].begin();it != r.trans[i][c].end; it++){
        
      }
    }

  }
  cout << tab.size() << at.trans.size()<< endl;
  */


  return r;
}

//******************************************************************************
