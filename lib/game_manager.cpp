#include "game_manager.h"


using namespace std;

Game_Manager* Game_Manager::GM = new Game_Manager();

Game_Manager::Game_Manager(){
	this->games = new vector<Game*>();
	this->chats = new map<int64_t,Chat*>();
	this->lobby_chat = new Chat();
	this->lobby = new Lobby();
}

string Game_Manager::prepare_message(int args,...){
	va_list valist;
	int i = 0;
	string out = "";
	va_start(valist, args);
	for(i = 0;i<args;i++){
		if(i == 0){
			out += (string)va_arg(valist, string);
			out += COMMAND;
		}else if(i==1){
			out += (string)va_arg(valist, string);
			
		}else{
			out += DATA_SEP;
			out += (string)va_arg(valist, string);
		}
	}
	va_end(valist);
	return out;
}

string Game_Manager::process(Client* c,string data, int command, Game* out_game, int* sd){
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
			vector<string>* game_data = c_explode(DATA_SEP,data);
			map<string,string>* args = new map<string,string>();
			args->insert(pair<string,string>("user",game_data->at(0)));
			args->insert(pair<string,string>("gid",game_data->at(1)));
			string info = get_game_info(args);
			delete args;
			delete game_data;
			game_data = c_explode(DATA_SEP,data);
			Game_Manager::create_game(game_data,c->fd);
			string out = Game_Manager::prepare_message(2,"NEW",game_data->at(FEN_INDEX));
			delete game_data;
			return out;
		}
		case JOIN:
		{
			vector<string>* game_data = c_explode(DATA_SEP,data);
			map<string,string>* args = new map<string,string>();
			args->insert(pair<string,string>("user",game_data->at(0)));
			args->insert(pair<string,string>("gid",game_data->at(1)));
			string info = get_game_info(args);
			delete args;
			delete game_data;
			game_data = c_explode(DATA_SEP,data);
			Game_Manager::join_game(game_data,c->fd);
			string out = Game_Manager::prepare_message(2,"JOIN",game_data->at(FEN_INDEX));
			delete game_data;
			return out;
		}
		case LOGIN:
		{
			c->username = new string(data);
			return "LOGGED_IN";
		}
		case OFFER_DRAW:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game* game = Game_Manager::GM->games->at(gid);
			if(game->white->username == *c->username){
				*sd = game->black->sd;
			}else{
				*sd = game->white->sd;
			}
			return "DRAW_OFFERED";
		}
		case ACCEPT_DRAW:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game* game = Game_Manager::GM->games->at(gid);
			if(game->white->username == *c->username){
				*sd = game->black->sd;
			}else{
				*sd = game->white->sd;
			}
			return "DRAW_ACCEPTED";
		}
		case DECLINE_DRAW:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game* game = Game_Manager::GM->games->at(gid);
			if(game->white->username == *c->username){
				*sd = game->black->sd;
			}else{
				*sd = game->white->sd;
			}
			return "DRAW_DECLINED";
		}	
		case RESIGN:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game* game = Game_Manager::GM->games->at(gid);
			if(game->white->username == *c->username){
				*sd = game->black->sd;
			}else{
				*sd = game->white->sd;
			}
			return "RESIGN";
		}
		/**Chat Handlers**/
		case LOBBY_MESSAGE:
		{
			//vector<string>* msg_data = c_explode(DATA_SEP,data);
			Game_Manager::GM->lobby_chat->add(*c->username,data);
			Game_Manager::GM->lobby_chat->connect(c->fd);
			string msg = "CHAT";
			msg += COMMAND;
			msg += Game_Manager::GM->lobby_chat->get_last(DATA_SEP);

			Frame* frame = new Frame();
			frame->add((uint8_t*)msg.c_str());
			frame->fin = 1;
			frame->mask = 0;
			frame->mask_key = 0;
			frame->opcode = TEXT;
			Game_Manager::GM->lobby_chat->broadcast(frame);
			delete frame;
			return "";
		}
		case GET_LOBBY_MESSAGES:
		{
			Game_Manager::GM->lobby_chat->connect(c->fd);
			return Game_Manager::prepare_message(2,string("CHAT_ALL"),Game_Manager::GM->lobby_chat->to_string(DATA_SEP));
		}

		case GET_LOBBY_USERS:
		{
			Game_Manager::GM->lobby->add_user(*c->username,c->fd);
			cout<<"ADDED USER"<<endl;
			cout<<"Current Users: "<<Game_Manager::GM->lobby->get_users(' ')<<endl;
			string msg = "LOBBY_USERS";
			msg += COMMAND;
			msg += Game_Manager::GM->lobby->get_users(DATA_SEP);
			Frame* frame = new Frame();
			frame->add((uint8_t*)msg.c_str());
			frame->fin = 1;
			frame->mask = 0;
			frame->opcode = TEXT;
			Game_Manager::GM->lobby->broadcast(frame);
			delete frame;
			return "";
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
	if(Game_Manager::find_game(gid)!= -1){
		Game_Manager::join_game(data,sd);
		return;
	}
	Board* b = new Board(FEN,"",castle);
	User* u = new User(username,sd);
	Game* g = new Game(nullptr,nullptr,gid,b,turn,0,inc);
	g->white_time = stoi(data->at(WTIME_INDEX));
	g->black_time = stoi(data->at(BTIME_INDEX));
	if(side == WHITE){
		g->white = u;
	}else{
		g->black = u;
	}
	Game_Manager::GM->games->push_back(g);
}

void Game_Manager::join_game(vector<string>* data, int sd){
	int64_t gid = stoi(data->at(ID_INDEX));
	int64_t index = Game_Manager::find_game(gid);
	if(index == -1){
		Game_Manager::create_game(data,sd);
		return;
	}
	Game* g = Game_Manager::GM->games->at(index);
	char side = data->at(SIDE_INDEX)[0];
	if(side == WHITE){
		g->white = new User(data->at(USER_INDEX),sd);
	}else{
		g->black = new User(data->at(USER_INDEX),sd);
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
	uint8_t offset = 0;
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

