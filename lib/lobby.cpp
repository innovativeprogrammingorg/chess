#include "lobby.h"

using namespace std;

Lobby::Lobby(){
	this->games = new vector<Lobby_Game*>();
	this->users = new vector<User*>();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
}

Lobby::~Lobby(){
	delete this->games;
	delete this->users;
	pthread_mutex_destroy(this->lock);
}

bool Lobby::has_games(){
	return this->games->size()>0;
}


bool Lobby::has_user(string username){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i)->username->compare(username) == 0){
			pthread_mutex_unlock(this->lock);
			return true;
		}
	}
	pthread_mutex_unlock(this->lock);
	return false;
}

User* Lobby::get_user(string username){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i)->username->compare(username) == 0){
			pthread_mutex_unlock(this->lock);
			return this->users->at(i);
		}
	}
	pthread_mutex_unlock(this->lock);
	return nullptr;
}

void Lobby::add_user(User* user){

	if(this->has_user(*user->username)){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->users->push_back(user);
	pthread_mutex_unlock(this->lock);
}

void Lobby::add_user(string name){
	this->add_user(new User(name));
}

void Lobby::add_game(Lobby_Game* lg){
	pthread_mutex_lock(this->lock);
	this->games->push_back(lg);
	pthread_mutex_unlock(this->lock);
}

void Lobby::add_game(int type, int sec,int min,int inc,char side,string host){
	
	Lobby_Game* lg = new Lobby_Game(type,sec,min,inc,host);
	User* p1 = new User(host,side);
	lg->add_player(p1);
	this->add_game(lg);
}

void Lobby::add_game(vector<string>* data,string host){
	//cout<<"DATA LENGTH IS: "<<data->size()<<endl;
	if(data->size()<5){
		cout<<"Failed to create game"<<endl;
		return;
	}
	string in_type = data->at(0);
	int type = REGULAR;
	if(in_type.compare("Bug House")==0){
		type = BUG_HOUSE;
	}
	//cout<<"Creating the game..."<<endl;
	cout<<"Game Parameters"<<endl<<type<<endl<<data->at(1)<<endl<<data->at(2)<<endl;
	
	this->add_game(
		type,
		stoi(data->at(1)),
		stoi(data->at(2)),
		stoi(data->at(3)),
		(char)data->at(4)[0],
		host);
	//cout<<"Created game successfully"<<endl;
}

Lobby_Game* Lobby::get_game(int64_t id){
	if(!this->has_games()){
		return nullptr;
	}
	for(auto it = this->games->begin();it!= this->games->end();it++){
		if((*it)->id == id){
			return *it;
		}
	}
	return nullptr;
}


Lobby_Game* Lobby::remove_game(string host){
	Lobby_Game* out;
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->games->size();i++){
		if(this->games->at(i)->host.compare(host) == 0){
			out = this->games->at(i);
			this->games->erase(this->games->begin() + i);
			pthread_mutex_unlock(this->lock);
			return out;
		}
	}
	pthread_mutex_unlock(this->lock);
	return nullptr;
}

Lobby_Game* Lobby::remove_game(int id){
	Lobby_Game* out;
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->games->size();i++){
		if(this->games->at(i)->id == id){
			out = this->games->at(i);
			this->games->erase(this->games->begin() + i);
			pthread_mutex_unlock(this->lock);
			return out;
		}
	}
	pthread_mutex_unlock(this->lock);
	return nullptr;
}

string Lobby::get_users(){
	string out = "";
	pthread_mutex_lock(this->lock);
	this->check();
	for(int64_t i = 0;i<this->users->size();i++){
		User* user = this->users->at(i);
		if(i != 0){
			out += DATA_SEP;
		}
		out += *user->username;
	}
	pthread_mutex_unlock(this->lock);
	return out;
}

string Lobby::get_games(){
	if(this->games->size()==0){
		return "none";
	}
	string out = "";
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->games->size();i++){
		if(i != 0){
			out += DATA_SEP;
		}
		out += this->games->at(i)->to_string(DATA_SEP);
	}
	pthread_mutex_unlock(this->lock);
	return out;
}

void Lobby::broadcast(Frame* frame){
	pthread_mutex_lock(this->lock);
	
	for(int64_t i = 0;i<this->users->size();i++){
		frame->send(this->users->at(i)->sd());
	}
	pthread_mutex_unlock(this->lock);
}

void Lobby::notify_users(){
	string msg = "LOBBY_USERS";
	msg += COMMAND;
	msg += this->get_users();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void Lobby::notify_games(){
	string msg = "LOBBY_GAMES";
	msg += COMMAND;
	msg += this->get_games();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void Lobby::notify(){
	this->notify_games();
	this->notify_users();
}

void Lobby::check(){
	if(this->users->size()==0){
		return;
	}
	for(auto it = this->users->begin();it != this->users->end();it++){
		if((*it)->sd() == -1){
			this->users->erase(it);
			if(this->users->size()>0){
				it = this->users->begin();
			}else{
				return;
			}
			
		}
	}
}



