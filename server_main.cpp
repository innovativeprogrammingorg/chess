#include "./include/server.h"

Vector clients = NULL;
char * last_input = NULL;
Client active_client = NULL;

int main(){
	int opt = TRUE;
	int master_socket , addrlen , new_socket, activity, i , valread , sd;
	int max_sd;
	int port = 8989;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];  
	fd_set readfds;
	int cons = 0;
	Client c;
	//start_PHP_CGI(8777);
	signal(SIGSEGV,segfault_catch);
	signal(SIGINT,kill_all);

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
	if (listen(master_socket, 200) < 0){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	/*puts("Listener created successfully");*/
	
	addrlen = sizeof(address);	 
	while(TRUE) {
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
		 
		printf("The number of connections is %i\n",cons);
		for (i = 0 ; i < cons ; i++) {
			sd = ((Client)vector_get(clients,i))->fd;
			if(sd > 0)
				FD_SET(sd,&readfds);
			 
			if(sd > max_sd)
				max_sd = sd;
		}
  		/*printf("The max process id is %i\n",max_sd);*/
		activity = select(max_sd + 1 , &readfds , NULL , NULL , NULL);
		if ((activity < 0) && (errno!=EINTR)) {
			printf("select error");
		}
		if (FD_ISSET(master_socket, &readfds)){
			if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0){
				perror("accept");
				exit(EXIT_FAILURE);
			}
			vector_push(&clients,new_client(new_socket,inet_ntoa(address.sin_addr),ntohs(address.sin_port)));	
			cons++;
			continue;
		}
		for (i = 0; i < cons; i++){
			/*printf("Checking on user %i\n",i);*/
			sd = ((Client)vector_get(clients,i))->fd;
			/*printf("Retrieved the socket of that user\n");*/
			if (FD_ISSET( sd , &readfds)){
				/*printf("Checking to see what action occured...\n");*/
				if ((valread = read( sd , buffer, BUFFER_SIZE - 1)) == 0){
					/*printf("Removing the User from the list\n");*/
					vector_pop(&clients,i);
					/*printf("Removed Successfully!\n");*/
					cons--;
					close(sd);
					i = 0;
				}else{
					
					active_client = (Client)vector_get(clients,i);
					active_client->last_active = time(NULL);
					/*printf("The socket number for this message is %i\n",sd);*/
					buffer[valread] = '\0';
					last_input = buffer;
					create_new_thread(active_client,buffer);
					//write(0, buffer, strlen(buffer));
				}
			}else{
				c = (Client)vector_get(clients,i);
				if(time(NULL) - c->last_active > CLIENT_TIMEOUT){
					vector_pop(&clients,i);
					cons--;
					close(sd);
					i = 0;
				}
			}
		}
		memset(&buffer,0,BUFFER_SIZE);
	}
	return EXIT_SUCCESS;
} 



Vector get_current_clients(){
	return clients;
}

Client get_active_client(){
	return active_client;
}

void segfault_catch(int signum){
	puts("Server encountered a segmentation fault");
	printf("The last received is : \n%s \n",last_input);
	/*printf("Showing the breaks... \n %s\n ",str_replace("\n","<cl>",str_replace("\r\n","<clrf>",last_input)));*/
	execl("./server","./server",NULL);
	//exit(EXIT_FAILURE);
}

void kill_all(int signum){
	vector_clean(clients);
	puts("Killed\n");
	exit(EXIT_SUCCESS);
}