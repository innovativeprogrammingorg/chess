#include "chess.h"
using namespace std;

static char rows[8] = {'a','b','c','d','e','f','g'};

Chess::Chess(Game* game){
	this->game = game;
	this->last = 0;
	this->chat = new Chat();
	this->chat->connect(this->game->black);
	this->chat->connect(this->game->white);
	this->history = new vector<string>();
	this->waiting_for_promotion = false;
	this->moves = new vector<string>();
	
}
Chess::~Chess(){
	delete this->game;
	delete this->chat;
	delete this->history;
	delete this->moves;
}

void Chess::start(){
	string msg = "GAME_START";
	msg += COMMAND;
	msg += ltos(this->game->id);
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::move(int r,int c, int r2, int c2,char side){
	time_t hold = time(NULL);
	this->store(this->game->board);
	uint8_t result = this->game->move(r,c,r2,c2,side);
	switch(result){
		case FALSE:
		{
			this->history->pop_back();
			invalid_move();
			break;
		}
		case PROMOTION:
		{
			this->waiting_for_promotion = true;
			promotion_row = r2;
			promotion_col = c2;
			break;
		}
		case CASTLE:
		case TRUE:
		{
			char fen = this->game->board->getTile(r2,c2)->p->FEN;
			string mv = "";
			mv += fen;
			mv += rows[r2-1];
			mv += itoa(c2);
			this->send_move(mv);
			this->send_board();
			break;
		}
	}
}

void Chess::offer_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_OFFERED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void Chess::accept_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_ACCEPTED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void Chess::decline_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_DECLINED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void Chess::resign(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "RESIGN";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void Chess::take_back(string user){
	if(this->history->size()==0){
		Frame* frame = new Frame(1,0,0,0,0,TEXT);
		string msg = "NA";
		frame->add((uint8_t*)msg.c_str());
		int sd = (this->game->white->username->compare(user) == 0) ? this->game->white->sd() : this->game->black->sd();
		frame->send(sd);
		delete frame;
		return;
	}
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "TAKEBACK_REQUESTED";
	frame->add((uint8_t*)msg.c_str());
	int sd = (this->game->white->username->compare(user) != 0) ? this->game->white->sd() : this->game->black->sd();
	frame->send(sd);
	delete frame;
}

void Chess::store(Board* board){
	this->history->push_back(board->to_string());
}

void Chess::message(string user,string msg){
	this->chat->add(user,msg);
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string res = "CHAT";
	res += COMMAND;
	res += this->chat->get_last();
	frame->add((uint8_t*)res.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_board(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "BOARD";
	msg += COMMAND;
	msg += this->game->board->generateFEN();
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_time(){
	/**
	 * For when time is implemented
	 */
}

void Chess::send_move(string move){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "MOVE";
	msg += COMMAND;
	msg += move;
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_moves(){
	string msg = "MOVES_ALL";
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	msg += COMMAND;
	if(this->moves->size()==0){
		msg += "none";
	}else{
		for(auto it = this->moves->begin();it!=this->moves->end();it++){
			if(it != this->moves->begin()){
				msg += DATA_SEP;
			}
			msg += *it;
		}
	}
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_all(){

}

void Chess::invalid_move(){
	Frame* frame = new Frame();
	string msg = "INVALID_MOVE";
	frame->add((uint8_t*)msg.c_str());
	frame->fin = 1;
	frame->mask = 0;
	frame->opcode = TEXT;
	int sd = (this->game->turn == WHITE) ? this->game->white->sd() : this->game->black->sd();
	frame->send(sd);
	delete frame;
}

void Chess::promote(char piece){
	if(!this->waiting_for_promotion){
		return;
	}
	this->game->board->forceChange(promotion_row,promotion_col,piece);

}