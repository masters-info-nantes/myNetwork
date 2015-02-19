#ifndef __CLIENT__
#define __CLIENT__
#include <stdbool.h>
#include "RequestQueue.h"
#include "LinkedListString.h"

typedef enum {
	FREE,
	RESERVED,
	WORKING
} ClientState;

char* strClientState(ClientState state);

typedef struct Client Client;
struct Client {
	char id[10];
	ClientState state;
	Client* usedBy;
	RequestQueue* waiting;
};

Client* makeClient(char* id);
Client* cloneClient(Client* c);
void setClientId(Client* c, char* id);
void setClientState(Client* c, ClientState state);
void setClientUsedBy(Client* c, Client* usedBy);

#endif /* __CLIENT__ */
