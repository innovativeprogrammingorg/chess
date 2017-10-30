#include "chat.h"

using namespace std;

Message::Message(string* user,string* message){
	this->user = user;
	this->message = message;
	this->date = time(NULL);
}

Message::~Message(){
	delete this->message;
	delete this->user;
}

Chat::Chat(){
	this->messages = new vector<Message*>();
	this->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
	this->users = new vector<int>();
}

Chat::~Chat(){
	delete this->messages;
	pthread_mutex_destroy(this->lock);
	delete this->users;
}

void Chat::add(Message* m){
	pthread_mutex_lock(this->lock);
	if(this->messages->size() >= MAX_MESSAGES){
		this->messages->erase(this->messages->begin());
	}
	this->messages->push_back(m);
	pthread_mutex_unlock(this->lock);
}

void Chat::add(string* user, string* msg){
	this->add(new Message(user,msg));
}

void Chat::add(string user, string msg){
	this->add(new string(user),new string(msg));
}

string Chat::to_string(char sep){
	pthread_mutex_lock(this->lock);
	string out = "";
	for(int i = 0;i<this->messages->size();i++){
		if(i!=0){ 
			out += sep; 
		}
		Message* m  = this->messages->at(i);
		out += *m->user;
		out += sep;
		out += *m->message;
	}
	pthread_mutex_unlock(this->lock);
	return out;
}

int64_t Chat::indexOf(int sd){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i) == sd){
			pthread_mutex_unlock(this->lock);
			return i;
		}
	}
	pthread_mutex_unlock(this->lock);
	return -1;
}

void Chat::connect(int sd){
	
	if(this->indexOf(sd) != -1){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->users->push_back(sd);
	pthread_mutex_unlock(this->lock);
}

void Chat::disconnect(int sd){
	int64_t index = this->indexOf(sd);
	pthread_mutex_lock(this->lock);
	if(index == -1){
		this->users->erase(this->users->begin() + index);
	}
	pthread_mutex_unlock(this->lock);
}

void Chat::broadcast(Frame* frame, int sd){
	pthread_mutex_lock(this->lock);
	if(sd != 0){
		frame->send(sd);
		pthread_mutex_unlock(this->lock);
		return;
	}
	for(int64_t i = 0;i<this->users->size();i++){
		frame->send(this->users->at(i));
	}
	pthread_mutex_unlock(this->lock);
}
