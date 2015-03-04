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
hostent * ptr_host;
servent * ptr_service;
char* host_addr = "localhost";
int host_port = 5000;
char clientId[11];

// private functions
char* runExternalProgram(char* programPath);

int myNetworkCreateSocket();
bool myNetworkOpenSocketConnexion(int socket);
void myNetworkCloseSocketConnexion(int socket);

bool myNetworkWrite(int socket_descriptor, LinkedListString* msg);
LinkedListString* myNetworkRead(int socket_descriptor);


void myNetworkStartDaemon(char* programPath, int listenPort) {
	host_port = listenPort;
	//~ pid_t daemonPID = fork();
	//~ if(daemonPID != 0) {
		//~ printf("Daemon PID is %d\n",daemonPID);
		//~ exit(0);
	//~ }
	VERBOSE("myNetworkStartDaemon","start");
	char* clientId;
	int socket_descriptor;
	
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
	printf("programPath >%s<\n",programPath);
	
	char buffer[5];
	char* stroutput = "";
	FILE *output = popen(programPath,"r");
	while(fgets(buffer,5,output) != NULL) {
		stroutput = concat(stroutput,buffer);
	}
	pclose(output);
	//~ printf(">%s<",stroutput);
	return stroutput;
}


int myNetworkCreateSocket() {
	if ((ptr_host = gethostbyname(host_addr)) == NULL) {
	perror("erreur : impossible de trouver le serveur a partir de son adresse.");
		return -1;
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
	char buffer[256];
	int longueur;
	LinkedListString* request = makeLinkedListString();
	bool concatNext = false;
	while((longueur = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {
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
	return request;
}

/* Definition of public functions */

char* myNetworkConnectClient(int socket_descriptor) {
	VERBOSE("DAEMON.myNetworkConnectClient","start");
	//~ char* msg = "CLIENT\nON\n\n";
	LinkedListString* msg = makeLinkedListString();
	addString(msg,"CLIENT");
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
			char* clientId = (char*)malloc(sizeof(char)*10);
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

bool myNetworkDisconnectClient(int socket_descriptor, char* clientId) {
	//~ char* msg = concat("CLIENT ",concat(clientId,"\nOFF\n\n"));
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
	//~ char* msg = concat("CLIENT ",concat(clientId,"\nTRY\n\n"));
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("CLIENT ",clientId));
	addString(msg,"TRY");
	LinkedListString* res;
	if(myNetworkWrite(socket_descriptor,msg)) {
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);
		if(line1[0] == 'O' && line1[1] == 'K') {
			char* clientId = (char*)malloc(sizeof(char)*10);
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
	//~ char* msg = concat("CLIENT ",concat(clientId,concat("\nASK ",concat(targetClient,concat("\n",concat(data,"\n\n"))))));
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("CLIENT ",clientId));
	addString(msg,concat("ASK ",targetClient));
	addString(msg,data);
	
	LinkedListString* res;
	if(myNetworkWrite(socket_descriptor,msg)) {
		res = myNetworkRead(socket_descriptor);
		char* line1 = getString(res,0);
		if(line1[0] == 'O' && line1[1] == 'K') {
			if(sameString(targetClient,line1+(3*sizeof(char)),10)) {
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
	//~ char* msg = concat("CLIENT ",concat(clientId,"\nWAITING\n\n"));
	LinkedListString* msg = makeLinkedListString();
	addString(msg,concat("CLIENT ",clientId));
	addString(msg,"WAITING");
	if(myNetworkWrite(socket_descriptor,msg)) {
		return myNetworkRead(socket_descriptor);
	}
	return 0;
}

