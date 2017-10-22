#ifndef CLIENT_H
#define CLIENT_H

#include "game.h"
#include "user.h"
#include <netinet/in.h>



class Client{
	public:
		uint32_t addr;
		in_port_t port;
		uint32_t sd;
		Client(uint32_t addr,in_port_t port,uint32_t sd);
};

#endif