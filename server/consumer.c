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
			char cut[BUFFER_SIZE];

			LinkedListString* request = makeLinkedListString();
			//~ LinkedListString* cutBuffer = makeLinkedListString();
			int longueur = 0;
			bool concatNext = false;
			bool endOfRequest = false;
			while(longueur != 1 && longueur <= BUFFER_SIZE) {
				VERBOSE("CONSUMER","reading socket");
				if(endOfRequest) {
					VERBOSE("CONSUMER","end of request");
					break;
				}
				if ((longueur = read(*client, buffer, sizeof(buffer))) <= 0) {
					ALERT("CONSUMER","can't read more");
					close(*client);
					break;
				}
				
				if(longueur > 0) {
					if(longueur < BUFFER_SIZE)
						buffer[longueur] = '\0';
					VERBOSE("CONSUMER","start read buffer >%s<(%d)",buffer,longueur);
					if(longueur == 1 && buffer[0] == '\n') {
						VERBOSE("CONSUMER","break reading socket");
						break;
					}
					if(longueur > 1) {
						int cuti = 0;
						for(int i=0;i<longueur;i++) {
							if(buffer[i] == '\n') {
								if(buffer[i-1] == '\n') {
									endOfRequest = true;
									break;
								}
								//~ cut[cuti] = '\n';
								//~ cut[cuti+1] = '\0';
								cut[cuti] = '\0';
								if(longueur == BUFFER_SIZE) {
									if(concatNext) {
										concatLastString(request,cut);
										VERBOSE("CONSUMER","concatLastString(_,>%s<) cuti(%d)",cut,cuti);
									} else {
										addString(request,cut);
										VERBOSE("CONSUMER","addString(_,>%s<) cuti(%d)",cut,cuti);
									}
									concatNext = true;
								} else {
									if(concatNext) {
										concatLastString(request,cut);
										VERBOSE("CONSUMER","concatLastString(_,>%s<) cuti(%d)",cut,cuti);
									} else {
										addString(request,cut);
										VERBOSE("CONSUMER","addString(_,>%s<) cuti(%d)",cut,cuti);
									}
									concatNext = false;
								}
								cuti = 0;
							} else {
								cut[cuti] = buffer[i];
								cuti++;
							}
							
						}
						VERBOSE("CONSUMER","end cut (%d)",longueur);
					}
				}
				//~ if(longueur > 0) {
					//~ if(longueur < BUFFER_SIZE)
						//~ buffer[longueur] = '\0';
					//~ if(longueur > 1) {
						//~ if(longueur == BUFFER_SIZE) {
							//~ if(concatNext) {
								//~ concatLastString(request,buffer);
							//~ } else {
								//~ addString(request,buffer);
							//~ }
							//~ concatNext = true;
						//~ } else {
							//~ if(concatNext) {
								//~ concatLastString(request,buffer);
							//~ } else {
								//~ addString(request,buffer);
							//~ }
							//~ concatNext = false;
						//~ }
					//~ }
					VERBOSE("CONSUMER","received : >%s<(%d)",buffer,longueur);
				//~ }
			}
			VERBOSE("CONSUMER","start read request (req size %d)",getSize(request));
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
