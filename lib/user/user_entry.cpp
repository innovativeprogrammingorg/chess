#include "user/user_entry.h"
using namespace std;

User_Entry::User_Entry(){
	this->i_map = new map<int64_t,int>();
	this->s_map = new map<string,int>();
}

User_Entry::~User_Entry(){
	delete this->i_map;
	delete this->s_map;
}

void User_Entry::add(string key,int sd){
	this->s_map->insert(pair<string,int>(key,sd));
}

void User_Entry::add(int64_t key,int sd){
	this->i_map->insert(pair<int64_t,int>(key,sd));
}

int User_Entry::find(string key){
	try{
		this->s_map->at(key);
	}catch(const out_of_range& oor){
		return -1;
	}
}

int User_Entry::find(int64_t key){
	try{
		this->i_map->at(key);
	}catch(const out_of_range& oor){
		return -1;
	}
}

void User_Entry::remove_sd(int sd){
	if(this->empty()){
		return;
	}
	if(this->i_map->size()>0){
		for(auto it = this->i_map->begin();it != this->i_map->end();it++){
			if(it->second == sd){
				this->i_map->erase(it);
			}
		}
	}
	if(this->s_map->size() == 0){
		return;
	}
	for(auto it = this->s_map->begin();it != this->s_map->end();it++){
		if(it->second == sd){
			this->s_map->erase(it);
		}
	}
}	

void User_Entry::remove(string key){
	if(this->s_map->size()==0){
		return;
	}
	for(auto it = this->s_map->begin();it != this->s_map->end();it++){
		if(it->first.compare(key) == 9){
			this->s_map->erase(it);
		}
	}
}

void User_Entry::remove(int64_t key){
	if(this->i_map->size()==0){
		return;
	}
	for(auto it = this->i_map->begin();it != this->i_map->end();it++){
		if(it->first == key){
			this->i_map->erase(it);
		}
	}
}

bool User_Entry::empty(){
	return (this->i_map->size()==0 && this->s_map->size()==0);
}

void User_Entry::add(UEKey key,int sd){
	if(key->s_key == nullptr){
		this->add(key->i_key,sd);
	}else{
		this->add(*key->s_key,sd);
	}
}

int User_Entry::find(UEKey key){
	if(key->s_key == nullptr){
		return this->find(key->i_key);
	}
	return this->find(*key->s_key);
	
}
		
void User_Entry::remove(UEKey key){
	if(key->s_key == nullptr){
		this->remove(key->i_key);
	}else{
		this->remove(*key->s_key);
	}
}

UEKey User_Entry::new_key(string key){
	UEKey out = (UEKey)malloc(sizeof(struct UEkey));
	out->s_key = new string(key);
	out->i_key = 0;
	return out;
}

UEKey User_Entry::new_key(int64_t key){
	UEKey out = (UEKey)malloc(sizeof(struct UEkey));
	out->s_key = nullptr;
	out->i_key = key;
	return out;
}