#ifndef DEF_BACKTRACKINGNONREC
#define DEF_BACKTRACKINGNONREC

#include <iostream>
#include <list>
#include "Noeud.hpp"
#include "Solv.hpp"
#include "Problem.hpp"
#include "Constraint.hpp"
#include <string>

class BacktrackingNonRec : public Solveur{

public:
	BacktrackingNonRec(int x, std::vector<Constraint*> contraintes);
	BacktrackingNonRec(std::string chaine, std::vector<Constraint*> contraintes);
	//resoud le probleme et retourne le nombre de solutions
	int solve();
	//cre le sous-noeud d'un noeud
	void branch(Noeud noeud);
	void parser(std::string chaine);
	std::string toString();

private:
	std::list<Noeud> noeuds;
	Problem problem;

};

#endif
