#include "server.h"

int create_socket(){
	int master_socket;
	int opt = TRUE;
	if((master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	return master_socket;
}

struct sockaddr_in init_connection(int master_socket){
	struct sockaddr_in address;
	int port = 5959;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	printf("Listener on port %d \n", port);
	if (listen(master_socket, MAX_CONNECTIONS) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	return address;
}