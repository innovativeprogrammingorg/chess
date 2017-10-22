#include "client.h"

using namespace std;


vector<Client*>* Client::clients = new vector<Client*>();

Client::Client(int fd,string ip,int port){
	this->fd = fd;
	this->ip = ip;
	this->port = port;
	this->cookies = nullptr;
	this->last_active = time(NULL);
}

bool Client::equals(Client* c){
	return this->fd == c->fd && this->ip.compare(c->ip) == 0 && this->port = c->port;
}

Client* Client::find_client_by_ip(string ip){
	size_t length = Client::clients->size();
	Client* c = nullptr;
	for(uint_fast64_t i = 0;i<length;i++){
		c = Client::clients->at(i);
		if(ip.compare(c->ip) == 0){
			return c;
		}
	}
	return nullptr;
}

void Client::add_client(Client* c){
	Client::clients->push_back(c);
}

void Client::drop_client(Client* c){
	for(int i = 0;i<Client::clients->size();i++){
		if(Client::clients->at(i)->equals(c)){
			Client::clients->erase(Client::clients->begin() + i);
			return;
		}
	}
}

void Client::drop_client(int i){
	Client::clients->erase(Client::clients->begin() + i);
}