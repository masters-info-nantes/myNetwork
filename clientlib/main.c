#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libmynetwork.h"

#define MODE_NORMAL 0
#define MODE_DAEMON 1
#define MODE_INPUT_FILE 2

int mode;
char* file;

void normalMode();
void inputFileMode();

int main(int argc, char** argv) {
	mode = MODE_NORMAL;
	int port = 5000;
	for(int i=0;i<argc;i++) {
		if(strncmp("-p",argv[i],2) == 0 || strncmp("--port",argv[i],6) == 0) {
			int tmp = atoi(argv[i+1]);
			if(tmp > 0) {
				port = tmp;
				i++;
			}
		} else if(strncmp("-d",argv[i],2) == 0 || strncmp("--daemon",argv[i],8) == 0) {
			mode = MODE_DAEMON;
		} else if(strncmp("-i",argv[i],2) == 0) {
			mode = MODE_INPUT_FILE;
			file = argv[i+1];
			i++;
		}
	}
	
	switch(mode) {
	case MODE_DAEMON:
		myNetworkStartDaemon("./client -i",port);
		break;
	case MODE_INPUT_FILE:
		inputFileMode();
		break;
	case MODE_NORMAL:
		normalMode();
		break;
	default:
		perror("no valid mode\n");
	}
	return 0;
}

void normalMode() {
	sleep(1);
	printf("I'm the Doctor !\n");
}

void inputFileMode() {
	sleep(1);
	printf("INPUT FILE MODE");
}
