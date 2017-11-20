#include "game_manager.h"

using namespace std;


map<int64_t,Chess*>* games;

Game_Manager::Game_Manager(){
	this->games = new map<int64_t,Chess*>();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);

	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("SELECT ID FROM chessgame ORDER BY ID DESC LIMIT 1");
	if(res->next()){
		this->id = res->getInt64("ID") + 1;
	}else{
		cout<<"Warning: no results found in chessgame"<<endl;
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
	pthread_mutex_lock(this->lock);
	game->game->id = this->id;
	this->id++;
	this->games->insert(pair<int64_t,Chess*>((int64_t)game->game->id,game));
	pthread_mutex_unlock(this->lock);
}

void Game_Manager::add_game(Chess* game,int64_t id){
	pthread_mutex_lock(this->lock);
	game->game->id = id;
	this->games->insert(pair<int64_t,Chess*>(id,game));
	pthread_mutex_unlock(this->lock);
}

Chess* Game_Manager::get_game(int64_t id){
	Chess* out = nullptr;
	try{
		out = this->games->at(id);
	}catch(const out_of_range& oor){
		out = nullptr;
	}
	return out;
}

Chess* Game_Manager::load_game(int64_t id){
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",(int)id);
	if(!res->next()){
		cout<<"Warning: Could not load requested game"<<endl;
		return nullptr;
	}
	string special = "false";
	if(res->getString("WEP").compare("false") != 0){
		special = res->getString("WEP");
	}else if(res->getString("BEP").compare("false") != 0){
		special = res->getString("BEP");
	}
	User* black = new User(res->getString("Black"),BLACK);
	User* white = new User(res->getString("White"),WHITE);
	Board* board = new Board(res->getString("Board"),special, res->getString("Castle"));
	Timer* timer = new Timer(res->getInt("WTime"),res->getInt("BTime"),res->getInt("I"),res->getString("Turn")[0],time(NULL),res->getInt("Undo_Time"));
	Game* game = new Game(black,white,id,board,timer);

	Chess* chess = new Chess(game,res->getString("Past"),res->getString("moves"),res->getString("White_Captured"),
							res->getString("Black_Captured"),res->getInt("Turns"));
	this->add_game(chess,id);
	delete conn;
	delete res;
	return chess;
}

void Game_Manager::drop_game(int64_t id){
	pthread_mutex_lock(this->lock);
	try{
		this->games->erase(this->games->find(id));
	}catch(const out_of_range& oor){
		cout<<"Warning: Could not find the game to erase"<<endl;
	}
	pthread_mutex_unlock(this->lock);
}

void Game_Manager::save_game(int64_t id){
	try{
		this->games->at(id)->save();
	}catch(const out_of_range& oor){
		cout<<"Warning could not find game to save"<<endl;
	}
}

void Game_Manager::save(){
	if(this->games->size() == 0){
		return;
	}
	pthread_mutex_lock(this->lock);
	for(auto it = this->games->begin();it != this->games->end();it++){
		it->second->save();
	}
	pthread_mutex_unlock(this->lock);

}