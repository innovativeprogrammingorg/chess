#ifndef _SERVER_SERVER_H_
#define _SERVER_SERVER_H_

#include <string.h>
#include <sys/epoll.h>
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <resolv.h>
#include <sys/epoll.h>
#include <signal.h>
#include <pthread.h>
#include "server/concurrency.h"
#include "server/client.h"

#define BUFFER_SIZE 200000
#define MAX_EVENTS 128 

class Server{
	private:
		int port;
		char buffer[BUFFER_SIZE];
		struct epoll_event* events;
		int master_socket;
		int efd;
		struct epoll_event event;
		struct sockaddr_in address;
	public:
		Client* active_client;
		Server(int port);
		void run() __attribute__((noreturn));
};

#endif