#include "libmynetwork.h"
#define _DEFAULT_SOURCE
#include <linux/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"
#include "colorlog.h"

typedef struct sockaddr 	sockaddr;
typedef struct sockaddr_in 	sockaddr_in;
typedef struct hostent 		hostent;
typedef struct servent 		servent;

int socket_descriptor;
//~ int longueur;
sockaddr_in adresse_locale;
bool inited = false;
hostent * ptr_host;
servent * ptr_service;
char* host_addr = "localhost";
int host_port = 5000;
char clientId[11];

// private functions
char* runExternalProgram(char* programPath);

bool myNetworkWrite(int socket_descriptor, LinkedListString* msg);
LinkedListString* myNetworkRead(int socket_descriptor);

void myNetworkSetPort(int listenPort) {
	host_port = listenPort;
}

void myNetworkSetHost(char* host) {
	host_addr = host;
}

void myNetworkStartDaemon(char* programPath) {
	//~ pid_t daemonPID = fork();
	//~ if(daemonPID != 0) {
		//~ printf("Daemon PID is %d\n",daemonPID);
		//~ exit(0);
	//~ }

	VERBOSE("myNetworkStartDaemon","start");

	char* clientId;
	int socket_descriptor;
	
	/* Declare connected */
	VERBOSE("DAEMON","Open socket");
	socket_descriptor = myNetworkCreateSocket();
	if(myNetworkOpenSocketConnexion(socket_descriptor)) {
		SUCCESS("DAEMON","Socket opened");
		VERBOSE("DAEMON","Connect client");
		clientId = myNetworkConnectClient(socket_descriptor);
		if(clientId != 0) {
			SUCCESS("DAEMON","Client connected");
		} else {
			WARNING("DAEMON","Client not connected");
		}
		
		VERBOSE("DAEMON","Close socket");
		myNetworkCloseSocketConnexion(socket_descriptor);
		SUCCESS("DAEMON","Socket closed");
	} else {
		WARNING("DAEMON","Socket not opened");
	}
	
	while(1) {
		VERBOSE("DAEMON","Open socket");
	socket_descriptor = myNetworkCreateSocket();
	if(myNetworkOpenSocketConnexion(socket_descriptor)) {
		SUCCESS("DAEMON","Socket opened");
		VERBOSE("DAEMON","ask waiting request");
		
		LinkedListString* req = myNetworkWaitingRequest(socket_descriptor,clientId);
		//
		myNetworkCloseSocketConnexion(socket_descriptor);
		//
		printf("%d && %d,   %s\n", req != 0, !sameString(getString(req,0),"NOTHING",7), toCharRequest(req));
		if(req != 0 && !sameString(getString(req,0),"NOTHING",7)) {
			char targetClient[11];
			char* line1 = getString(req,0);
			for(int i=7;i<strlen(line1);i++) {
				targetClient[i-7] = line1[i];
			}
			targetClient[11] = '\0';
			VERBOSE("DAEMON","target client >%s<(%d)",targetClient,strlen(targetClient));
			char* tmpFileName = "tmp.txt";
			FILE* tmpFile = fopen(tmpFileName,"w+");
			for(int i=0;i<getSize(req);i++) {
				fprintf(tmpFile,"%s",getString(req,i));
			}
			fclose(tmpFile);
			char* progRet = "je suis la sortie de mon programme";
			VERBOSE("DAEMON","prog ret >%s<",progRet);
			socket_descriptor = myNetworkCreateSocket();
			myNetworkOpenSocketConnexion(socket_descriptor);
			VERBOSE("DAEMON","myNetworkResponseRequest");
			myNetworkResponseRequest(socket_descriptor,clientId,targetClient,progRet);
			VERBOSE("DAEMON","myNetworkResponseRequest ok");
			myNetworkCloseSocketConnexion(socket_descriptor);
		} else {
			WARNING("DAEMON","error when asking waiting request");
		}
		
		VERBOSE("DAEMON","Close socket");
		//myNetworkCloseSocketConnexion(socket_descriptor);
		SUCCESS("DAEMON","Socket closed");
	} else {
		WARNING("DAEMON","Socket not opened");
	}
	}
	
	/* Declare disconnected */
	VERBOSE("DAEMON","Open socket");
	socket_descriptor = myNetworkCreateSocket();
	if(myNetworkOpenSocketConnexion(socket_descriptor)) {
		SUCCESS("DAEMON","Socket opened");
		VERBOSE("DAEMON","Disconnect client");
		if(myNetworkDisconnectClient(socket_descriptor,clientId)) {
			SUCCESS("DAEMON","Client disconnected");
		} else {
			WARNING("DAEMON","Client not disconnected");
		}
		
		VERBOSE("DAEMON","Close socket");
		myNetworkCloseSocketConnexion(socket_descriptor);
		SUCCESS("DAEMON","Socket closed");
	} else {
		WARNING("DAEMON","Socket not opened");
	}
	
	VERBOSE("myNetworkStartDaemon","end\n");
}

/* Definition of private functions */

char* runExternalProgram(char* programPath) {
	VERBOSE("DAEMON.runExternalProgram","start programPath >%s<\n",programPath);
	
	char buffer[5];
	char* stroutput = "";
	FILE *output = popen(programPath,"r");
	while(fgets(buffer,5,output) != NULL) {
		stroutput = concat(stroutput,buffer);
	}
	pclose(output);
	VERBOSE("DAEMON.runExternalProgram","end >%s<",stroutput);
	return stroutput;
}


int myNetworkCreateSocket() {
	if(!inited) {
		if ((ptr_host = gethostbyname(host_addr)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son adresse.");
			return -1;
		}
		inited = true;
	}
	bcopy((char*)ptr_host->h_addr, (char*)&adresse_locale.sin_addr, ptr_host->h_length);
	adresse_locale.sin_family = AF_INET; /* ou ptr_host->h_addrtype; */
	adresse_locale.sin_port = htons(host_port);
	if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("erreur : impossible de creer la socket de connexion avec le serveur.");
		return -1;
    }
	return socket_descriptor;
}

bool myNetworkOpenSocketConnexion(int socket) {
	if ((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de se connecter au serveur.");
		return false;
    }
    return true;
}

void myNetworkCloseSocketConnexion(int socket) {
	close(socket);
}

bool myNetworkWrite(int socket_descriptor, LinkedListString* msg) {
	char* line;

	for(int i=0;i<getSize(msg);i++) {

		line = concat(getString(msg,i),"\n");
		if((write(socket_descriptor, line, strlen(line))) < 0) {
			return false;
		}
	}
	if((write(socket_descriptor, "\n", 1)) < 0) {
		return false;
	}
	return true;
}

LinkedListString* myNetworkRead(int socket_descriptor) {
	// TODO cut buffer
	char buffer[256];
	int longueur;
	LinkedListString* request = makeLinkedListString();
	bool concatNext = false;

	while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
		printf("longueur:  %d   \n", longueur);
		if(longueur < 256)
			buffer[longueur] = '\0';
		if(longueur == 1 && buffer[0] == '\n')
			break;
		if(longueur > 1) {
			if(longueur == 256) {
				if(concatNext) {
					concatLastString(request,buffer);
				} else {
					addString(request,buffer);
				}
				concatNext = true;
			} else {
				if(concatNext) {
					concatLastString(request,buffer);
				} else {
					addString(request,buffer);
				}
				concatNext = false;
			}
		}
	}
	printf("longueur:  %d   %d\n", longueur, socket_descriptor);
	return request;
}

/* Definition of public functions */

char* myNetworkConnectCM(int socket_descriptor, bool isMaster) {
	VERBOSE("DAEMON.myNetworkConnectClient","start");
	LinkedListString* msg = makeLinkedListString();
	if(isMaster) {
		addString(msg,"MASTER");
	} else {
		addString(msg,"CLIENT");
	}
	addString(msg,"ON");
	LinkedListString* res;
	VERBOSE("DAEMON.myNetworkConnectClient","send connection msg");
	if(myNetworkWrite(socket_descriptor,msg)) {
		VERBOSE("DAEMON.myNetworkConnectClient","connection msg send");
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);
		VERBOSE("DAEMON.myNetworkConnectClient","before test OK");
		if(line1[0] == 'O' && line1[1] == 'K') {
			VERBOSE("DAEMON.myNetworkConnectClient","OK received");
			char* clientId = (char*)malloc(sizeof(char)*11);
			clientId[10] = '\0';
			VERBOSE("DAEMON.myNetworkConnectClient","copy id");
			for(int i=0;i<10;i++) {
				if(line1[i] != '\n') {
					clientId[i] = line1[3+i];
				} else {
					VERBOSE("DAEMON.myNetworkConnectClient","end 0");
					return 0;
				}
			}
			VERBOSE("DAEMON.myNetworkConnectClient","end >%s<",clientId);
			return clientId;
		} else {
			VERBOSE("DAEMON.myNetworkConnectClient","end 0");
			return 0;
		}
	}
	VERBOSE("DAEMON.myNetworkConnectClient","end 0");
	return 0;
}

char* myNetworkConnectClient(int socket_descriptor) {
	return myNetworkConnectCM(socket_descriptor,false);
}

char* myNetworkConnectMaster(int socket_descriptor) {
	return myNetworkConnectCM(socket_descriptor,true);
}

bool myNetworkDisconnectClient(int socket_descriptor, char* clientId) {
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("CLIENT ",clientId));
	addString(msg,"OFF");
	LinkedListString* res;
	if(myNetworkWrite(socket_descriptor,msg)) {
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);
		if(line1[0] == 'O' && line1[1] == 'K') {
			return true;
		} else {
			return false;
		}
	}
	return false;
}

char* myNetworkReserveClient(int socket_descriptor, char* clientId) {
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("MASTER ",clientId));
	addString(msg,"TRY");
	LinkedListString* res;
	if(myNetworkWrite(socket_descriptor,msg)) {
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);

		if(line1[0] == 'O' && line1[1] == 'K') {
			char* clientId = (char*)malloc(sizeof(char)*11);
			clientId[10] = '\0';
			for(int i=0;i<10;i++) {
				if(line1[i] != '\n') {
					clientId[i] = line1[3+i];
				} else {
					return 0;
				}
			}
			return clientId;
		} else {
			return 0;
		}
	}
	return 0;
}

bool myNetworkAskClient(int socket_descriptor, char* clientId, char* targetClient, char* data) {
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("MASTER ",clientId));
	addString(msg,concat("ASK ",targetClient));
	addString(msg,data);

	LinkedListString* res;

	if(myNetworkWrite(socket_descriptor,msg)) {
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);
		if(line1[0] == 'O' && line1[1] == 'K') {
			char* clientId = line1+(3*sizeof(char));
			clientId[10] = '\0';
			if(sameString(targetClient,clientId,10)) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
	return false;
}

LinkedListString* myNetworkWaitingRequest(int socket_descriptor, char* clientId) {
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("CLIENT ",clientId));
	addString(msg,"WAITING");
	if(myNetworkWrite(socket_descriptor,msg)) {
		LinkedListString* res = myNetworkRead(socket_descriptor);
		if(res != 0) {
			return res;
		} else {
			return 0;
		}
	}
	return 0;
}

bool myNetworkResponseRequest(int socket_descriptor, char* clientId, char* targetClient, char* data) {
	VERBOSE("DAEMON.myNetworkResponseRequest","start");
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("MASTER ",targetClient));
	targetClient[10] = '\0';
	addString(msg,concat("RES ",clientId));
	addString(msg,data);

	VERBOSE("DAEMON.myNetworkResponseRequest","send res");
	LinkedListString* res;
	if(myNetworkWrite(socket_descriptor,msg)) {
		VERBOSE("DAEMON.myNetworkResponseRequest","write ok");
		res = myNetworkRead(socket_descriptor);
		VERBOSE("DAEMON.myNetworkResponseRequest","read ok (%d)",getSize(res));
		char* line1 = getString(res,0);
		if(line1[0] == 'O' && line1[1] == 'K') {
			VERBOSE("DAEMON.myNetworkResponseRequest","end ok");
			return true;
		} else {
			VERBOSE("DAEMON.myNetworkResponseRequest","end 1 >%s<",line1);
			return false;
		}
	}
	VERBOSE("DAEMON.myNetworkResponseRequest","end 0");
	return false;
}

