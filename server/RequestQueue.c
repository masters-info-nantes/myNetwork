#include <stdio.h>
#include <stdlib.h>
#include "RequestQueue.h"

RequestQueue* makeRequestQueue() {
	RequestQueue* q = (RequestQueue*)malloc(sizeof(RequestQueue));
	pthread_mutex_init(&(q->mutex),NULL);
	return q;
}

void deleteRequestQueue(RequestQueue* q, bool withContent) {
	pthread_mutex_lock(&(q->mutex));
	RequestElement* current = q->head;
	RequestElement* next;
	while(current != NULL) {
		next = current->next;
		if(withContent) {
			free(current->content);
		}
		free(current);
		current = next;
	}
	pthread_mutex_unlock(&(q->mutex));
	pthread_mutex_destroy(&(q->mutex));
	free(q);
}


void enqueueRequest(RequestQueue* q, LinkedListString* newElt) {
	pthread_mutex_lock(&(q->mutex));
	RequestElement* e = (RequestElement*)malloc(sizeof(RequestElement));
	e->next = NULL;
	e->content = newElt;
	if(requestQueueIsEmpty(q)) {
		q->tail = e;
		q->head = e;
	} else {
		q->tail->next = e;
		q->tail = e;
	}
	pthread_mutex_unlock(&(q->mutex));
}

LinkedListString* dequeueRequest(RequestQueue* q) {
	pthread_mutex_lock(&(q->mutex));
	RequestElement* e = NULL;
	if(!requestQueueIsEmpty(q)) {
		e = q->head;
		if(e->next == NULL) {
			q->head = NULL;
			q->tail = NULL;
		} else {
			q->head = e->next;
		}
	}
	pthread_mutex_unlock(&(q->mutex));
	if(e == NULL) {
		return NULL;
	} else {
		return e->content;
	}
}

bool requestQueueIsEmpty(RequestQueue* q) {
	if(q->head == NULL)
		return true;
	return false;
}
