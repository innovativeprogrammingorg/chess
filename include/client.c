#include "client.h"


Client new_client(int fd,char* ip,int port){
	Client out = (Client)malloc(sizeof(struct client));
	out->fd = fd;
	out->ip = ip;
	out->port = port;
	out->cookies = NULL;
	out->last_active = time(NULL);
	return out;
}

Client find_client_by_ip(Vector clients,char* ip){
	size_t length = vector_length(clients);
	Client c = NULL;
	uint_fast64_t i;
	for(i = 0;i<length;i++){
		c = (Client)vector_get(clients,i);
		if(strcompare(ip,c->ip)){
			return c;
		}
	}
	return NULL;
}