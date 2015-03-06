#ifndef __LIBMYNETWORK__
#define __LIBMYNETWORK__

#include <stdbool.h>
#include "LinkedListString.h"

void myNetworkStartDaemon(char* programPath, int listenPort);

int myNetworkCreateSocket();
bool myNetworkOpenSocketConnexion(int socket);
void myNetworkCloseSocketConnexion(int socket);

char* myNetworkConnectClient(int socket_descriptor);
char* myNetworkConnectMaster(int socket_descriptor);
bool myNetworkDisconnectClient(int socket_descriptor, char* clientId);

char* myNetworkReserveClient(int socket_descriptor, char* clientId);
bool myNetworkAskClient(int socket_descriptor, char* clientId, char* targetClient, char* data);
LinkedListString* myNetworkWaitingRequest(int socket_descriptor, char* clientId);
bool myNetworkResponseRequest(int socket_descriptor, char* clientId, char* targetClient, char* data);

#endif /* __LIBMYNETWORK__ */
