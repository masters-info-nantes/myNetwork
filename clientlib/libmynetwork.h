#ifndef __LIBMYNETWORK__
#define __LIBMYNETWORK__

typedef myNetwork_client_id int;

void myNetworkStartDaemon(char* programPath);

void myNetworkSendTaskResponse(char* res);

myNetwork_client_id myNetworkReserveClient();

#endif /* __LIBMYNETWORK__ */
