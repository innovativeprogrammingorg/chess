#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <unistd.h>
#include <stdlib.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include "cookies.h"
#include "str.h"

class Client{
	public:
		static vector<Client*>* clients;
		string ip;
		int port;
		int fd;
		map<string,Cookie*> cookies;
		time_t last_active;

		bool equals(Client* c);
		Client(int fd,char* ip,int port);
		static Client* find_client_by_ip(string ip);
		static void add_client(Client* c);
		static void drop_client(Client* c);
		static void drop_client(int i);

};



#ifndef CLIENT_TIMEOUT
#define CLIENT_TIMEOUT 600
#endif


#endif