#include "common.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "consumer.h"
#include "colorlog.h"
#include "Queue.h"

void* consumer(void* arg) {
	VERBOSE("CONSUMER","start");
	int *client;
	while(true) {
		client = dequeue(waiting);
		VERBOSE("CONSUMER","new try %p",client);
		if(client != NULL) {
			INFO("CONSUMER","new client");

			char buffer[BUFFER_SIZE];
			//~ char** request;
			//~ int** requestSize;
			int longueur = 0;
			while(longueur <= BUFFER_SIZE) {
				if ((longueur = read(*client, buffer, sizeof(buffer))) <= 0) {
					ALERT("CONSUMER","can't read more");
					break;
				}
				SUCCESS("CONSUMER","received : >%s<(%d)",buffer,longueur);
				if(longueur == 1)
					break;
			}
			SUCCESS("CONSUMER","end reading");
			buffer[0] = 'O';
			buffer[1] = 'K';
			buffer[2] = '\0';
			write(*client,buffer,3);
			close(*client);
		} else {
			INFO("CONSUMER","no waiting client");
			sleep(1);
		}
	}
	VERBOSE("CONSUMER","end");
	return NULL;
}

//~ char** processRequest(char** request, int** requestSize) {
	//~ return;
//~ }
