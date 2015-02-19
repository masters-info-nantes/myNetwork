#ifndef __LISTCLIENT__
#define __LISTCLIENT__
#include <stdbool.h>
#include <pthread.h>
#include "Client.h"

typedef struct ClientEntry ClientEntry;
struct ClientEntry {
	ClientEntry* back;
	ClientEntry* next;
	Client* content;
};

typedef struct {
	ClientEntry* first;
	ClientEntry* last;
	int size;
	pthread_mutex_t mutex;
} ListClient;

ListClient* makeListClient();
void deleteListClient(ListClient* l, bool withContent);

void addClient(ListClient* l, Client* c);
Client* getClient(ListClient* l, int index);
int getClientIndex(ListClient* l, char* id);
int getSizeListClient(ListClient* l);
Client* removeClient(ListClient* l, int index);

void printListClient(ListClient* l);

// synchronized functions
Client* reserveClientFromListClient(ListClient* l, Client* askReservation);

#endif /* __LISTCLIENT__ */
