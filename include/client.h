#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>
#include <time.h>
#include <map>
#include <pthread.h>
#include "str.h"

using namespace std;
class Client{
	public:
		static vector<Client*>* clients;
		static pthread_mutex_t* check_lock;
		string ip;
		int port;
		int sd;
		time_t last_active;
		bool handshaked;
		string* username;
		pthread_mutex_t* lock;
		/**
		 * Compares this client to another one
		 * @param  c Another client
		 * @return   Whether or not the clients are equal
		 */
		bool equals(Client* c);
		Client(int fd,string ip,int port);
		static void init();
		static Client* find_client_by_ip(string ip);
		static Client* find_client(string username);
		static void add_client(Client* c);
		static void drop_client(Client* c);
		static void drop_client(int i);
		static Client* client_at(int i);

};



#ifndef CLIENT_TIMEOUT
#define CLIENT_TIMEOUT 1 * 60 * 60
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#endif