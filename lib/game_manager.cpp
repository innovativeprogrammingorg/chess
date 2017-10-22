#include "game_manager.h"


using namespace std;

Game_Manager::Game_Manager(){
	this->games = new vector<Game*>();
}

int64_t Game_Manager::find_game(int64_t id){
	int64_t i = 0;
	for(int64_t i = 0;i<this->games->size();i++){
		if(this->games->at(i)->id == id){
			return i;
		}
	}
	return -1;
}

void Game_Manager::create_game(vector<string>* data,int fd){
	char side = data->at(SIDE_INDEX)[0];
	string username = data->at(USER_INDEX);
	int64_t gid = stoi(data->at(ID_INDEX));
	string FEN = data->at(FEN_INDEX);
	char turn = data->at(TURN_INDEX)[0];
	uint64_t inc = stoi(data->at(INC_INDEX));
	string castle = "bkbqwkwq";
	uint64_t duration = stoi(data->at(DUR_INDEX));

	Board* b = new Board(FEN,"",castle);
	User* u = new User(username);
	Game* g = new Game(nullptr,nullptr,gid,b,turn,duration,inc);
	if(side == WHITE){
		g->white = u;
	}else{
		g->black = u;
	}
	this->games->push_back(g);
}

void Game_Manager::join_game(vector<string>* data,int fd){
	int64_t gid = stoi(data->at(3));
	int64_t index = this->find_game(gid);
	if(index == -1){
		this->create_game(data,fd);
		return;
	}
	Game* g = this->games->at(index);
	char side =data->at(1)[0];
	if(side == WHITE){
		g->white = new User(data->at(2));
	}else{
		g->black = new User(data->at(2));
	}
}

string Game_Manager::get_game_state(int64_t gid){
	JSON* data = new JSON("string");
	int64_t index = this->find_game(gid);
	if(index == -1){
		return "";
	}
	Game* g = this->games->at(index);
	data->add("Black",g->black->username);
	data->add("White",g->white->username);
	data->add("Board",g->board->getBoardData());
	data->add("Turn",new string((char*)&(g->turn)));
	data->add("Start",itoa(g->start));
	data->add("Duration",itoa(g->duration));
	data->add("Inc",itoa(g->inc));
	data->add("WTime",itoa(g->white_time));
	data->add("BTime",itoa(g->black_time));
	data->add("ID",itoa(gid));
	string out = data->to_string();
	delete data;
	return out;
}

int* Game_Manager::processMoveData(vector<string>* data){
	int* out = (int*)malloc(sizeof(int)*4);
	uint8_t offset = 2;
	out[0] = data->at(offset)[0];
	out[1] = data->at(offset+1)[0];
	out[2] = data->at(offset+2)[0];
	out[3] = data->at(offset+3)[0];
	return out;
}

void Game_Manager::disconnectClient(int64_t id){
	int64_t index = this->find_game(id);
	this->games->at(index)->store();

}

