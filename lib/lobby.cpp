#include "lobby.h"

using namespace std;
/**
 * In the future, this should be updated from the database
 */
int64_t Lobby_Game::game_id = 0;


Lobby_Game::Lobby_Game(int type,int sec,int min,int inc,char side,string host){
	this->type = type;
	this->sec = sec;
	this->min = min;
	this->inc = inc;
	this->side = side;
	this->host = host;
	this->id = Lobby_Game::game_id++;
}

Lobby_Game::~Lobby_Game(){

}

string Lobby_Game::get_type(){
	switch(this->type){
		case REGULAR:
		{
			return "REGULAR";
		}
		case BUG_HOUSE:
		{
			return "BUG_HOUSE";
		}
	}
	return "UNKNOWN";
}

string Lobby_Game::to_string(char sep){
	string out = "";
	out += this->get_type();
	out += sep;
	out += itoa(this->sec);
	out += sep;
	out += itoa(this->min);
	out += sep;
	out += itoa(this->inc);
	out += sep;
	out += this->side;
	out += sep;
	out += this->host;
	out += sep;
	out += ltos(this->id);
	return out;
}


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

bool Lobby::has_user(int sd){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i)->sd == sd){
			pthread_mutex_unlock(this->lock);
			return true;
		}
	}
	pthread_mutex_unlock(this->lock);
	return false;
}

bool Lobby::has_user(string username){
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		if(this->users->at(i)->username.compare(username) == 0){
			pthread_mutex_unlock(this->lock);
			return true;
		}
	}
	pthread_mutex_unlock(this->lock);
	return false;
}

void Lobby::add_user(User* user){

	if(this->has_user(user->sd)){
		return;
	}
	pthread_mutex_lock(this->lock);
	this->users->push_back(user);
	pthread_mutex_unlock(this->lock);
}

void Lobby::add_user(string name, int sd){
	this->add_user(new User(name,sd));
}

void Lobby::add_game(Lobby_Game* lg){
	pthread_mutex_lock(this->lock);
	this->games->push_back(lg);
	pthread_mutex_unlock(this->lock);
}

void Lobby::add_game(int type, int sec,int min,int inc,char side,string host){
	this->add_game(new Lobby_Game(type,sec,min,inc,side,host));
}

void Lobby::add_game(vector<string>* data,string host){
	this->add_game(new Lobby_Game(
		stoi(data->at(0)),
		stoi(data->at(1)),
		stoi(data->at(2)),
		stoi(data->at(3)),
		(char)data->at(4)[0],
		host
		));
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

string Lobby::get_users(char sep){
	string out = "";
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->users->size();i++){
		User* user = this->users->at(i);
		if(i != 0){
			out += sep;
		}
		out += user->username;
	}
	pthread_mutex_unlock(this->lock);
	return out;
}

string Lobby::get_games(char sep){
	string out = "";
	pthread_mutex_lock(this->lock);
	for(int64_t i = 0;i<this->games->size();i++){
		if(i != 0){
			out += sep;
		}
		out += this->games->at(i)->to_string(sep);
	}
	pthread_mutex_unlock(this->lock);
	return out;
}

void Lobby::broadcast(Frame* frame){
	pthread_mutex_lock(this->lock);
	
	for(int64_t i = 0;i<this->users->size();i++){
		frame->send(this->users->at(i)->sd);
	}
	pthread_mutex_unlock(this->lock);
}
