#include "Client.h"
#include <stdlib.h>

char* strClientState(ClientState state) {
	switch(state) {
	case FREE:
		return "FREE";
	case RESERVED:
		return "RESERVED";
	case WORKING:
		return "WORKING";
	}
	return "ERROR";
}

Client* makeClient(char* id) {
	Client* c = (Client*)malloc(sizeof(Client));
	setClientId(c,id);
	c->state = FREE;
	c->usedBy = 0;
	c->waiting = makeRequestQueue();
	return c;
}

Client* cloneClient(Client* c) {
	Client* clone = makeClient(c->id);
	clone->state = c->state;
	clone->usedBy = c->usedBy;
	return clone;
}

void setClientId(Client* c, char* id) {
	for(int i=0;i<10;i++) {
		c->id[i] = id[i];
	}
}

void setClientState(Client* c, ClientState state) {
	c->state = state;
}

void setClientUsedBy(Client* c, Client* usedBy) {
	c->usedBy = usedBy;
}
