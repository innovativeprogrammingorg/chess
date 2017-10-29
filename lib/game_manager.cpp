#include "game_manager.h"


using namespace std;

Game_Manager* Game_Manager::GM = new Game_Manager();

Game_Manager::Game_Manager(){
	this->games = new vector<Game*>();
}

string Game_Manager::process(Client* c,string data, int command, Game* out_game){
	switch(command){
		case MOVE:
			{
				vector<string>* move_data = c_explode(DATA_SEP,data);
				int* move = Game_Manager::processMoveData(move_data);
				if(c->username == nullptr){
					c->username = new string(move_data->at(USER_INDEX));
				}
				int64_t gi = Game_Manager::find_game(*c->username);
				if(gi == -1){
					cout<<"GAME NOT FOUND"<<endl;
					return "ERROR";
				}
				Game* game = Game_Manager::GM->games->at(gi);
				if(out_game != nullptr){
					*out_game = *game;
				}
				uint8_t results = game->move(move[0],move[1],move[2],move[3],move_data->at(SIDE_INDEX)[0]);
				delete move_data;
				free(move);
				if(!results){
					return "Illegal Move";
				}

				if(results == PROMOTION){
					return "Promotion";//Waits for choice before sending
				}else{
					game->send_state_to_opponent(c->fd);
					return "Move Success";
				}
				break;
			}
			
		case NEW:
			{
				vector<string>* game_data = explode("#",data);
				Game_Manager::create_game(game_data,c->fd);
				return "Success";
			}
		case JOIN:
			{
				vector<string>* game_data = explode("#",data);
				try{
					Game_Manager::join_game(game_data,c->fd);
				}catch(const out_of_range& oor){
					return "Game Not Found";
				}
				return "Success";
			}
		case LOGIN:
			{
				c->username = new string(data);
				return "Logged In";
			}

			






	}
	return "ERROR";
}

int64_t Game_Manager::find_game(int64_t id){
	int64_t i = 0;
	for(int64_t i = 0;i<Game_Manager::GM->games->size();i++){
		if(Game_Manager::GM->games->at(i)->id == id){
			return i;
		}
	}
	return -1;
}

int64_t Game_Manager::find_game(string username){
	int64_t i = 0;
	for(int64_t i = 0;i<Game_Manager::GM->games->size();i++){
		if(Game_Manager::GM->games->at(i)->white->username == username || Game_Manager::GM->games->at(i)->black->username == username){
			return i;
		}
	}
	return -1;
}

void Game_Manager::create_game(vector<string>* data, int sd){
	char side = data->at(SIDE_INDEX)[0];
	string username = data->at(USER_INDEX);
	int64_t gid = stoi(data->at(ID_INDEX));
	string FEN = data->at(FEN_INDEX);
	char turn = data->at(TURN_INDEX)[0];
	uint64_t inc = stoi(data->at(INC_INDEX));
	string castle = "bkbqwkwq";
	uint64_t duration = stoi(data->at(DUR_INDEX));

	Board* b = new Board(FEN,"",castle);
	User* u = new User(username,sd);
	Game* g = new Game(nullptr,nullptr,gid,b,turn,duration,inc);
	if(side == WHITE){
		g->white = u;
	}else{
		g->black = u;
	}
	Game_Manager::GM->games->push_back(g);
}

void Game_Manager::join_game(vector<string>* data, int sd){
	int64_t gid = stoi(data->at(3));
	int64_t index = Game_Manager::find_game(gid);
	if(index == -1){
		Game_Manager::create_game(data,sd);
		return;
	}
	Game* g = Game_Manager::GM->games->at(index);
	char side = data->at(1)[0];
	if(side == WHITE){
		g->white = new User(data->at(2),sd);
	}else{
		g->black = new User(data->at(2),sd);
	}
}

string Game_Manager::get_game_state(int64_t gid){
	JSON* data = new JSON("string");
	int64_t index = Game_Manager::find_game(gid);
	if(index == -1){
		return "";
	}
	Game* g = Game_Manager::GM->games->at(index);
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
	int64_t index = Game_Manager::GM->find_game(id);
	Game_Manager::GM->games->at(index)->store();

}

