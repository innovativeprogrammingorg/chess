#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>
#include <time.h>
#include <pthread.h>
#include "str.h"
#include "user/user_manager.h"

using namespace std;
class client{
	public:
		static vector<client*> clients;
		static pthread_mutex_t* check_lock;
		string ip;
		int port;
		int sd;
		time_t last_active;
		bool handshaked;
		string username;
		pthread_mutex_t lock;
		/**
		 * Compares this client to another one
		 * @param  c Another client
		 * @return   Whether or not the clients are equal
		 */
		bool equals(client* c);
		client(int fd,const string& ip,int port);
		virtual ~client();
		static void init();
		static client* find_client_by_ip(const string& ip);
		static client* find_client(int sd);
		static client* find_client(const string& username);
		static void add_client(client* c);
		static void add_client(int fd,const string& ip,int port);
		static void drop_client(client* c);
		static void drop_client(int i);
		static client* client_at(int i);

};



#ifndef CLIENT_TIMEOUT
#define CLIENT_TIMEOUT 1 * 60 * 60
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#endif