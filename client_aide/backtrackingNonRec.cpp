#include "backtrackingNonRec.hpp"

#include <string>
#include <fstream>
#include <unistd.h>
#include "../clientlib/colorlog.h"
using namespace std;

BacktrackingNonRec::BacktrackingNonRec(int x, std::vector<Constraint*> contraintes):problem(x,contraintes){
	if(x > 0){
		noeuds.push_front(problem.initialNode());
	}

	int socket = myNetworkCreateSocket();

	myNetworkOpenSocketConnexion(socket);
	id_master = myNetworkConnectMaster(socket);
	myNetworkCloseSocketConnexion(socket);

}

BacktrackingNonRec::BacktrackingNonRec(string chaine, std::vector<Constraint*> contraintes):problem(contraintes){
	parser(chaine);
}

void BacktrackingNonRec::parser(std::string chaine){
	ifstream fichier(chaine.c_str());
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
				int num = *it;
				char n[3];		
				chaine+=sprintf(n, "%d", num);+",";
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

		string donnees="";
		if(noeuds.size()>2){

			donnees = toString();

			/*RESEAUX*/
			VERBOSE("queen","before myNetworkCreateSocket");
			int socket = myNetworkCreateSocket();
            VERBOSE("queen","after myNetworkCreateSocket before myNetworkOpenSocketConnexion");
			myNetworkOpenSocketConnexion(socket);
			VERBOSE("queen","after myNetworkOpenSocketConnexion before myNetworkReserveClient");
			char* id_client = myNetworkReserveClient(socket, id_master);
			VERBOSE("queen","after myNetworkReserveClient before myNetworkCloseSocketConnexion");
			myNetworkCloseSocketConnexion(socket);
			VERBOSE("queen","after myNetworkCloseSocketConnexion");
			if(id_client != 0){
				char* cha = new char[donnees.length()+1];
				strcpy(cha, donnees.c_str());
				VERBOSE("queen","before myNetworkCreateSocket");
                socket = myNetworkCreateSocket();
                VERBOSE("queen","after myNetworkCreateSocket before myNetworkOpenSocketConnexion");
			    myNetworkOpenSocketConnexion(socket);
			    VERBOSE("queen","after myNetworkOpenSocketConnexion before myNetworkAskClient");
				bool ok = myNetworkAskClient(socket, id_master, id_client, cha);
			    VERBOSE("queen","after myNetworkAskClient before myNetworkCloseSocketConnexion");
			    myNetworkCloseSocketConnexion(socket);
			    VERBOSE("queen","after myNetworkCloseSocketConnexion");
			}

			/*FRESEAUX*/
		}

		std::list<Noeud>::iterator list_iter = noeuds.begin();

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

	/*RESEAUX*/
	sleep(10);
	int socket = myNetworkCreateSocket();
	myNetworkOpenSocketConnexion(socket);
	LinkedListString* temp = myNetworkWaitingRequest(socket, id_master);
	myNetworkCloseSocketConnexion(socket);
	while(temp != 0 && strncmp(getString(temp, 0), "NOTHING", 7) != 0){
		std::cout<<"atoi     "<<getString(temp, 2)<<std::endl;
		//nb_so+=atoi(getString(temp, 2));

		socket = myNetworkCreateSocket();
		myNetworkOpenSocketConnexion(socket);
		temp = myNetworkWaitingRequest(socket, id_master);
		myNetworkCloseSocketConnexion(socket);
	}
	socket = myNetworkCreateSocket();
	myNetworkOpenSocketConnexion(socket);
	myNetworkDisconnectClient(socket, id_master);
	/*FRESEAUX*/

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

