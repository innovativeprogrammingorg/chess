#include "manager.h"


using namespace std;

Manager* Manager::GM = new Manager();

Manager::Manager(){
	this->games = new Game_Manager();
	this->lobby_chat = new Chat();
	this->lobby = new Lobby();
	this->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(this->lock,NULL);
}

string Manager::process(Client* c,string data, int command, Game* out_game, int* sd){
	switch(command){
		case MOVE:
		{
			vector<string>* move_data = c_explode(DATA_SEP,data);
			int* move = Manager::processMoveData(move_data);
			if(c->username == nullptr){
				c->username = new string(move_data->at(USER_INDEX));
			}
			int64_t gi = Manager::find_game(*c->username);
			if(gi == -1){
				cout<<"GAME NOT FOUND"<<endl;
				return "ERROR";
			}
			Chess* game = Manager::GM->games->at(gi);
			if(out_game != nullptr){
				*out_game = *(game->game);
			}
			game->move(move[0],move[1],move[2],move[3],move_data->at(SIDE_INDEX)[0]);
			
			return "";
		}

		case NEW:
		case JOIN:
		{
			//No longer used
			return "ERROR";
		}
		case LOGIN:
		{
			c->username = new string(data);
			return "LOGGED_IN";
		}
		case OFFER_DRAW:
		{
			int64_t gid = Manager::find_game(*c->username);
			Manager::GM->games->at(gid)->offer_draw(*c->username);
			return "DRAW_OFFERED";
		}
		case ACCEPT_DRAW:
		{
			int64_t gid = Manager::find_game(*c->username);
			Manager::GM->games->at(gid)->accept_draw(*c->username);
			return "";
		}
		case DECLINE_DRAW:
		{
			int64_t gid = Manager::find_game(*c->username);
			Manager::GM->games->at(gid)->decline_draw(*c->username);
			return "DRAW_DECLINED";
		}	
		case RESIGN:
		{
			int64_t gid = Manager::find_game(*c->username);
			Manager::GM->games->at(gid)->resign(*c->username);
			return "RESIGN";
		}
		/**Chat Handlers**/
		case LOBBY_MESSAGE:
		{
			Manager::GM->lobby_chat->add(*c->username,data);
			Manager::GM->lobby_chat->send_last();
			return "";
		}
		case GET_LOBBY_MESSAGES:
		{
			Manager::GM->lobby_chat->connect(*c->username);
			return Frame::prepare_message(2,string("CHAT_ALL"),Manager::GM->lobby_chat->to_string());
		}

		case GET_LOBBY_USERS:
		{
			Manager::GM->lobby->add_user(*c->username);
			Manager::GM->lobby->notify_users();
			return "";
		}
		case CREATE_LOBBY_GAME:
		{
			vector<string>* game_data = c_explode(DATA_SEP,data);
			Manager::GM->lobby->add_game(game_data,*c->username);
			if(!Manager::GM->lobby->has_games()){
				delete game_data;
				return "ERROR";
			}
			Manager::GM->lobby->notify_games();
			delete game_data;
			return "";
		}
		case GET_LOBBY_GAMES:
		{
			if(!Manager::GM->lobby->has_games()){
				return "";
			}
			return Frame::prepare_message(2,"LOBBY_GAMES",Manager::GM->lobby->get_games());
		}
		case GET_LOBBY_ALL:
		{
			Manager::GM->lobby->add_user(*c->username);
			Manager::GM->lobby_chat->connect(*c->username);
			Manager::GM->lobby->notify();
			return Frame::prepare_message(2,string("CHAT_ALL"),Manager::GM->lobby_chat->to_string());
		}

		case REMOVE_LOBBY_GAME:
		{
			Manager::GM->lobby->remove_game(stoi(data));
			Manager::GM->lobby->notify_games();
			return "";
		}

		case JOIN_LOBBY_GAME:
		{
			int64_t id = stoi(data);
			Lobby_Game* lg = Manager::GM->lobby->get_game(id);
			lg->add_player(*c->username);
			if(lg->is_ready()){
				Game* g = lg->create_game();
				Chess* game = new Chess(g);

				Manager::GM->lobby->remove_game(id);
				Manager::create_game(game);
				game->start();
			}
			return "";
		}
		case GET_GAME_ALL:
		{
			int64_t id = stoi(data);
			Chess* game = Manager::find_game(id);
			if(game == nullptr){
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
			Chess* game = Manager::find_game(id);
			if(game == nullptr){
				return Frame::prepare_message(2,"ERROR","Game Not Found");
			}
			game->message(*c->username,msg_data->at(1));
			delete msg_data;
			return "";
		}

			
	}
	return "ERROR";
}

Chess* Manager::find_game(int64_t id){
	Chess* out = Manager::GM->games->get_game(id);
	if(out == nullptr){
		out = Manager::GM->games->load_game(id);
	}
	return out;
}


void Manager::create_game(Chess* g){
	Manager::GM->games->add_game(g);
}


int* Manager::processMoveData(vector<string>* data){
	int* out = (int*)malloc(sizeof(int)*4);
	uint8_t offset = 0;
	out[0] = data->at(offset)[0];
	out[1] = data->at(offset+1)[0];
	out[2] = data->at(offset+2)[0];
	out[3] = data->at(offset+3)[0];
	return out;
}


