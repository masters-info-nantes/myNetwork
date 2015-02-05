#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

Queue* makeQueue() {
	Queue* q = (Queue*)malloc(sizeof(Queue));
	pthread_mutex_init(&(q->mutex),NULL);
	return q;
}

void deleteQueue(Queue* q, bool withContent) {
	pthread_mutex_lock(&(q->mutex));
	Element* current = q->head;
	Element* next;
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


void enqueue(Queue* q, int* newElt) {
	pthread_mutex_lock(&(q->mutex));
	Element* e = (Element*)malloc(sizeof(Element));
	e->next = NULL;
	e->content = newElt;
	if(queueIsEmpty(q)) {
		q->tail = e;
		q->head = e;
	} else {
		q->tail->next = e;
		q->tail = e;
	}
	pthread_mutex_unlock(&(q->mutex));
}

int* dequeue(Queue* q) {
	pthread_mutex_lock(&(q->mutex));
	Element* e = NULL;
	if(!queueIsEmpty(q)) {
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

bool queueIsEmpty(Queue* q) {
	if(q->head == NULL)
		return true;
	return false;
}
