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

Manager::~Manager(){
	delete this->games;
	delete this->lobby_chat;
	delete this->lobby;
	pthread_mutex_unlock(this->lock);
	pthread_mutex_destroy(this->lock);
}

string Manager::process(Client* c,string data, int command){
	switch(command){
		case MOVE:
		{
			vector<string>* move_data = c_explode(DATA_SEP,data);
			if(move_data->size()<5){
				delete move_data;
				return "INVALID_MOVE";
			}
			int* move = Manager::processMoveData(move_data);
			Chess* game = Manager::find_game(stoi(move_data->at(0)));
			delete move_data;
			if(game == nullptr){
				cout<<"GAME NOT FOUND"<<endl;
				return "ERROR";
			}
			game->move(move[0],move[1],move[2],move[3],game->get_side_of(*c->username));
			free(move);
			return "";
		}
		case LOGIN:
		{
			//cout<<"Logging the user in..."<<endl;
			if(data.find(DATA_SEP) == string::npos){//default to lobby
				c->username = new string(data);
				User_Manager::UM->connect(c->username,string("LOBBY"),c->sd);
			}else{
				vector<string>* msg_data = c_explode(DATA_SEP,data);
				c->username = new string(msg_data->at(0));
				User_Manager::UM->connect(c->username,stoi(msg_data->at(1)),c->sd);
				delete msg_data;
			}
			
			return "LOGGED_IN";
		}
		case OFFER_DRAW:
		{
			int64_t id = stoi(data);
			Manager::find_game(id)->offer_draw(*c->username);
			return "DRAW_OFFERED";
		}
		case ACCEPT_DRAW:
		{
			int64_t id = stoi(data);
			Manager::find_game(id)->accept_draw(*c->username);
			return "";
		}
		case DECLINE_DRAW:
		{
			int64_t id = stoi(data);
			Manager::find_game(id)->decline_draw(*c->username);
			return "DRAW_DECLINED";
		}	
		case RESIGN:
		{	
			int64_t id = stoi(data);
			Manager::find_game(id)->resign(*c->username);
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
				return Frame::prepare_message(2,string("ERROR"),string("Game Not Found"));
			}
			game->send_all(c->sd);
			return "";
		}

		case CHESS_MESSAGE:
		{	
			vector<string>* msg_data = c_explode(DATA_SEP,data);
			if(msg_data->size()<2){
				delete msg_data;
				return "ERROR";
			}
			int64_t id = stoi(msg_data->at(0));
			Chess* game = Manager::find_game(id);
			if(game == nullptr){
				delete msg_data;
				return Frame::prepare_message(2,"ERROR","Game Not Found");
			}
			game->message(*c->username,msg_data->at(1));
			delete msg_data;
			return "";
		}

		case PROMOTE:
		{
			
			vector<string>* msg_data = c_explode(DATA_SEP,data);
			if(msg_data->size()<2){
				return "ERROR";
			}
			int64_t id = stoi(msg_data->at(0));
			Chess* game = Manager::find_game(id);
			if(game == nullptr){
				delete msg_data;
				return Frame::prepare_message(2,string("ERROR"),string("Game Not Found"));
			}
			char piece = msg_data->at(1)[0];
			game->promote(piece,c->username);
			return "PROMOTED";
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
	uint8_t offset = 1;
	cout<<"Move data is: "<<data->at(offset)<<" "<<data->at(offset+1)<<" "<<data->at(offset+2)<<" "<<data->at(offset+3)<<endl;
	out[0] = data->at(offset)[0] - 48;
	out[1] = data->at(offset+1)[0] - 48;
	out[2] = data->at(offset+2)[0] - 48;
	out[3] = data->at(offset+3)[0] - 48;
	return out;
}


