#include "chess/chess.h"
using namespace std;

static const char columns[8] = {'a','b','c','d','e','f','g'};

Chess::Chess(Game* game){
	this->game = game;
	this->last = 0;
	this->chat = new Chat();
	this->chat->connect(this->game->black,User_Entry::new_key(this->game->id));
	this->chat->connect(this->game->white,User_Entry::new_key(this->game->id));
	this->waiting_for_promotion = false;
	this->winner = 0;
}

Chess::~Chess(){
	delete this->game;
	delete this->chat;
}

void Chess::init(){
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",this->game->id);
	if(res->next()){
		return;
	}
	delete res;
	string turn = (this->game->timer->get_turn() == WHITE) ? "w" : "b";
	conn->execute("siissssisl","INSERT INTO chessgame (Turn,WTime,BTime,White,Black,Winner,Type,I,Board,ID) values (?,?,?,?,?,?,?,?,?,?)",
		turn,
		this->game->timer->get_white_time(),
		this->game->timer->get_black_time(),
		*this->game->white->username,
		*this->game->black->username,
		string("n"),
		string("Regular"),
		this->game->timer->get_increment(),
		this->game->board->generateFEN(),
		this->game->id
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
	frame->send(this->game->white->sd("LOBBY"));
	frame->send(this->game->black->sd("LOBBY"));
	delete frame;
}

void Chess::notify_sides(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = Frame::prepare_message(2,string("SIDE"),string("w"));
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->white->sd(this->game->id));
	frame->clear();
	msg = msg = Frame::prepare_message(2,string("SIDE"),string("b"));
	frame->add((uint8_t*)msg.c_str());
	frame->send(this->game->black->sd(this->game->id));
	delete frame;
}

void Chess::notify_turn(){
	string msg = "TURN";
	msg += COMMAND;
	msg += (char)this->game->timer->get_turn();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void Chess::move(int r,int c, int r2, int c2,char side){
	if(this->waiting_for_promotion){
		return;
	}
	this->game->history->add_past(this->game->board->to_string());
	uint8_t result = this->game->move(r,c,r2,c2,side);
	if(this->game->inCheck(side)){
		result = FALSE;
	}
	switch(result){
		case FALSE:
		{
			this->game->history->remove_last_past();
			this->invalid_move();
			break;
		}
		case PROMOTION:
		{
			this->waiting_for_promotion = true;
			this->promotion_row = r2;
			this->promotion_col = c2;
			char fen = this->game->board->getTile(r2,c2)->p->FEN;
			string mv = "";
			mv += fen;
			mv += columns[c2-1];
			mv += itoa(r2);
			this->game->history->add_move(mv);
			this->send_promotion();
			break;
		}
		case QUEEN_CASTLE:
		{
			string mv = "o-o";
			this->next(mv);
			break;
		}
		case KING_CASTLE:
		{
			string mv = "o-o-o";
			this->next(mv);
			break;
		}
		case TRUE:
		{
			char fen = this->game->board->getTile(r2,c2)->p->FEN;
			string mv = "";
			mv += fen;
			mv += columns[8-c2];
			mv += itoa(r2);
			
			this->next(mv);
			break;
		}
	}
}

void Chess::next(string mv){
	this->send_board();
	if(mv.compare("")!= 0){
		this->game->history->add_move(mv);
		this->send_move(mv);
	}else{
		this->send_moves();
	}
	this->game->timer->next();
	this->send_time();
	this->notify_turn();
	this->save();
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
	if(!this->game->history->has_past()){
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

void Chess::message(string user,string msg){
	this->chat->add(user,msg);
	this->broadcast(Frame::prepare_message(2,string("CHAT"),this->chat->get_last()));
}

void Chess::send_board(){
	string msg = Frame::prepare_message(2,string("BOARD"),this->game->board->generateFEN());
	this->broadcast(msg);
}

void Chess::send_time(){
	string msg = Frame::prepare_message(3,string("TIME"),Timer::format_time(this->game->timer->get_white_time()),
											Timer::format_time(this->game->timer->get_black_time()));
	this->broadcast(msg);
}

void Chess::send_move(string move){
	this->broadcast(Frame::prepare_message(2,string("MOVE"),move));
}

void Chess::send_moves(){
	if(this->game->history->has_moves()>0){
		this->broadcast(Frame::prepare_message(2,string("MOVES_ALL"),this->game->history->get_moves()));
	}
}

void Chess::send_taken(){
	if(this->game->history->has_white_taken()){
		this->broadcast(Frame::prepare_message(2,string("WHITE_TAKEN_ALL"),this->game->history->get_white_taken()));
	}
	if(this->game->history->has_black_taken()){
		this->broadcast(Frame::prepare_message(2,string("BLACK_TAKEN_ALL"),this->game->history->get_black_taken()));
	}
}

void Chess::send_taken(uint8_t side,char piece){
	string msg = Frame::prepare_message(2,(side == WHITE)? string("WHITE_TAKEN") : string("BLACK TAKEN"),piece);
	this->broadcast(msg);
}

void Chess::send_promotion(){
	if(!this->waiting_for_promotion){
		return;
	}
	string msg = Frame::prepare_message(3,string("PROMOTION"),itoa(this->promotion_row),itoa(this->promotion_col));
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send((this->game->timer->get_turn() == WHITE)? this->game->white->sd(this->game->id) : this->game->black->sd(this->game->id));
	delete frame;
}

void Chess::send_all(int sd){
	this->notify_sides();
	this->chat->send_all(sd);
	this->send_board();
	this->notify_turn();
	this->send_time();
	this->send_moves();

}

void Chess::broadcast(string msg){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void Chess::broadcast(Frame* frame){
	frame->send(this->game->white->sd(this->game->id));
	frame->send(this->game->black->sd(this->game->id));
}

void Chess::invalid_move(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "INVALID_MOVE";
	frame->add((uint8_t*)msg.c_str());
	int sd = (this->game->timer->get_turn() == WHITE) ? this->game->white->sd(this->game->id) : this->game->black->sd(this->game->id);
	frame->send(sd);
	delete frame;
}

void Chess::promote(char piece){
	if(!this->waiting_for_promotion){
		return;
	}
	this->game->board->forceChange(promotion_row,promotion_col,piece);
	this->next();
	this->waiting_for_promotion = false;
}

void Chess::save(){
	SQLConn* conn = new SQLConn("chessClub");
	string taken = "";
	taken += this->game->board->taken;
	this->game->timer->update();
	conn->execute("siiiissssisssssl","UPDATE `chessgame` SET "
									  "`Turn` = ?,"
									  "`WTime` = ?, "
									  "`BTime` = ?, "
									  "`Undo_Time` = ?, "
									  "`turns` = ?, "
									  "`WEP` = ?, "
									  "`BEP` = ?, "
									  "`Castle` = ?, "
									  "`Last_Taken` = ?, "
									  "`Promote` = ?, "
									  "`White_Captured` = ?, "
									  "`Black_Captured` = ?, "
									  "`Board` = ?, "
									  "`moves` = ?, "
									  "`Past` = ? "
									  "WHERE `ID` = ?",
		(this->game->timer->get_turn() == WHITE) ? string("w") : string("b"),							  
		this->game->timer->get_white_time(),
		this->game->timer->get_black_time(),
		this->game->timer->get_undo(),
		this->game->history->turns,
		(this->game->timer->get_turn() == WHITE) ? this->game->board->special : string("false"),
		(this->game->timer->get_turn() == BLACK) ? this->game->board->special : string("false"),
		this->game->board->getCastleData(),
		taken,
		this->waiting_for_promotion ? 1 : 0,
		this->game->history->get_white_taken(),
		this->game->history->get_black_taken(),
		this->game->board->generateFEN(),
		this->game->history->get_moves(),
		this->game->history->get_past(),
		this->game->id
	);
	delete conn;
}

char Chess::get_side_of(string username){
	if(this->game->white->username->compare(username) == 0){
		return WHITE;
	}
	return BLACK;
}	