#include "chess/chess.h"
using namespace std;

static const char columns[8] = {'a','b','c','d','e','f','g'};

chess::chess(const game& g) : chess_game(g),chat(),last(0),waiting_for_promotion(false),winner(0)
{
	this->chat->connect(this->chess_game.black,user_entry::new_key(this->chess_game.id));
	this->chat->connect(this->chess_game.white,user_entry::new_key(this->chess_game.id));//CSMA is applicable for lan
}

bool chess::has_game(int64_t id)
{
	return this->game->id == id;
}

void chess::init()
{
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",this->game->id);
	if(res->next()){
		return;
	}
	delete res;
	string turn = (this->game->timer->get_turn() == WHITE) ? "w" : "b";
	conn->execute("siissssisl","INSERT INTO chessgame (Turn,WTime,BTime,White,Black,Winner,Type,I,Board,ID) values (?,?,?,?,?,?,?,?,?,?)",
		turn,
		this->chess_game.timer.get_white_time(),
		this->chess_game.timer.get_black_time(),
		this->chess_game.white.username,
		this->chess_game.black.username,
		string("n"),
		string("Regular"),
		this->chess_game.timer.get_increment(),
		this->chess_game.board.generate_FEN(),
		this->chess_game.id
	);
	delete conn;
}

void chess::start()
{
	this->init();
	string msg = "GAME_START";
	msg += COMMAND;
	msg += to_string(this->chess_game.id);
	frame f(1,0,0,0,0,TEXT);
	f.add((uint8_t*)msg.c_str());
	f.send(this->chess_game.white.sd("LOBBY"));
	f.send(this->chess_game.black.sd("LOBBY"));
}

void chess::notify_sides()
{
	frame f(1,0,0,0,0,TEXT);
	string msg = frame::prepare_message(2,string("SIDE"),string("w"));
	f.add((uint8_t*)msg.c_str());
	f.send(this->chess_game.white.sd(this->chess_game.id));
	f.clear();
	msg = frame::prepare_message(2,string("SIDE"),string("b"));
	f.add((uint8_t*)msg.c_str());
	f.send(this->game->black->sd(this->game->id));
}

void chess::notify_turn(){
	string msg = "TURN";
	msg += COMMAND;
	msg += (char)this->game->timer->get_turn();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void chess::notify_game_over(){
	string msg;
	switch(this->winner){
		case DRAW:
			msg = "DRAW";

		case WHITE:
			msg = Frame::prepare_message(2,string("WINNER"),string("WHITE"));
			break;
		case BLACK:
			msg = Frame::prepare_message(2,string("WINNER"),string("BLACK"));
			break;
		default:
			return;
	}
	this->broadcast(msg);
}

void chess::move(int r,int c, int r2, int c2,string user){
	char side;

	if(this->game->black->username->compare(user) == 0){
		side = BLACK;
	}else{
		side = WHITE;
	}

	if(this->waiting_for_promotion || side != this->game->timer->get_turn() ){
		return;
	}

	this->game->history->add_past(this->game->board->to_string());

	uint8_t result = this->game->move(r,c,r2,c2,side);
	string check_marker = "";
	if(this->game->timer->get_timeout()){
		this->winner = Board::otherSide(side);
		this->notify_game_over();
	}
	if(this->game->inCheck(side)){
		result = FALSE;
	}else if(this->game->inCheck(Board::otherSide(side))){
		check_marker = "+";
	}
	
	switch(result){
		case FALSE:
		{
			this->game->history->remove_last_past();
			this->invalid_move();
			return;
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
			return;
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
			mv += check_marker;
			this->next(mv);
			break;
		}
	}
	if(this->game->isCheckmate(Board::otherSide(side))){
		this->winner = side;
		this->notify_game_over();
	}
	if(this->game->isDraw()){
		this->winner = DRAW;
		this->notify_game_over();
	}
}

void chess::next(string mv){
	if(this->game->board->taken != 'X'){
		if(this->game->timer->get_turn() == WHITE){
			this->game->history->add_white_taken(this->game->board->taken);
		}else{
			this->game->history->add_black_taken(this->game->board->taken);
		}
	} 
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
	this->send_taken();
	this->save();
}


void chess::offer_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_OFFERED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void chess::accept_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_ACCEPTED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void chess::decline_draw(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "DRAW_DECLINED";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void chess::resign(string user){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	int sd = (this->game->white->username->compare(user) != 0)? this->game->white->sd() : this->game->black->sd();
	string msg = "RESIGN";
	frame->add((uint8_t*)msg.c_str());
	frame->send(sd);
	delete frame;
}

void chess::take_back(string user){
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

void chess::message(string user,string msg){
	this->chat->add(user,msg);
	this->broadcast(Frame::prepare_message(2,string("CHAT"),this->chat->get_last()));
}

void chess::send_board(){
	string msg = Frame::prepare_message(2,string("BOARD"),this->game->board->generateFEN());
	this->broadcast(msg);
}

void chess::send_time(){
	string msg = Frame::prepare_message(3,string("TIME"),Timer::format_time(this->game->timer->get_white_time()),
											Timer::format_time(this->game->timer->get_black_time()));
	this->broadcast(msg);
}

void chess::send_move(string move){
	this->broadcast(Frame::prepare_message(2,string("MOVE"),move));
}

void chess::send_moves(){
	if(this->game->history->has_moves()>0){
		this->broadcast(Frame::prepare_message(2,string("MOVES_ALL"),this->game->history->get_moves()));
	}
}

void chess::send_taken(){
	if(this->game->history->has_white_taken()){
		this->broadcast(Frame::prepare_message(2,string("WHITE_TAKEN_ALL"),this->game->history->get_white_taken()));
	}
	if(this->game->history->has_black_taken()){
		this->broadcast(Frame::prepare_message(2,string("BLACK_TAKEN_ALL"),this->game->history->get_black_taken()));
	}
}

void chess::send_taken(uint8_t side,char piece){
	string msg = Frame::prepare_message(2,(side == WHITE)? string("WHITE_TAKEN") : string("BLACK TAKEN"),piece);
	this->broadcast(msg);
}


void chess::send_promotion(){
	if(!this->waiting_for_promotion){
		return;
	}
	string msg = Frame::prepare_message(3,string("PROMOTION"),itoa(this->promotion_row),itoa(this->promotion_col));
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	frame->send((this->game->timer->get_turn() == WHITE)? this->game->white->sd(this->game->id) : this->game->black->sd(this->game->id));
	delete frame;
}

void chess::send_all(int sd){
	this->game->timer->update();
	this->notify_sides();
	this->chat->send_all(sd);
	this->send_board();
	this->notify_turn();
	this->send_time();
	this->send_taken();
	this->send_moves();
}

void chess::broadcast(string msg){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void chess::broadcast(Frame* frame){
	frame->send(this->game->white->sd(this->game->id));
	frame->send(this->game->black->sd(this->game->id));
}

void chess::invalid_move(){
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	string msg = "INVALID_MOVE";
	frame->add((uint8_t*)msg.c_str());
	int sd = (this->game->timer->get_turn() == WHITE) ? this->game->white->sd(this->game->id) : this->game->black->sd(this->game->id);
	frame->send(sd);
	delete frame;
}

void chess::promote(char piece,string username){
	if(!this->waiting_for_promotion){
		return;
	}
	this->game->board->forceChange(promotion_row,promotion_col,piece);
	this->next();
	this->waiting_for_promotion = false;
}

void chess::save(){
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

char chess::get_side_of(string username){
	if(this->game->white->username->compare(username) == 0){
		return WHITE;
	}
	return BLACK;
}	