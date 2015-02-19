#include "backtrackingNonRec.hpp"
#include <fstream>
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
	ifstream fichier(chaine, ios::in);
	if(fichier){
		string ligne;
		while(getline(fichier, ligne)){
			getline(fichier, ligne);
			std::vector<std::set<int> >temp;
			int i = 0;
			while(i<ligne.size()){
				if(ligne.at(i)=='{'){
					std::set<int> ens_temp;
					i++;
					int val = ligne.at(i)-48;
					i++;
					ens_temp.insert(val);
					temp.push_back(ens_temp);
				}
				i++;

			}
			Noeud n;
			n.setDomains(temp);
			noeuds.push_front(n);
		
		}
	}
	else{
		cout<<"kjhbhjvbgv"<<endl;
	}
}

string BacktrackingNonRec::toString(){
	string chaine = "";
	std::list<Noeud>::iterator list_iter = noeuds.begin();
	Noeud n = *list_iter;
	int taille = noeuds.size()/2;
	for(int i = 0; i<taille; i++){
		for(int i=0; i<n.getDomains().size();i++){
			chaine+="{";
			set<int> domain = n.getDomains().at(i);
			for (std::set<int>::iterator it = domain.begin(); it != domain.end(); it++){
				chaine+=to_string(*it)+",";
			}
			chaine+="}";
		}
		chaine+="\n";
		std::list<Noeud>::iterator temp = list_iter;

		list_iter++;
		noeuds.erase(temp);

	}

	return chaine;
}

int BacktrackingNonRec::solve(){
	int nb_so = 0;
	int cpt = 0;

	while(!noeuds.empty()){
		cout<<noeuds.size()<<"\n"<<endl;
		string donnees="";
		if(noeuds.size()>20000){
			donnees = toString();
		}
		std::list<Noeud>::iterator list_iter = noeuds.begin();

		list_iter->toString();
		cout<<noeuds.size()<<"\n"<<endl;
		while(list_iter != noeuds.end()){

			std::list<Noeud>::iterator temp = list_iter;

			list_iter++;

			Proof p = problem.testSat(*temp);
			if(p == succes){
				nb_so++;
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

