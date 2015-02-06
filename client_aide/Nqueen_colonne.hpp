#ifndef DEF_NQUEENCOLONNE
#define DEF_NQUEENCOLONNE

#include <iostream>
#include "Constraint.hpp"
#include "Proof.hpp"
#include <set>

class Nqueen_colonne: public Constraint{

public:
	//teste la contrainte
	Nqueen_colonne();
	Proof test(Noeud noeud);
};

#endif
