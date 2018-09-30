#include "game_manager.h"

using namespace std;


static map<int64_t,Chess*>* games;

game_manager::game_manager()
{
	
	pthread_mutex_init(this->lock,NULL);

	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("SELECT ID FROM chessgame ORDER BY ID DESC LIMIT 1");
	if(res->next()){
		this->id = res->getInt64("ID") + 1;
	}else{
		cerr<<"Warning: no results found in chessgame"<<endl;
		this->id = 0;
	}
	delete res;
	delete conn;
}

game_manager::~game_manager(){
	pthread_mutex_destroy(&this->lock);
}

void game_manager::add_game(Chess game){
	pthread_mutex_lock(this->lock);
	game->game->id = this->id;
	this->id++;
	this->games->insert(pair<int64_t,Chess*>((int64_t)game->game->id,game));
	pthread_mutex_unlock(this->lock);
}

void game_manager::add_game(chess* game,int64_t id){
	pthread_mutex_lock(this->lock);
	game->game->id = id;
	this->games->insert(pair<int64_t,Chess*>(id,game));
	pthread_mutex_unlock(this->lock);
}

IGame* game_manager::get_game(int64_t id){
	Chess* out = nullptr;
	try{
		out = this->games->at(id);
	}catch(const out_of_range& oor){
		out = nullptr;
	}
	return out;
}

IGame* game_manager::load_game(int64_t id){
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",(int)id);
	if(!res->next()){
		cerr<<"Warning: Could not load requested game"<<endl;
		return nullptr;
	}
	string special = "false";
	if(res->getString("WEP").compare("false") != 0){
		special = res->getString("WEP");
	}else if(res->getString("BEP").compare("false") != 0){
		special = res->getString("BEP");
	}
	user black(res->getString("Black"),BLACK);
	user white(res->getString("White"),WHITE);
	board game_board(res->getString("Board"),special, res->getString("Castle"));
	timer clck(res->getInt("WTime"),res->getInt("BTime"),res->getInt("I"),res->getString("Turn")[0],time(NULL),res->getInt("Undo_Time"));
	
	History* history = new History(res->getString("Past"),res->getString("moves"),res->getString("White_Captured"),
									res->getString("Black_Captured"),res->getInt("Turns"));
	Game* game = new Game(black,white,id,board,timer,history);
	Chess* chess = new Chess(game);
	this->add_game(chess,id);
	delete conn;
	delete res;
	return chess;
}

void game_manager::drop_game(int64_t id){
	pthread_mutex_lock(this->lock);
	try{
		this->games->erase(this->games->find(id));
	}catch(const out_of_range& oor){
		cout<<"Warning: Could not find the game to erase"<<endl;
	}
	pthread_mutex_unlock(this->lock);
}

void game_manager::save_game(int64_t id){
	try{
		this->games.at(id)->save();
	}catch(const out_of_range& oor){
		cout<<"Warning could not find game to save"<<endl;
	}
}

void game_manager::save(){
	if(this->games->size() == 0){
		return;
	}
	pthread_mutex_lock(this->lock);
	for(auto it = this->games->begin();it != this->games->end();it++){
		it->second->save();
	}
	pthread_mutex_unlock(this->lock);

}