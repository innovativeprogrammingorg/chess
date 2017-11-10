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
	this->users = new vector<User*>();
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

string Chat::to_string(){
	pthread_mutex_lock(this->lock);
	string out = "";
	for(int i = 0;i<this->messages->size();i++){
		if(i!=0){ 
			out += DATA_SEP; 
		}
		Message* m  = this->messages->at(i);
		out += *m->user;
		out += DATA_SEP;
		out += *m->message;
	}
	pthread_mutex_unlock(this->lock);
	if(out.compare("") == 0){
		out = "none";
	}
	return out;
}

int64_t Chat::indexOf(string name){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i)->username->compare(name) == 0){
			pthread_mutex_unlock(this->lock);
			return i;
		}
	}
	pthread_mutex_unlock(this->lock);
	return -1;
}
void Chat::connect(User* user){
	if(this->indexOf(*user->username) != -1){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->users->push_back(user);
	pthread_mutex_unlock(this->lock);
}

void Chat::connect(string name){
	this->connect(new User(name));	
}

void Chat::disconnect(string name){
	int64_t index = this->indexOf(name);
	pthread_mutex_lock(this->lock);
	if(index != -1){
		this->users->erase(this->users->begin() + index);
	}
	pthread_mutex_unlock(this->lock);
}

void Chat::disconnect(User* user){
	this->disconnect(*user->username);
}

void Chat::broadcast(Frame* frame){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		frame->send(this->users->at(i)->sd());
	}
	pthread_mutex_unlock(this->lock);
}


string Chat::get_last(){
	string out = "";
	pthread_mutex_lock(this->lock);
	Message* m = this->messages->back();
	out = *m->user;
	out += DATA_SEP;
	out += *m->message;
	pthread_mutex_unlock(this->lock);
	return out;
}

void Chat::send_last(){
	string msg = "CHAT";
	msg += COMMAND;
	msg += this->get_last();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void Chat::send_all(int sd){
	string msg = "CHAT_ALL";
	msg += COMMAND;
	msg += this->to_string();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

