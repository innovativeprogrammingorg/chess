#include "game_manager.h"

using namespace std;


map<int64_t,Chess*>* games;

Game_Manager::Game_Manager(){
	this->games = new map<int64_t,Chess*>();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("SELECT ID FROM chessgame ORDER BY ID DEC LIMIT 1");
	if(res->next()){
		this->id = res->getInt("ID");
	}else{
		cout<<"ERROR, not results found in chessgame"<<endl;
		this->id = 0;
	}
	delete res;
	delete conn;
}

Game_Manager::~Game_Manager(){
	delete this->games;
	pthread_mutex_unlock(this->lock);
	pthread_mutex_destroy(this->lock);
}

void Game_Manager::add_game(Chess* game){

}

void Game_Manager::add_game(Chess* game,int64_t id){
	game->game->id = id;
	this->games->insert(pair<int64_t,Chess*>(id,game));
}

Chess* Game_Manager::get_game(int64_t id){

}

Chess* Game_Manager::load_game(int64_t id){

}

void Game_Manager::drop_game(int64_t id){
	try{
		this->games->erase(this->games->find(id));
	}catch(const out_of_range& oor){
		return;
	}
	
}

void Game_Manager::save_game(int64_t id){

}

void Game_Manager::save(){

}