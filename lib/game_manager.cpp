#include "game_manager.h"


using namespace std;

Game_Manager* Game_Manager::GM = new Game_Manager();

Game_Manager::Game_Manager(){
	this->games = new vector<Chess*>();
	this->lobby_chat = new Chat();
	this->lobby = new Lobby();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
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
			Chess* game = Game_Manager::GM->games->at(gi);
			if(out_game != nullptr){
				*out_game = *(game->game);
			}
			game->move(move[0],move[1],move[2],move[3],move_data->at(SIDE_INDEX)[0]);
			
			return "";
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
			Game_Manager::create_game(game_data,c->sd);
			string out = Frame::prepare_message(2,"NEW",game_data->at(FEN_INDEX));
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
			Game_Manager::join_game(game_data,c->sd);
			string out = Frame::prepare_message(2,"JOIN",game_data->at(FEN_INDEX));
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
			Game_Manager::GM->games->at(gid)->offer_draw(*c->username);
			return "DRAW_OFFERED";
		}
		case ACCEPT_DRAW:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game_Manager::GM->games->at(gid)->accept_draw(*c->username);
			return "";
		}
		case DECLINE_DRAW:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game_Manager::GM->games->at(gid)->decline_draw(*c->username);
			return "DRAW_DECLINED";
		}	
		case RESIGN:
		{
			int64_t gid = Game_Manager::find_game(*c->username);
			Game_Manager::GM->games->at(gid)->resign(*c->username);
			return "RESIGN";
		}
		/**Chat Handlers**/
		case LOBBY_MESSAGE:
		{
			Game_Manager::GM->lobby_chat->add(*c->username,data);
			Game_Manager::GM->lobby_chat->send_last();
			return "";
		}
		case GET_LOBBY_MESSAGES:
		{
			Game_Manager::GM->lobby_chat->connect(*c->username);
			return Frame::prepare_message(2,string("CHAT_ALL"),Game_Manager::GM->lobby_chat->to_string());
		}

		case GET_LOBBY_USERS:
		{
			Game_Manager::GM->lobby->add_user(*c->username);
			Game_Manager::GM->lobby->notify_users();
			return "";
		}
		case CREATE_LOBBY_GAME:
		{
			vector<string>* game_data = c_explode(DATA_SEP,data);
			Game_Manager::GM->lobby->add_game(game_data,*c->username);
			if(!Game_Manager::GM->lobby->has_games()){
				delete game_data;
				return "ERROR";
			}
			Game_Manager::GM->lobby->notify_games();
			delete game_data;
			return "";
		}
		case GET_LOBBY_GAMES:
		{
			if(!Game_Manager::GM->lobby->has_games()){
				return "";
			}
			return Frame::prepare_message(2,"LOBBY_GAMES",Game_Manager::GM->lobby->get_games());
		}
		case GET_LOBBY_ALL:
		{
			Game_Manager::GM->lobby->add_user(*c->username);
			Game_Manager::GM->lobby_chat->connect(*c->username);
			Game_Manager::GM->lobby->notify();
			return Frame::prepare_message(2,string("CHAT_ALL"),Game_Manager::GM->lobby_chat->to_string());
		}

		case REMOVE_LOBBY_GAME:
		{
			Game_Manager::GM->lobby->remove_game(stoi(data));
			Game_Manager::GM->lobby->notify_games();
			return "";
		}

		case JOIN_LOBBY_GAME:
		{
			int64_t id = stoi(data);
			Lobby_Game* lg = Game_Manager::GM->lobby->get_game(id);
			lg->add_player(*c->username);
			if(lg->is_ready()){
				Game* g = lg->create_game();
				Chess* game = new Chess(g);

				Game_Manager::GM->lobby->remove_game(id);
				Game_Manager::create_game(game);
				game->start();
			}
			return "";
		}
		case GET_GAME_ALL:
		{
			int64_t id = stoi(data);
			Chess* game;
			try{
				game = Game_Manager::GM->games->at(Game_Manager::find_game(id));
			}catch(const out_of_range& err){
				return Frame::prepare_message(2,"ERROR","Game Not Found");
			}
			game->notify_sides();
			game->chat->send_all(c->sd);	
			game->send_board();
			game->notify_turn();
			game->send_time();
			return "";
		}

		case CHESS_MESSAGE:
		{	
			vector<string>* msg_data = c_explode(DATA_SEP,data);
			if(msg_data->size()<2){
				return "ERROR";
			}
			int64_t id = stoi(msg_data->at(0));
			Chess* game;
			try{
				game = Game_Manager::GM->games->at(Game_Manager::find_game(id));
			}catch(const out_of_range& err){
				return Frame::prepare_message(2,"ERROR","Game Not Found");
			}
			game->message(*c->username,msg_data->at(1));
			delete msg_data;
			return "";
		}

			
	}
	return "ERROR";
}

int64_t Game_Manager::find_game(int64_t id){
	int64_t i = 0;
	for(int64_t i = 0;i<Game_Manager::GM->games->size();i++){
		if(Game_Manager::GM->games->at(i)->game->id == id){
			return i;
		}
	}
	return -1;
}

int64_t Game_Manager::find_game(string username){
	int64_t i = 0;
	for(int64_t i = 0;i<Game_Manager::GM->games->size();i++){
		if(Game_Manager::GM->games->at(i)->game->white->username->compare(username)==0
			 || Game_Manager::GM->games->at(i)->game->black->username->compare(username)==0){
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
	Chess* game = new Chess(g);
	Game_Manager::GM->games->push_back(game);
}

void Game_Manager::create_game(Chess* g){
	Game_Manager::GM->games->push_back(g);
}

void Game_Manager::join_game(vector<string>* data, int sd){
	int64_t gid = stoi(data->at(ID_INDEX));
	int64_t index = Game_Manager::find_game(gid);
	if(index == -1){
		Game_Manager::create_game(data,sd);
		return;
	}
	Chess* g = Game_Manager::GM->games->at(index);
	char side = data->at(SIDE_INDEX)[0];
	if(side == WHITE){
		g->game->white = new User(data->at(USER_INDEX),sd);
	}else{
		g->game->black = new User(data->at(USER_INDEX),sd);
	}
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


