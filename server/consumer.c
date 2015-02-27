#include "common.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "consumer.h"
#include "colorlog.h"
#include "Queue.h"
#include "LinkedListString.h"
#include "Client.h"
#include "ListClient.h"
#include "parser.h"

void* consumer(void* arg) {
	VERBOSE("CONSUMER","start");

	int *client;
	while(true) {
		client = dequeue(waiting);
		VERBOSE("CONSUMER","new try %p",client);
		if(client != NULL) {
			INFO("CONSUMER","new client");
			char buffer[BUFFER_SIZE];

			LinkedListString* request = makeLinkedListString();
			int longueur = 0;
			bool concatNext = false;
			while(longueur != 1 && longueur <= BUFFER_SIZE) {
				if ((longueur = read(*client, buffer, sizeof(buffer))) <= 0) {
					ALERT("CONSUMER","can't read more");
					close(*client);
					break;
				}
				if(longueur > 0) {
					if(longueur < BUFFER_SIZE)
						buffer[longueur] = '\0';
					if(longueur > 1) {
						if(longueur == BUFFER_SIZE) {
							if(concatNext) {
								concatLastString(request,buffer);
							} else {
								addString(request,buffer);
							}
							concatNext = true;
						} else {
							if(concatNext) {
								concatLastString(request,buffer);
							} else {
								addString(request,buffer);
							}
							concatNext = false;
						}
					}
					VERBOSE("CONSUMER","received : >%s<(%d)",buffer,longueur);
				}
			}
			if(getSize(request) > 0) {
				VERBOSE("CONSUMER","end reading");
				char* response = processRequest(request);
				INFO("CONSUMER","processRequest");

				printListClient(listClient);
				INFO("CONSUMER","printListClient");

				write(*client,response,strlen(response));
				INFO("CONSUMER","response send response");

				close(*client);
			}
		} else {
			VERBOSE("CONSUMER","no waiting client");
			sleep(1);
		}
	}
	VERBOSE("CONSUMER","end");
	return NULL;
}
