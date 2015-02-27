#include "ListClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colorlog.h"
#include "common.h"

#ifndef NULL
#define NULL 0
#endif

ListClient* makeListClient() {
	ListClient* l = (ListClient*)malloc(sizeof(ListClient));
	l->size = 0;
	l->first = 0;
	l->last = 0;
	pthread_mutex_init(&(l->mutex),NULL);
	return l;
}

void deleteListClient(ListClient* l, bool withContent) {
	ClientEntry* current = l->first;
	ClientEntry* next;
	while(current != NULL) {
		next = current->next;
		if(withContent) {
			free(current->content);
		}
		free(current);
		current = next;
	}
	pthread_mutex_destroy(&(l->mutex));
	free(l);
}

void addClient(ListClient* l, Client* c) {
	ClientEntry* e = (ClientEntry*)malloc(sizeof(ClientEntry));
	e->back = NULL;
	e->next = NULL;
	e->content = cloneClient(c);
	if(getSizeListClient(l) == 0) {
		l->last = e;
		l->first = e;
		l->size++;
	} else {
		e->back = l->last;
		l->last->next = e;
		l->last = e;
		l->size++;
	}
}



ClientEntry* getClientEntry(ListClient* l, int index) {
	if(index == 0) {
		return l->first;
	} else if(index == (l->size-1)) {
		return l->last;
	} else if(index < l->size && index > 0) {
		ClientEntry* cur = l->first;
		for(int i=1;i<=index;i++) {
			cur = cur->next;
		}
		return cur;
	}
	return 0;
}

Client* getClient(ListClient* l, int index) {
	ClientEntry* entry = getClientEntry(l,index);
	if(entry != 0) {
		return entry->content;
	}
	return 0;
}

int getClientIndex(ListClient* l, char* id) {
	ClientEntry* cur = l->first;
	int i = 0;
	while(cur != 0) {
		if(sameString(cur->content->id,id,10)) {
			return i;
		}
		i++;
		cur = cur->next;
	}
	return -1;
}

int getSizeListClient(ListClient* l) {
	return l->size;
}

Client* removeClient(ListClient* l, int index) {
	ClientEntry* entry = getClientEntry(l,index);
	if(entry != 0) {
		if(entry->back == 0) {
			if(entry->next == 0) {
				l->first = 0;
			} else {
				l->first = entry->next;
			}
		} else {
			entry->back->next = entry->next;
		}
		if(entry->next == 0) {
			if(entry->back == 0) {
				l->last = 0;
			} else {
				l->last = entry->back;
			}
		} else {
			entry->next->back = entry->back;
		}
		l->size--;
		Client* c = entry->content;
		free(entry);
		return c;
	}
	return 0;
}

void printListClient(ListClient* l) {
	printf("ListClient (%d) :\n",getSizeListClient(l));
	if(getSizeListClient(l) > 0) {
		ClientEntry *cur = l->first;
		while(cur != 0) {
			printf("\t[%s,%s]\n",cur->content->id,strClientState(cur->content->state));
			cur = cur->next;
		}
	} else {
		printf("\tempty\n");
	}
	printf("\n");
}


Client* reserveClientFromListClient(ListClient* l, Client* askReservation) {
	Client* ret = 0;
	pthread_mutex_lock(&(l->mutex));
	ClientEntry* cur = l->first;
	while(cur != 0) {
		if(cur->content->state == FREE) {
			ret = cur->content;
			setClientState(ret,RESERVED);
			setClientUsedBy(ret,askReservation);
			break;
		}
		cur = cur->next;
	}
	pthread_mutex_unlock(&(l->mutex));
	return ret;
}
