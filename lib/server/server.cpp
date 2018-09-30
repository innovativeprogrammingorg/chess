#include "server/server.h"
using namespace std;

static int make_socket_non_blocking(int sfd){
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


Server::Server(int port){
	int opt = TRUE;
	struct epoll_event event;
	memset(&this->event,0,sizeof(struct epoll_event));
	this->port = port;
	this->events = nullptr;
	if((this->master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		cerr<<"socket failed"<<endl;
		abort();
	}
	if(setsockopt(this->master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 ){
		cerr<<"setsockopt"<<endl;
		abort();
	}
	this->address.sin_family = AF_INET;
	this->address.sin_addr.s_addr = INADDR_ANY;
	this->address.sin_port = htons(this->port);
	if (bind(this->master_socket, (struct sockaddr *)&this->address, sizeof(this->address))<0) {
		cerr<<"bind failed"<<endl;
		abort();
	}
	if(make_socket_non_blocking(this->master_socket) == -1){
		cerr<<"non block"<<endl;
		abort();
	}
	if (listen(this->master_socket, SOMAXCONN) < 0){
		cerr<<"listen"<<endl;
		abort();
	}

	this->efd = epoll_create1(0);
	if (this->efd == -1){
		cerr<<"epoll_create"<<endl;
		abort();
	}
	this->event.data.fd = master_socket;
	this->event.events = EPOLLIN | EPOLLET;
	if(epoll_ctl(this->efd, EPOLL_CTL_ADD, this->master_socket, &event) == -1){
		cerr<<"epoll_ctl"<<endl;
		abort();
	}
	this->events = (struct epoll_event*) calloc(sizeof(struct epoll_event),MAX_EVENTS);
}
void Server::run(){
	int addrlen , new_socket, sd;
	size_t valread;
	int n;

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
}