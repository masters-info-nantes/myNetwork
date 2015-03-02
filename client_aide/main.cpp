#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Noeud.hpp"
#include "backtrackingNonRec.hpp"
#include "Constraint.hpp"
#include "Nqueen_colonne.hpp"
#include "Nqueen_diago.hpp"
#include "Solv.hpp"
#include "../clientlib/libmynetwork.h"

#define MODE_NORMAL 0
#define MODE_DAEMON 1
#define MODE_INPUT_FILE 2

using namespace std;

int mode;
char* file;

void normalMode();
void inputFileMode();


int main(int argc, char *argv[]){

	mode = MODE_NORMAL;
	
	for(int i=0;i<argc;i++) {
		if(strncmp("-d",argv[i],2) == 0 || strncmp("--daemon",argv[i],8) == 0) {
			mode = MODE_DAEMON;
		} else if(strncmp("-i",argv[i],2) == 0) {
			mode = MODE_INPUT_FILE;
			file = argv[i+1];
			i++;
		}
	}

	std::vector<Constraint*> contraintes;
	Nqueen_colonne contrainte_colonne;
	Nqueen_diago contrainte_diago;
	contraintes.push_back(&contrainte_colonne);
	contraintes.push_back(&contrainte_diago);

	int nb_sol;

	if(mode == MODE_DAEMON){
		myNetworkStartDaemon("./solve -i");
	}
	else if(mode == MODE_INPUT_FILE){
		BacktrackingNonRec b(file, contraintes);
		nb_sol = b.solve();
		cout<<nb_sol<<endl;
	}
	else if(mode == MODE_NORMAL){
		BacktrackingNonRec b(7, contraintes);
		nb_sol = b.solve();
		cout<<nb_sol<<endl;
	}
	else{
		perror("no valid mode\n");
	}

	
	return 0;
}
