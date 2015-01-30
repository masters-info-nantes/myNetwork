#ifndef __QUEUE__
#define __QUEUE__

#ifndef NULL
#define NULL 0
#endif

#include <pthread.h>
#include <stdbool.h>

typedef struct Element Element;
struct Element {
	Element* next;
	int* content;
};

typedef struct {
   Element* head;
   Element* tail;
   pthread_mutex_t mutex;
} Queue;


Queue* makeQueue();
void deleteQueue(Queue* q, bool withContent);

void enqueue(Queue* q, int* newElt);
int* dequeue(Queue* q);
bool queueIsEmpty(Queue* q);


#endif /* __QUEUE__ */
