#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "producer.h"
#include "Queue.h"

extern Queue* waiting;

int* newIntPtr(int i) {
	int* ptr = (int*)malloc(sizeof(int));
	*ptr = i;
	return ptr;
}

void* producer(void* arg) {
	int i = 0;
	while(i < 10) {
		printf("PRODUCER start\n");
		enqueue(waiting,newIntPtr(i));
		printf("PRODUCER enqueue(%d)\n",i);
		i++;
		printf("PRODUCER end\n");
	}
	return NULL;
}
