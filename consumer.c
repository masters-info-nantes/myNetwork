#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "consumer.h"
#include "Queue.h"

extern Queue* waiting;

void* consumer(void* arg) {
	int *val;
	while(true) {
		printf("\tCONSUMER start\n");
		val = dequeue(waiting);
		if(val != NULL) {
			printf("\tCONSUMER dequeue(%d)\n",*val);
		} else {
			printf("\tCONSUMER dequeue(NULL)\n");
			sleep(1);
		}
		printf("\tCONSUMER end\n");
	}
	return NULL;
}
