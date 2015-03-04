#ifndef __LIBMYNETWORK__
#define __LIBMYNETWORK__

#include <stdbool.h>
#include "LinkedListString.h"

void myNetworkStartDaemon(char* programPath, int listenPort);

char* myNetworkConnectClient(int socket_descriptor);
bool myNetworkDisconnectClient(int socket_descriptor, char* clientId);

char* myNetworkReserveClient(int socket_descriptor, char* clientId);
bool myNetworkAskClient(int socket_descriptor, char* clientId, char* targetClient, char* data);
LinkedListString* myNetworkWaitingRequest(int socket_descriptor, char* clientId);

#endif /* __LIBMYNETWORK__ */
