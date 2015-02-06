#include "common.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "colorlog.h"
#include "Queue.h"

#include "producer.h"
#include "consumer.h"
#include "help.h"
#include "version.h"

#define MODE_NORMAL 0
#define MODE_DAEMON 1
#define MODE_HELP 2
#define MODE_VERSION 3

int mode = MODE_NORMAL;

int start();
int readArgs(int argc, char** argv);

int main(int argc, char** argv) {
	VERBOSE("main","start");

	readArgs(argc,argv);
	switch(mode) {
	case MODE_HELP:
		modeHelp();
		exit(0);
		break;
	case MODE_VERSION:
		modeVersion();
		exit(0);
		break;
	case MODE_DAEMON:{
			pid_t daemonPID = fork();
			if(daemonPID != 0) {
				printf("Daemon PID is %d\n",daemonPID);
				exit(0);
			}
		}
		break;
	}
	start();
	VERBOSE("main","end");
	return 0;
}

int start() {
	waiting = makeQueue();
	pthread_create(&threads[THREAD_PRODUCER],NULL,&producer,NULL);
	pthread_create(&threads[THREAD_CONSUMER],NULL,&consumer,NULL);
	pthread_create(&threads[THREAD_CONSUMER2],NULL,&consumer,NULL);
	pthread_join(threads[THREAD_PRODUCER],NULL);
	pthread_join(threads[THREAD_CONSUMER],NULL);
	pthread_join(threads[THREAD_CONSUMER2],NULL);
	deleteQueue(waiting,true);
	return 0;
}

int readArgs(int argc, char** argv) {
	for(int i=0;i<argc;i++) {
		if(strncmp("-h",argv[i],2) == 0 || strncmp("--help",argv[i],6) == 0) {
			mode = MODE_HELP;
		} else if(strncmp("-d",argv[i],2) == 0 || strncmp("--daemon",argv[i],8) == 0) {
			mode = MODE_DAEMON;
		} else if(strncmp("-v",argv[i],2) == 0 || strncmp("--version",argv[i],9) == 0) {
			mode = MODE_VERSION;
		}
	}
	return 0;
}
