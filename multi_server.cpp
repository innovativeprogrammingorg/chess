#include "server.h"

using namespace std;

Client* active_client = nullptr;

static int make_socket_non_blocking (int sfd){
	int flags = fcntl(sfd, F_GETFL, 0);
	if(flags == -1){
		perror("fcntl");
		return -1;
	}
	flags |= O_NONBLOCK;

	if (fcntl(sfd, F_SETFL, flags) == -1){
		perror("fcntl");
		return -1;
	}

	return 0;
}

int main(){

	srand(time(NULL));

	int opt = TRUE;
	int master_socket , addrlen , new_socket, sd;
	int port = 8989;
	size_t valread;
	struct sockaddr_in address;
	char buffer[BUFFER_SIZE];  
	Client::init();
	int n;
	struct epoll_event event;// = (struct epoll_event*)calloc(sizeof(struct epoll_event),1);
	struct epoll_event* events = nullptr;
	int efd;
	memset(&event,0,sizeof(struct epoll_event));
	//signal(SIGSEGV,segfault_catch);
	signal(SIGINT,kill_all);
	signal(SIGPIPE,SIG_IGN);

	if( (master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		cerr<<"socket failed"<<endl;
		abort();
	}
 
	if(setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		cerr<<"setsockopt"<<endl;
		abort();
	}
	
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	
	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0) {
		cerr<<"bind failed"<<endl;
		abort();
	}
	if(make_socket_non_blocking (master_socket) == -1){
		cerr<<"non block"<<endl;
		abort();
	}
	/*printf("Listener on port %d \n", port);*/	
	if (listen(master_socket, SOMAXCONN) < 0){
		cerr<<"listen"<<endl;
		abort();
	}
	efd = epoll_create1(0);
	if (efd == -1){
		cerr<<"epoll_create"<<endl;
		abort();
	}
	event.data.fd = master_socket;
	event.events = EPOLLIN | EPOLLET;
	if(epoll_ctl (efd, EPOLL_CTL_ADD, master_socket, &event) == -1){
		cerr<<"epoll_ctl"<<endl;
		abort();
	}

	events = (struct epoll_event*) calloc(sizeof(struct epoll_event),MAX_EVENTS);

	/*puts("Listener created successfully");*/
	
	addrlen = sizeof(address);	 
	for(;;) {
		
		n = epoll_wait(efd, events,MAX_EVENTS, -1);//select(max_sd + 1 , &readfds , NULL , NULL , NULL);
		if((n < 0) && (errno!=EINTR)) {
			cout<<"select error"<<endl;
		}

		for(int i = 0; i < n; i++){
			if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))){
						/* An error has occured on this fd */
				cerr<<"epoll error"<<endl;
				active_client = Client::find_client(events[i].data.fd);
				if(active_client != nullptr){
					Client::drop_client(active_client);
				}
				
				close(events[i].data.fd);
				continue;
			}else if (master_socket == events[i].data.fd){
						/* We have a notification on the listening socket, which
							 means one or more incoming connections. */
				for(;;){

					new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen);
					if (new_socket == -1){
						if (!((errno == EAGAIN) ||(errno == EWOULDBLOCK))) {
							cout<<"accept"<<endl;
						}
						break;
					}

					Client::add_client(new_socket,inet_ntoa(address.sin_addr),ntohs(address.sin_port));
				
					
				/* Make the incoming socket non-blocking and add it to the
					 list of fds to monitor. */
					if (make_socket_non_blocking (new_socket) == -1){
						abort();
					}
					event.data.fd = new_socket;
					event.events = EPOLLIN | EPOLLET;
					if (epoll_ctl (efd, EPOLL_CTL_ADD, new_socket, &event) == -1){
						cerr<<"epoll_ctl"<<endl;
						abort();
					}
				}
				continue;

			}else{
							
				active_client = Client::find_client(events[i].data.fd);
				valread = read(events[i].data.fd, buffer, BUFFER_SIZE - 1);
				if (valread <= 0){
					/* End of file. The remote has closed the
						 connection. */
					cout<<"Removing the User from the list"<<endl;
						close(events[i].data.fd);
						Client::drop_client(active_client);
						break;
				}
				active_client->last_active = time(NULL);
				buffer[valread] = '\0';
				if(active_client->handshaked){
				//cout<<"Received a message from the client"<<endl;
				//cout<<"The given size is "<<valread<<endl;
					data_frame(active_client,buffer,valread);
				}else{
				//write(0, buffer, strlen(buffer));
					handshake(active_client,buffer,valread);
				}
				memset(&buffer,0,BUFFER_SIZE);
			}
		}
	}
	return EXIT_SUCCESS;
} 

Client* get_active_client(){
	return active_client;
}

void segfault_catch(int signum){
	cout<<"Server encountered a segmentation fault"<<endl;
	/*printf("Showing the breaks... \n %s\n ",str_replace("\n","<cl>",str_replace("\r\n","<clrf>",last_input)));*/
	execl("./server","./server",NULL);
	//exit(EXIT_FAILURE);
}

void kill_all(int signum){
	delete Client::clients;
	delete Control::lookup_table;
	cout<<"Killed"<<endl;
	exit(EXIT_SUCCESS);
}