#ifndef __REQUESTQUEUE__
#define __REQUESTQUEUE__

#ifndef NULL
#define NULL 0
#endif

#include <pthread.h>
#include <stdbool.h>
#include "LinkedListString.h"

typedef struct RequestElement RequestElement;
struct RequestElement {
	RequestElement* next;
	LinkedListString* content;
};

typedef struct {
   RequestElement* head;
   RequestElement* tail;
   pthread_mutex_t mutex;
} RequestQueue;


RequestQueue* makeRequestQueue();
void deleteRequestQueue(RequestQueue* q, bool withContent);

void enqueueRequest(RequestQueue* q, LinkedListString* newElt);
LinkedListString* dequeueRequest(RequestQueue* q);
bool requestQueueIsEmpty(RequestQueue* q);


#endif /* __REQUESTQUEUE__ */
