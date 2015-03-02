#include "libmynetwork.h"
#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "common.h"
#include "colorlog.h"

void runExternalProgram(char* programPath);

void myNetworkStartDaemon(char* programPath) {
	pid_t daemonPID = fork();
	if(daemonPID != 0) {
		printf("Daemon PID is %d\n",daemonPID);
		exit(0);
	}
	VERBOSE("myNetworkStartDaemon","start");
	
	runExternalProgram(programPath);
	
	VERBOSE("myNetworkStartDaemon","end\n");
}

void runExternalProgram(char* programPath) {
	printf("programPath >%s<\n",programPath);
	
	char buffer[5];
	char* stroutput = "";
	FILE *output = popen(programPath,"r");
	while(fgets(buffer,5,output) != NULL) {
		stroutput = concat(stroutput,buffer);
	}
	pclose(output);
	printf(">%s<",stroutput);
	
}

myNetwork_client_id myNetworkReserveClient() {
	
}

void myNetworkSendTaskResponse(char* res) {
	printf("res = >%s<\n",res);
}


