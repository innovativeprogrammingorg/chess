#include "CGI.h"

void start_PHP_CGI(int port){
	if(!fork()){
		char* args[3] = {"-b",concat("127.0.0.1:",itoa(port),FALSE),NULL};
		execv("/usr/bin/php-cgi",args);
		exit(EXIT_SUCCESS);
	}
}

uint64_t* run_CGI(char* message,char* script,char ** data){
	int opt = TRUE;
	int master_socket , addrlen , new_socket, valread;
	int max_sd;
	int port = 8777;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];  
	fd_set readfds;
	char * out = NULL;
	Client c = NULL;
	uint64_t* size = (uint64_t*)malloc(sizeof(uint64_t));

	if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( port );
	  
	
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	/*printf("Listener on port %d \n", port);*/	
	if (listen(master_socket, 2) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	/*puts("Listener created successfully");*/
	addrlen = sizeof(address);	 
	while(TRUE){
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
		if(c){
			if(c->fd > 0)
				FD_SET(c->fd,&readfds);
		 
			if(c->fd > max_sd)
				max_sd = c->fd;
		}
		
  		/*printf("The max process id is %i\n",max_sd);*/
		if ((select(max_sd + 1 , &readfds , NULL , NULL , NULL) < 0) && (errno!=EINTR)) {
			printf("select error");
		}
		if (FD_ISSET(master_socket, &readfds)){
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<=0){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			c = new_client(new_socket,inet_ntoa(address.sin_addr),ntohs(address.sin_port));\
			write(new_socket,message,strlen(message));	
			continue;
		}
		if(!c){
			continue;
		}
		/*printf("Retrieved the socket of that user\n");*/
		if (FD_ISSET( c->fd , &readfds)){
			/*printf("Checking to see what action occured...\n");*/
			if ((valread = read( c->fd , buffer, BUFFER_SIZE - 1)) == 0){
				close(c->fd);
				free(c);
				*data = out;
				return size;
			}else{
				*size = *size + valread;
				/*printf("The socket number for this message is %i\n",c->fd);*/
				buffer[valread] = '\0';
				out = concat(out,buffer,(out)? FIRST : FALSE);
			}
		}
		memset(&buffer,0,BUFFER_SIZE);
	}
	return NULL;
} 

Response CGI_response(Map m){
	char* body = (char*)map_value_at(m,"PHP_CGI");
	size_t size = *((size_t *)map_value_at(m,"SIZE"));
	return new_response(NULL,body,size);
}