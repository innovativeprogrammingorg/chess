#include "lobby_game.h"

using namespace std;

/**
 * In the future, this should be updated from the database
 */
int64_t Lobby_Game::game_id = 0;


Lobby_Game::Lobby_Game(int type,int sec,int min,int inc,string host){
	this->type = type;
	this->sec = sec;
	this->min = min;
	this->inc = inc;
	this->host = host;
	this->id = Lobby_Game::game_id++;
	this->players = new vector<User*>();
	this->ready = 0;
	switch(type){
		case REGULAR:
			this->seats = 2;
			break;
		case BUG_HOUSE:
			this->seats = 4;
			break;
		default:
			this->seats = 2;
			break;
	}
}

Lobby_Game::~Lobby_Game(){
	delete this->players;
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

bool Lobby_Game::is_ready(){
	return this->ready == this->seats;
}

int Lobby_Game::spots_open(){
	return this->seats - this->ready;
}

char Lobby_Game::get_open_side(){
	auto it = this->players->begin();
	switch((*it)->side){
		case 'w':
		case 'W':
			return 'b';
		case 'r':
		case 'R':
			return 'r';
		case 'b':
		case 'B':
			return 'w';
	}
	return 'r';
}

void Lobby_Game::add_player(User* player){
	this->players->push_back(player);
	this->ready++;
}

void Lobby_Game::add_player(string username){
	if(this->spots_open() != 1){
		return;
	}
	User* u = new User(username);
	u->side = this->get_open_side();
	this->add_player(u);

}

void Lobby_Game::drop_player(string user){
	if(this->players->empty()){
		return;
	}
	for(auto it = this->players->begin();it != this->players->end();it++){
		if((*it)->username->compare(user) == 0){
			this->players->erase(it);
			return;
		}
	}
}

string Lobby_Game::to_string(char sep){
	string out = "";
	out += this->get_type();//0
	out += sep;
	out += itoa(this->sec);//1
	out += sep;
	out += itoa(this->min);//2
	out += sep;
	out += itoa(this->inc);//3
	out += sep;
	out += this->get_open_side();//4
	out += sep;
	out += this->host;//5
	out += sep;
	out += ltos(this->id);//6
	out += sep;
	out += itoa(this->ready);//7
	return out;
}

Game* Lobby_Game::create_game(){
	string fen = "rnbqkbnr/pppppppp/XXXXXXXX/XXXXXXXX/XXXXXXXX/XXXXXXXX/PPPPPPPP/RNBQKBNR";
	User* white;
	User* black;
	switch(this->players->at(0)->side){
		case 'w':
		case 'W':
		{
			white = this->players->at(0);
			black = this->players->at(1);
		}
		case 'b':
		case 'B':
		{
			white = this->players->at(1);
			black = this->players->at(0);
		}
		case 'r':
		case 'R':
		{
			srand(time(NULL));
			if(rand()%2 == 0){
				white = this->players->at(0);
				black = this->players->at(1);
			}else{
				white = this->players->at(1);
				black = this->players->at(0);
			}
 		}
	}
	
	Board* board = new Board(fen,"", "wkwqbkbq");
	Game* out = new Game(black, white, this->id, board, WHITE,this->min * 60 + this->sec,this->inc);
	return out;
}	
