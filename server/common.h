#ifndef __COMMON__
#define __COMMON__

#include <pthread.h>
#include "Queue.h"

#define BUFFER_SIZE 256
#define MAX_MALLOC_TRY 10
#define SOCKET_PORT 5000
#define SOCKET_WAITING_QUEUE_SIZE 10


#define THREAD_PRODUCER 0
#define THREAD_CONSUMER 1
#define THREAD_CONSUMER2 2

pthread_t threads[3];
Queue* waiting;

#define uint unsigned int

#endif /* __COMMON__ */
