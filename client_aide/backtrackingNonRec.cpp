#include "backtrackingNonRec.hpp"

using namespace std;

BacktrackingNonRec::BacktrackingNonRec(int x, std::vector<Constraint*> contraintes):problem(x,contraintes){
	if(x > 0){
		noeuds.push_front(problem.initialNode());
	}
}

BacktrackingNonRec::BacktrackingNonRec(string chaine, std::vector<Constraint*> contraintes):problem(contraintes){
	parser(chaine);
}

void BacktrackingNonRec::parser(std::string chaine){

}

string BacktrackingNonRec::toString(Noeud n){
	string chaine = "";
	for(int i=0; i<n.getDomains().size();i++){
		chaine+="{";
		set<int> domain = n.getDomains().at(i);
		for (std::set<int>::iterator it = domain.begin(); it != domain.end(); it++){
			chaine+=to_string(*it)+",";
		}
		chaine+="}";
	}
	chaine+="\n";
	return chaine;
}

int BacktrackingNonRec::solve(){
	int nb_so = 0;
	int cpt = 0;
	while(!noeuds.empty()){

		std::list<Noeud>::iterator list_iter = noeuds.begin();
		cout<<toString(*list_iter)<<endl;
		cout<<"\n"<<endl;
		while(list_iter != noeuds.end()){

			std::list<Noeud>::iterator temp = list_iter;

			list_iter++;

			Proof p = problem.testSat(*temp);
			if(p == succes){
				nb_so++;
				//cout<<"Solution numero "<<nb_so<<": "<<endl;
				//temp->toString();
				noeuds.erase(temp);
			}
			else if(p == echec){
				noeuds.erase(temp);
			}
			else{
				branch(*temp);
				noeuds.erase(temp);
			}
		}
	cpt++;
	}
	return nb_so;
}

void BacktrackingNonRec::branch(Noeud noeud){
	int i=0;
	bool trouve = false;

	while(i < noeud.getDomains().size() && !trouve){
		if(noeud.getDomains().at(i).size() > 1){
			set<int> domain = noeud.getDomains().at(i);
			for (std::set<int>::iterator it = domain.begin(); it != domain.end(); it++){
				Noeud n;
				n = noeud.copie();
				n.clear_and_add(i, *it);

				noeuds.push_front(n);
				trouve = true;
			}
		}
		i++;
	}
}

