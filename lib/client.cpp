#include "client.h"

using namespace std;

Client::Client(uint32_t addr,in_port_t port,uint32_t sd){
	this->addr = addr;
	this->port = port;
	this->sd = sd;
}

