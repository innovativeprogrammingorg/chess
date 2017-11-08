#include "./include/server.h"
#include "./include/WebSocket/handshake.h"

using namespace std;

char* last_input = NULL;
Client* active_client = nullptr;

int main(){

	srand(time(NULL));
	int opt = TRUE;
	int master_socket , addrlen , new_socket, activity, valread , sd;
	int max_sd;
	int port = 8989;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];  
	fd_set readfds;
	Client* c;
	Client::init();
	//signal(SIGSEGV,segfault_catch);
	signal(SIGINT,kill_all);
	signal(SIGPIPE,SIG_IGN);

	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
 
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
 	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
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
	for(;;) {
		FD_ZERO(&readfds);
		FD_SET(master_socket, &readfds);
		max_sd = master_socket;
		 
		printf("The number of connections is %lu\n",Client::clients->size());
		for (auto it = Client::clients->begin();(it != Client::clients->end()) && !Client::clients->empty(); it++) {
			sd = ((Client*) *it)->sd;
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
			Client::add_client(new Client(new_socket,inet_ntoa(address.sin_addr),ntohs(address.sin_port)));
			continue;
		}
		for(auto it = Client::clients->begin();(it != Client::clients->end()) && !Client::clients->empty(); it++){
			active_client = *it;
			
			sd = active_client->sd;
			/*printf("Retrieved the socket of that user\n");*/
			if (FD_ISSET( sd , &readfds)){
				/*printf("Checking to see what action occured...\n");*/
				if ((valread = read( sd , buffer, BUFFER_SIZE - 1)) <= 0){
					printf("Removing the User from the list\n");
					Client::drop_client(active_client);
					/*printf("Removed Successfully!\n");*/
					close(sd);
					it = Client::clients->begin();
				}else{
					
					active_client->last_active = time(NULL);
					/*printf("The socket number for this message is %i\n",sd);*/
					
					buffer[valread] = '\0';
					last_input = buffer;
					
					if(active_client->handshaked){
						cout<<"Received a message from the client"<<endl;
						//cout<<"The given size is "<<valread<<endl;
						data_frame(active_client,buffer,valread);
					}else{
						//write(0, buffer, strlen(buffer));
						handshake(active_client,buffer);
					}
					
					
				}
			}
		
		}
		memset(&buffer,0,BUFFER_SIZE);
	}
	return EXIT_SUCCESS;
} 


Client* get_active_client(){
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
	delete Client::clients;
	puts("Killed\n");
	exit(EXIT_SUCCESS);
}