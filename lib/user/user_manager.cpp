#include "user/user_manager.h"
using namespace std;

User_Manager* User_Manager::UM = new User_Manager();

User_Manager::User_Manager(){
	this->data = new map<string*,User_Entry*,strptrcomp>();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
}

User_Manager::~User_Manager(){
	delete this->data;
	pthread_mutex_unlock(this->lock);
	pthread_mutex_destroy(this->lock);
}

void User_Manager::login(string username){
	//cout<<"Loggin in user "<<username<<" into the user management system"<<endl;
	pthread_mutex_lock(this->lock);
	this->data->insert(pair<string*,User_Entry*>(new string(username),new User_Entry()));
	pthread_mutex_unlock(this->lock);
}

void User_Manager::logout(string username){
	if(this->data->size()==0){
		return;
	}
	pthread_mutex_lock(this->lock);
	for(auto it = this->data->begin();it != this->data->end();it++){
		if(it->first->compare(username) == 0){
			this->data->erase(it);
			break;
		}
	}
	pthread_mutex_unlock(this->lock);
}

bool User_Manager::has_user(string* username){
	try{
		this->data->at(username);
	}catch(const out_of_range& oor){
		return false;
	}
	return true;
}

void User_Manager::connect(string* username,string key,int sd){
	if(!this->has_user(username)){
		this->login(*username);
	}
	pthread_mutex_lock(this->lock);
	try{
		this->data->at(username)->add(key,sd);
	}catch(const out_of_range& oor){
		cerr<<"User_Manager::connect:ERROR:"<<" Unexpected out of range exception when trying to find user"<<endl;
	}
	pthread_mutex_unlock(this->lock);

}

void User_Manager::connect(string* username,int64_t key,int sd){
	if(!this->has_user(username)){
		this->login(*username);
	}
	pthread_mutex_lock(this->lock);
	try{
		this->data->at(username)->add(key,sd);
	}catch(const out_of_range& oor){
		cerr<<"User_Manager::connect:ERROR:"<<" Unexpected out of range exception when trying to find user"<<endl;
	}
	
	pthread_mutex_unlock(this->lock);
}

void User_Manager::connect(string* username,UEKey key,int sd){
	if(key->s_key == nullptr){
		this->connect(username,key->i_key,sd);
	}else{
		this->connect(username,*key->s_key,sd);
	}
}

void User_Manager::disconnect(string* username,int sd){
	if(!this->has_user(username)){
		cout<<"User_Manager::disconnect:Warning: User does not exist"<<endl;
		return;
	}
	pthread_mutex_lock(this->lock);
	this->data->at(username)->remove_sd(sd);
	pthread_mutex_unlock(this->lock);
}

void User_Manager::drop(string* username,string key){
	if(!this->has_user(username)){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->data->at(username)->remove(key);
	pthread_mutex_unlock(this->lock);
}

void User_Manager::drop(string* username,int64_t key){
	if(!this->has_user(username)){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->data->at(username)->remove(key);
	pthread_mutex_unlock(this->lock);
}

void User_Manager::drop(string* username,UEKey key){
	if(key->s_key == nullptr){
		this->drop(username,key->i_key);
	}else{
		this->drop(username,*key->s_key);
	}
}

int User_Manager::lookup(string* username,string key){
	int out;
	if(!this->has_user(username)){
		return -1;
	}
	pthread_mutex_lock(this->lock);
	out = this->data->at(username)->find(key);
	//cout<<"Found sd to be "<<out<<endl;
	pthread_mutex_unlock(this->lock);
	return out;
}

int User_Manager::lookup(string* username,int64_t key){
	int out;
	if(!this->has_user(username)){
		return -1;
	}
	pthread_mutex_lock(this->lock);
	out = this->data->at(username)->find(key);
	pthread_mutex_unlock(this->lock);
	return out;
}

int User_Manager::lookup(string* username,UEKey key){
	if(key->s_key == nullptr){
		return this->lookup(username,key->i_key);
	}
	return this->lookup(username,*key->s_key);
}