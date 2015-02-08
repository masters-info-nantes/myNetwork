#include <iostream>
#include "Noeud.hpp"
#include <list>
#include "backtrackingNonRec.hpp"
#include "Constraint.hpp"
#include "Nqueen_colonne.hpp"
#include "Nqueen_diago.hpp"
#include "Solv.hpp"

using namespace std;

int main(int argc, char *argv[]){


	std::vector<Constraint*> contraintes;
	Nqueen_colonne contrainte_colonne;
	Nqueen_diago contrainte_diago;
	contraintes.push_back(&contrainte_colonne);
	contraintes.push_back(&contrainte_diago);
	BacktrackingNonRec b(6, contraintes);

	int nb_sol = b.solve();
	cout<<nb_sol<<endl;
               
	return nb_sol;
}

