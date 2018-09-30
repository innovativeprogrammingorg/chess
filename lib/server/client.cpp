#include "client.h"

using namespace std;


vector<Client*>* Client::clients = new vector<Client*>();
pthread_mutex_t* Client::check_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

Client::Client(int sd,string ip,int port){
	this->sd = sd;
	this->ip = ip;
	this->port = port;
	this->last_active = time(NULL);
	this->handshaked = false;
	this->username = nullptr;
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
}
Client::~Client(){
	if(this->username != nullptr){
		delete this->username;
	}
	pthread_mutex_unlock(this->lock);
	pthread_mutex_destroy(this->lock);
}

bool Client::equals(Client* c){
	return this->sd == c->sd && this->ip.compare(c->ip) == 0 && this->port == c->port;
}

Client* Client::find_client(int sd){
	pthread_mutex_lock(Client::check_lock);
	size_t length = Client::clients->size();
	Client* c = nullptr;
	for(uint_fast64_t i = 0;i<length;i++){
		c = Client::clients->at(i);
		if(c->sd == sd){
			pthread_mutex_unlock(Client::check_lock);
			return c;
		}
	}
	pthread_mutex_unlock(Client::check_lock);
	return nullptr;
}

Client* Client::find_client(string username){
	pthread_mutex_lock(Client::check_lock);
	if(Client::clients->size() == 0){
		pthread_mutex_unlock(Client::check_lock);
		return nullptr;
	}	

	for(auto it = Client::clients->begin();it != Client::clients->end();it++){
		if(username.compare(*(*it)->username) == 0){
			pthread_mutex_unlock(Client::check_lock);
			return *it;
		}
	}
	pthread_mutex_unlock(Client::check_lock);
	return nullptr;
}
Client* Client::find_client_by_ip(string ip){
	pthread_mutex_lock(Client::check_lock);
	size_t length = Client::clients->size();
	Client* c = nullptr;
	for(uint_fast64_t i = 0;i<length;i++){
		c = Client::clients->at(i);
		if(ip.compare(c->ip) == 0){
			pthread_mutex_unlock(Client::check_lock);
			return c;
		}
	}
	pthread_mutex_unlock(Client::check_lock);
	return nullptr;
}

void Client::add_client(Client* c){
	pthread_mutex_lock(Client::check_lock);
	Client::clients->push_back(c);
	pthread_mutex_unlock(Client::check_lock);
}

void Client::add_client(int fd,string ip,int port){
	Client::add_client(new Client(fd,ip,port));
}

void Client::drop_client(Client* c){
	pthread_mutex_lock(Client::check_lock);
	for(int i = 0;i<Client::clients->size();i++){
		if(Client::clients->at(i)->equals(c)){
			Client::clients->erase(Client::clients->begin() + i);
			pthread_mutex_unlock(Client::check_lock);
			return;
		}
	}
	pthread_mutex_unlock(Client::check_lock);
}

void Client::drop_client(int i){
	pthread_mutex_lock(Client::check_lock);
	try{
		Client* c = Client::clients->at(i);

		User_Manager::UM->disconnect(c->username,c->sd);
		c = nullptr;
		Client::clients->erase(Client::clients->begin() + i);
	}catch(const out_of_range& oor){
		cout<<"Error: Failed to delete client"<<endl;
	}
	
	pthread_mutex_unlock(Client::check_lock);
}

Client* Client::client_at(int i){
	return Client::clients->at(i);
}


static void* check_clients_activity(void* ign){
	struct timespec sleeper;
	time_t t;
	sleeper.tv_sec = 60;
	sleeper.tv_nsec = 0;
	for(;;){
		nanosleep(&sleeper,NULL);
		if(Client::clients->empty()){
			continue;
		}
		t = time(NULL);
		for(auto it = Client::clients->begin();it != Client::clients->end();it++){
			Client* c = *it;
			if(t - c->last_active > CLIENT_TIMEOUT){
				close(c->sd);
				Client::drop_client(c);
			}
		}
	}
	return NULL;
}

void Client::init(){
	pthread_mutex_init(Client::check_lock,NULL);
	pthread_t checker;// = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(&checker,NULL,check_clients_activity,NULL);
}