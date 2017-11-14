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

void Chess::init(){
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",this->game->id);
	if(res->next()){
		return;
	}
	delete res;
	string turn = (this->game->turn == WHITE) ? "w" : "b";
	conn->execute("sssssssis","INSERT INTO chessgame (Turn,WTime,BTime,White,Black,Winner,Type,I,Board) values (?,?,?,?,?,?,?,?,?)",
		turn,
		Chess::format_time(this->game->white_time),
		Chess::format_time(this->game->black_time),
		*this->game->white->username,
		*this->game->black->username,
		"n",
		"Regular",
		(int)this->game->inc,
		this->game->board->generateFEN();
		);
	delete conn;
}

void Chess::start(){
	this->init();
	string msg = "GAME_START";
	msg += COMMAND;
	msg += ltos(this->game->id);
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::notify_sides(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = Frame::prepare_message(2,"SIDE","w");
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->clear();
	msg = msg = Frame::prepare_message(2,"SIDE","b");
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::notify_turn(){
	string msg = "TURN";
	msg += COMMAND;
	msg += (char)this->game->turn;
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
	string res = Frame::prepare_message(2,"CHAT",this->chat->get_last());
	frame->add((uint8_t*)res.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_board(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = Frame::prepare_message(2,"BOARD",this->game->board->generateFEN());
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_time(){
	string wtime = Chess::format_time(this->game->white_time);
	string btime = Chess::format_time(this->game->black_time);
	btime += ":";
	if(bsec<10){
		btime += "0";	
	}
	btime += itoa(bsec);
	string msg = Frame::prepare_message(3,"TIME",wtime,btime);
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd());
	frame->send(this->game->black->sd());
	delete frame;
}

void Chess::send_move(string move){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = Frame::prepare_message(2,"MOVE",move);
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
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "INVALID_MOVE";
	frame->add((uint8_t*)msg.c_str());
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

void Chess::save(){
	SQLConn* conn = new SQLConn("chessClub");
	conn->execute("ssissssisssis","UPDATE chessgame SET "
									  "WTime = ? "
									  "BTime = ? "
									  "turns = ? "
									  "WEP = ? "
									  "BEP = ? "
									  "Castle = ? "
									  "Last_Taken = ? "
									  "Promote = ? "
									  "White_Captured = ? "
									  "Black_Captured = ? "
									  "Board = ? "
									  "moves = ? "
									  "Past = ? ",
		Chess::format_time(this->game->white_time),
		Chess::format_time(this->game->black_time),
		this->game->turns,
		"false",
		"false",
		this->game->board->getCastleData(),
		"X",
		this->waiting_for_promotion ? 1 : 0,
		"",
		"",
		this->game->board->generateFEN();
		"",
		*this->history->back()
	);

}

static string format_time(time_t seconds){
	int sec = seconds % 60;
	int min = (int)(seconds / 60);
	string out = itoa(min);
	out += ":";
	if(sec<10){
		out += "0";	
	}
	out += itoa(sec);
	return out;
}