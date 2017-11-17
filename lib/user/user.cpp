#include "user/user.h"

using namespace std;

User::User(string name){
	this->username = new string(name);
	this->side = 'w';
}

User::User(string name,char side){
	this->username = new string(name);
	this->side = side;
}

User::~User(){
	delete this->username;
}

int User::sd(){
	Client* c = Client::find_client(*this->username);
	if(c==nullptr){
		return -1;
	}
	return c->sd;
}

int User::sd(string key){
	return User_Manager::UM->lookup(this->username,key);
}

int User::sd(int64_t key){
	return User_Manager::UM->lookup(this->username,key);
}

int User::sd(UEKey key){
	if(key == nullptr){
		return this->sd();
	}
	return User_Manager::UM->lookup(this->username,key);
}
