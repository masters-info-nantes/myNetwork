#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "producer.h"
#include "colorlog.h"
#include "Queue.h"
#include <linux/types.h> 	/* pour les sockets */
#include <sys/socket.h>
#include <netdb.h> 		/* pour hostent, servent */
#include <strings.h> 		/* pour bcopy, ... */
#define TAILLE_MAX_NOM 256

#define ARRAYSIZE(x) (sizeof(x)/sizeof(*x))

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

int* newIntPtr(int i) {
	int* ptr = (int*)malloc(sizeof(int));
	if(ptr == NULL)
		return NULL;
	*ptr = i;
	return ptr;
}

void* producer(void* arg) {
	VERBOSE("PRODUCER","start");

	int socket_descriptor; /* descripteur de socket */
	int nouv_socket_descriptor; /* [nouveau] descripteur de socket */
	uint longueur_adresse_courante; /* longueur d'adresse courante d'un client */
    sockaddr_in adresse_locale; /* structure d'adresse locale*/
	sockaddr_in adresse_client_courant; /* adresse client courant */
    hostent* ptr_hote; /* les infos recuperees sur la machine hote */
    char machine[TAILLE_MAX_NOM+1]; /* nom de la machine locale */
	int* ptrSocket = 0;
    gethostname(machine,TAILLE_MAX_NOM); /* recuperation du nom de la machine */
    //~ gethostbyname(machine); /* recuperation du nom de la machine */

	/* recuperation de la structure d'adresse en utilisant le nom */
    if ((ptr_hote = gethostbyname(machine)) == NULL) {
		perror("erreur : impossible de trouver le serveur a partir de son nom.");
		exit(1);
    }

	/* initialisation de la structure adresse_locale avec les infos recuperees */

	/* copie de ptr_hote vers adresse_locale */
	bcopy((char*)ptr_hote->h_addr_list[0], (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	adresse_locale.sin_family = ptr_hote->h_addrtype; /* ou AF_INET */
	adresse_locale.sin_addr.s_addr = INADDR_ANY; /* ou AF_INET */

	adresse_locale.sin_port = htons(SOCKET_PORT);

	INFO("PRODUCER","Serveur will be connected to port : %d",ntohs(adresse_locale.sin_port));


	/* creation de la socket */
    if ((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		//~ perror("erreur : impossible de creer la socket de connexion avec le client.");
		ALERT("PRODUCER","impossible to create socket connexion");
		exit(1);
    }

    /* association du socket socket_descriptor à la structure d'adresse adresse_locale */
    if ((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0) {
		perror("erreur : impossible de lier la socket a l'adresse de connexion.");
		exit(1);
    }

    /* initialisation de la file d'ecoute */
    listen(socket_descriptor,SOCKET_WAITING_QUEUE_SIZE);

    longueur_adresse_courante = sizeof(adresse_client_courant);
	VERBOSE("PRODUCER","start listen");
	while(true) {
		VERBOSE("PRODUCER","wait client");
		nouv_socket_descriptor = accept(socket_descriptor,(sockaddr*)(&adresse_client_courant), &longueur_adresse_courante);
		VERBOSE("PRODUCER","new client");
		if(nouv_socket_descriptor < 0) {
			ALERT("PRODUCER","impossible d'accepter la connexion avec le client.");
			exit(1);
		}
		VERBOSE("PRODUCER","put to waiting queue client");
		ptrSocket = newIntPtr(nouv_socket_descriptor);
		if(ptrSocket == NULL) {
			ALERT("PRODUCER","dynamic allocation problem");
			exit(1);
		}
		enqueue(waiting,ptrSocket);
	}
	VERBOSE("PRODUCER","end");
	return NULL;
}
