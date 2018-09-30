#include "chat.h"

using namespace std;

message::message(const string& user,const string& msg) :
	user(user),
	msg(msg),
	date(time(NULL)){}

message::~message(){}

chat::chat()
{
	pthread_mutex_init(&this->lock,NULL);
}


chat::~chat()
{
	pthread_mutex_destroy(&this->lock);
}

void chat::add(const message& m)
{
	pthread_mutex_lock(&this->lock);
	if(this->messages.size() >= MAX_MESSAGES){
		this->messages.erase(this->messages.begin());
	}
	this->messages.push_back(m);
	pthread_mutex_unlock(&this->lock);
}

void chat::add(string* user, string* msg)
{
	this->add(*user,*msg);
}

void chat::add(const string& user, const string& msg){
	this->add(user,msg);
}

string chat::to_string(){
	pthread_mutex_lock(&this->lock);
	string out = "";
	for(int i = 0;i < this->messages.size();i++){
		if(i!=0){ 
			out += DATA_SEP; 
		}
		message m  = this->messages.at(i);
		out += m.user;
		out += DATA_SEP;
		out += m.message;
	}
	pthread_mutex_unlock(&this->lock);
	if(out.size() == 0){
		out = "none";
	}
	return out;
}

int64_t chat::index_of(const string& name)
{
	pthread_mutex_lock(&this->lock);
	int64_t i = -1;
	for(auto it = this->users.begin();it != this->users.end();it++){
		if(it->first.username.compare(name) == 0){
			i = it - this->users.begin();
			break;
		}
	}
	pthread_mutex_unlock(&this->lock);
	return i;
}
void chat::connect(const user& u,const UEKey key){
	if(this->index_of(u.username) != -1){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->users.emplace(u,key);
	pthread_mutex_unlock(this->lock);
}

void chat::connect(const string& name,const UEKey key)
{
	this->connect(user(name),key);	
}

void chat::disconnect(const string& name)
{
	user tmp(name);
	pthread_mutex_lock(&this->lock);
	try{
		this->users.erase(tmp);
	}catch(const out_of_range& oor){
		cout<<"Chat::disconnect:Error disconnecting user"<<endl;
	}
	pthread_mutex_unlock(&this->lock);
}

void chat::disconnect(const user& u)
{
	this->disconnect(u.username);
}

void chat::broadcast(const frame& f)
{
	pthread_mutex_lock(&this->lock);
	if(this->users.empty()){
		return;
	}
	for(auto it = this->users.begin();it != this->users->end();it++){
		frame.send(it->first.sd(it->second));
	}
	pthread_mutex_unlock(&this->lock);
}


string chat::get_last()
{
	string out = "";
	pthread_mutex_lock(&this->lock);
	const message m = this->messages->back();
	out = m.user;
	out += DATA_SEP;
	out += m.message;
	pthread_mutex_unlock(&this->lock);
	return out;
}

void chat::send_last()
{
	string msg = "CHAT";
	msg += COMMAND;
	msg += this->get_last();
	frame f(1,0,0,0,0,TEXT);
	f.add((uint8_t*)msg.c_str());
	this->broadcast(f);
}

void chat::send_all(const int sd)
{
	string msg = "CHAT_ALL";
	msg += COMMAND;
	msg += this->to_string();
	frame f(1,0,0,0,0,TEXT);
	f.add((uint8_t*)msg.c_str());
	f.send(sd);
}

