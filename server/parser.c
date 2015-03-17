#include "parser.h"
#include <string.h>
#include "common.h"
#include "colorlog.h"
#include "Client.h"
#include "ListClient.h"
#include "genid.h"

char* newClientConnection();
char* clientDisconnection(char* clientId);
char* waitingRequest(char* clientId);
char* reserveClient(char* masterId);
char* assignTaskToClient(char* masterId, char* targetClientId, char* data);
char* responseForTask(char* masterId, char* clientId, char* data);

char* processRequest(LinkedListString* request) {
	INFO("CONSUMER","start read request");
	for(int i=0;i<getSize(request);i++) {
		char* tmp = getString(request,i);
		INFO("CONSUMER",">%s<(%d)",tmp,(int)strlen(tmp));
	}
	INFO("CONSUMER","end read request");
	char* response = "KO unknown error";// TODO remove test default response
	char* line = getString(request,0);
	if(sameString(line,"CLIENT",6)) {
		if(line[6] == '\n' || line[6] == '\0') {// new client connection
			if(sameString(getString(request,1),"ON",2)) {// valid
				INFO("CONSUMER.processRequest","new client connection");
				response = newClientConnection(FREE);
			} else {// error
				return "KO parsing error (connection request)\n";
			}
		} else {//client disconnection or waiting
			for(int i=7;i<17;i++) {
				if(line[i] == '\n' || line[i] == '\0') {
					ALERT("CONSUMER.processRequest","error i=%d val=>%c</>%d< back=>%c</>%d< next=>%c</>%d<",i,line[i],line[i],line[i-1],line[i-1],line[i+1],line[i+1]);
					return "KO parsing error (client id size not incorrect)\n";
				}
			}
			char* line2 = getString(request,1);
			if(sameString(line2,"OFF",3)) {
				if(line2[3] == '\n' || line2[3] == '\0') {
					INFO("CONSUMER.processRequest","client deconnection");
					response = clientDisconnection(line+(7*sizeof(char)));
				} else {
					return "KO parsing error (off request)";
				}
			} else if(sameString(line2,"WAITING",7)) {
				if(line2[7] == '\n' || line2[7] == '\0') {
					INFO("CONSUMER.processRequest","client request for waiting request");
					response = waitingRequest(line+(7*sizeof(char)));
				} else {
					return "KO parsing error (waiting request)";
				}
			} else {
				ALERT("CONSUMER.processRequest","error line 2 >%s<",line2);
				return "KO parsing error (no client request matching)\n";
			}
			
			
		}
	} else if(sameString(line,"MASTER",6)) {
		if(line[6] == '\n' || line[6] == '\0') {// new master connection
			if(sameString(getString(request,1),"ON",2)) {// valid
				INFO("CONSUMER.processRequest","new master connection");
				response = newClientConnection(WORKING);
			} else {// error
				return "KO parsing error (master connection)\n";
			}
		} else {
			INFO("CONSUMER","step 3");
			for(int i=7;i<17;i++) {
				if(line[i] == '\n') {
					ALERT("CONSUMER.processRequest","error i=%d val=>%c</>%d< back=>%c</>%d< next=>%c</>%d<",i,line[i],line[i],line[i-1],line[i-1],line[i+1],line[i+1]);
					return "KO parsing error (master id size not incorrect)\n";
				}
			}
			INFO("CONSUMER","step 4");
			char* line2 = getString(request,1);
			if(sameString(line2,"TRY",3)) {// reserve request
				if(line2[3] == '\n' || line2[3] == '\0') {
					return reserveClient(line+(7*sizeof(char)));
				} else {
					return "KO parsing error (try request)\n";
				}
			} else if(sameString(line2,"ASK",3)) {// assign request
				INFO("CONSUMER.processRequest","ask start");
				if(line2[3] != '\n') {
						INFO("CONSUMER.processRequest","ASK START2");
					for(int i=4;i<14;i++) {
						if(line2[i] == '\0' || line2[i] == '\n') {
							ALERT("CONSUMER.processRequest","error i=%d val=>%c</>%d< back=>%c</>%d< next=>%c</>%d<",i,line2[i],line2[i],line2[i-1],line2[i-1],line2[i+1],line2[i+1]);
							return "KO parsing error (client id size not incorrect)\n";
						}
					}
					if(getSize(request) < 3) {
						ALERT("CONSUMER.processRequest","error request size %d",getSize(request));
						return "KO parsing error (ask request must have data)\n";
					}
					char* masterId = line+(7*sizeof(char));
					char* clientId = line2+(4*sizeof(char));
					LinkedListString* dataList = makeLinkedListString();

					for(int i=2;i<getSize(request);i++) {
						addString(dataList,getString(request,i));
					}
					//~ INFO("CONSUMER.processRequest","bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
					char* data = toCharRequest(dataList);
					//~ INFO("CONSUMER.processRequest","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
					deleteLinkedListString(dataList,false);
					return assignTaskToClient(masterId,clientId,data);
				} else {
					return "KO parsing error (ask request)\n";
				}
			} else if(sameString(line2,"RES",3)) {// result request
				if(line2[3] != '\n' || line2[3] != '\0') {
					for(int i=4;i<14;i++) {
						if(line[i] == '\0' || line[i] == '\n') {
							ALERT("CONSUMER.processRequest","error i=%d val=>%c</>%d< back=>%c</>%d< next=>%c</>%d<",i,line2[i],line2[i],line2[i-1],line2[i-1],line2[i+1],line2[i+1]);
							return "KO parsing error (client id size not incorrect)\n";
						}
					}
					if(getSize(request) < 3) {
						ALERT("CONSUMER.processRequest","error request size %d",getSize(request));
						return "KO parsing error (res request must have data)\n";
					}
					char* masterId = line+(7*sizeof(char));
					char* clientId = line2+(4*sizeof(char));
					LinkedListString* dataList = makeLinkedListString();
					for(int i=2;i<getSize(request);i++) {
						addString(dataList,getString(request,i));
					}
					char* data = toCharRequest(dataList);
					deleteLinkedListString(dataList,false);
					return responseForTask(masterId,clientId,data);
				} else {
					return "KO parsing error (response request)\n";
				}
			} else {
				return "KO parsing error (no master request matching)\n";
			}
		}
	} else {
		return "KO parsing error (no request matching)\n";
	}
	return response;
}

char* newClientConnection(ClientState newClientState) {
	Client* newClient = makeClient(genNextClientId());
	setClientState(newClient,newClientState);
	INFO("newClientConnection","new client [id=%s,state=%d]",newClient->id,newClient->state);
	addClient(listClient,newClient);
	return concat(concat("OK ",newClient->id),"\n");
}

char* clientDisconnection(char* clientId) {
	int pos = getClientIndex(listClient,clientId);
	if(pos >= 0) {
		removeClient(listClient,pos);
	} else {
		WARNING("CONSUMER.clientDisconnection","client not remove from list %s",clientId);
	}
	return "OK\n";
}

char* waitingRequest(char* clientId) {
	int clientPos = getClientIndex(listClient,clientId);
	if(clientPos < 0)
		return "KO unrecognize client id";
	Client* client = getClient(listClient,clientPos);
	LinkedListString* waitReq = dequeueRequest(client->waiting);
	if(waitReq == 0) {
		return "NOTHING\n\n";
	} else {
		return toCharRequest(waitReq);
	}
}

char* reserveClient(char* masterId) {
	int masterPos = getClientIndex(listClient,masterId);
	if(masterPos >= 0) {
		Client* reservedClient = reserveClientFromListClient(listClient,getClient(listClient,masterPos));
		if(reservedClient != 0) {
			return concat("OK ",reservedClient->id);
		} else {
			return "KO no client available";
		}
	}
	return "KO unrecognize master id";
}

char* assignTaskToClient(char* masterId, char* targetClientId, char* data) {
	int masterPos = getClientIndex(listClient,masterId);
	if(masterPos < 0)
		return "KO unrecognize master id";
	int clientPos = getClientIndex(listClient,targetClientId);
	if(clientPos < 0)
		return "KO unrecognize client id";
	Client* master = getClient(listClient,masterPos);
	Client* client = getClient(listClient,clientPos);
	if(client->usedBy != master) {
		return "KO bad master";
	}
	LinkedListString* req = makeLinkedListString();
	addString(req,concat("MASTER ",master->id));
	addString(req,concat("ASK ",client->id));
	addString(req,data);
	enqueueRequest(client->waiting,req);
	setClientState(client,WORKING);
	return concat("OK ",client->id);
}

char* responseForTask(char* masterId, char* clientId, char* data) {
	SUCCESS("responseForTask","start");
	int masterPos = getClientIndex(listClient,masterId);
	if(masterPos < 0)
		return "KO unrecognize master id";
	int clientPos = getClientIndex(listClient,clientId);
	if(clientPos < 0)
		return "KO unrecognize client id";
	Client* master = getClient(listClient,masterPos);
	Client* client = getClient(listClient,clientPos);
	if(client->usedBy != master) {
		return "KO bad master";
	}
	LinkedListString* req = makeLinkedListString();
	addString(req,concat("MASTER ",master->id));
	addString(req,concat("RES ",client->id));
	addString(req,data);
	enqueueRequest(master->waiting,req);
	SUCCESS("responseForTask","client FREE");
	setClientState(client,FREE);
	return concat("OK ",master->id);
}
