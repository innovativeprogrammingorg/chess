#include "client.h"

using namespace std;


vector<client*>* client::clients = new vector<client*>();
pthread_mutex_t* client::check_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

client::client(int sd,const string& ip,int port) : ip(ip),port(port), sd(sd),last_active(time(NULL)),handshaked(false)
{
	pthread_mutex_init(&this->lock,NULL);
}
client::~client(){
	pthread_mutex_destroy(&this->lock);
}

bool client::equals(client* c){
	return this->sd == c->sd && this->ip.compare(c->ip) == 0 && this->port == c->port;
}

client* client::find_client(int sd){
	pthread_mutex_lock(client::check_lock);
	size_t length = client::clients->size();
	client* c = nullptr;
	for(uint_fast64_t i = 0;i<length;i++){
		c = client::clients->at(i);
		if(c->sd == sd){
			pthread_mutex_unlock(client::check_lock);
			return c;
		}
	}
	pthread_mutex_unlock(client::check_lock);
	return nullptr;
}

client* client::find_client(const string& username){
	pthread_mutex_lock(client::check_lock);
	if(client::clients->size() == 0){
		pthread_mutex_unlock(client::check_lock);
		return nullptr;
	}	

	for(auto it = client::clients->begin();it != client::clients->end();it++){
		if(username.compare((*it)->username) == 0){
			pthread_mutex_unlock(client::check_lock);
			return *it;
		}
	}
	pthread_mutex_unlock(client::check_lock);
	return nullptr;
}
client* client::find_client_by_ip(const string& ip){
	pthread_mutex_lock(client::check_lock);
	size_t length = client::clients->size();
	client* c = nullptr;
	for(uint_fast64_t i = 0;i<length;i++){
		c = client::clients->at(i);
		if(ip.compare(c->ip) == 0){
			pthread_mutex_unlock(client::check_lock);
			return c;
		}
	}
	pthread_mutex_unlock(client::check_lock);
	return nullptr;
}

void client::add_client(client* c){
	pthread_mutex_lock(client::check_lock);
	client::clients->push_back(c);
	pthread_mutex_unlock(client::check_lock);
}

void client::add_client(int fd,const string& ip,int port){
	client::add_client(new client(fd,ip,port));
}

void client::drop_client(client* c){
	pthread_mutex_lock(client::check_lock);
	for(int i = 0;i<client::clients->size();i++){
		if(client::clients->at(i)->equals(c)){
			client::clients->erase(client::clients->begin() + i);
			pthread_mutex_unlock(client::check_lock);
			return;
		}
	}
	pthread_mutex_unlock(client::check_lock);
}

void client::drop_client(int i){
	pthread_mutex_lock(client::check_lock);
	try{
		client* c = client::clients->at(i);

		User_Manager::UM->disconnect(c->username,c->sd);
		c = nullptr;
		client::clients->erase(client::clients->begin() + i);
	}catch(const out_of_range& oor){
		cout<<"Error: Failed to delete client"<<endl;
	}
	
	pthread_mutex_unlock(client::check_lock);
}

client* client::client_at(int i){
	return client::clients->at(i);
}


static void* check_clients_activity(void* ign){
	struct timespec sleeper;
	time_t t;
	sleeper.tv_sec = 60;
	sleeper.tv_nsec = 0;
	for(;;){
		nanosleep(&sleeper,NULL);
		if(client::clients->empty()){
			continue;
		}
		t = time(NULL);
		for(auto it = client::clients->begin();it != client::clients->end();it++){
			client* c = *it;
			if(t - c->last_active > CLIENT_TIMEOUT){
				close(c->sd);
				client::drop_client(c);
			}
		}
	}
	return NULL;
}

void client::init(){
	pthread_mutex_init(client::check_lock,NULL);
	pthread_t checker;// = (pthread_t*)malloc(sizeof(pthread_t));
	pthread_create(&checker,NULL,check_clients_activity,NULL);
}