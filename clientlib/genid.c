#include "genid.h"
#include <pthread.h>
#include <stdio.h>

int __nextClientId = 0;
char __nextClientIdStr[10];
pthread_mutex_t __nextClientIdMutex;

void genNextClientIdInit() {
	pthread_mutex_init(&__nextClientIdMutex,NULL);
}

void genNextClientIdEnd() {
	pthread_mutex_destroy(&__nextClientIdMutex);
}

char* genNextClientId() {
	pthread_mutex_lock(&__nextClientIdMutex);
	sprintf(__nextClientIdStr,"%010d", __nextClientId);
	__nextClientId++;
	pthread_mutex_unlock(&__nextClientIdMutex);
	return __nextClientIdStr;
}
