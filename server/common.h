#ifndef __COMMON__
#define __COMMON__

#include <pthread.h>
#include "Queue.h"
#include "ListClient.h"

#define PROGRAM_NAME "MyNetwork"
#define PROGRAM_VERSION "0.0.1a"
#define PROGRAM_AUTHORS "Franck Boncler & Anthony Pena"


#define BUFFER_SIZE 256
#define SOCKET_PORT 5000
#define SOCKET_WAITING_QUEUE_SIZE 10


#define THREAD_PRODUCER 0
#define THREAD_CONSUMER 1
#define THREAD_CONSUMER2 2

pthread_t threads[3];
Queue* waiting;
ListClient* listClient;

#define uint unsigned int

char* concat(char* s1, char* s2);
char* clonestr(char* s);
bool sameString(char* s1, char* s2, int nbCharToCompare);

#endif /* __COMMON__ */
