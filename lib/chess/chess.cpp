#include "chess/chess.h"
using namespace std;

static const char columns[8] = {'a','b','c','d','e','f','g'};

chess::chess(const game& g) : chess_game(g),chat(),last(0),waiting_for_promotion(false),winner(0)
{
	this->chat.connect(this->chess_game.black,user_entry::new_key(this->chess_game.id));
	this->chat.connect(this->chess_game.white,user_entry::new_key(this->chess_game.id));//CSMA is applicable for lan
}

bool chess::has_game(int64_t id)
{
	return this->game.id == id;
}

void chess::init()
{
	SQLConn* conn = new SQLConn("chessClub");
	sql::ResultSet* res = conn->fetch("i","SELECT * FROM chessgame WHERE ID = ?",this->game->id);
	if(res->next()){
		return;
	}
	delete res;
	string turn = (this->game.timer.get_turn() == WHITE) ? "w" : "b";
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
	f.send(this->game.black.sd(this->chess_game.id));
}

void chess::notify_turn()
{
	string msg = "TURN";
	msg += COMMAND;
	msg += (char)this->game->timer->get_turn();
	Frame* frame = new Frame(1,0,0,0,0,TEXT);
	frame->add((uint8_t*)msg.c_str());
	this->broadcast(frame);
	delete frame;
}

void chess::notify_game_over()
{
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

void chess::move(int r,int c, int r2, int c2,string user)
{
	char side;

	if(this->chess_game.black.username.compare(user) == 0){
		side = BLACK;
	}else{
		side = WHITE;
	}

	if(this->waiting_for_promotion || side != this->chess_game.timer.get_turn() ){
		return;//Can't move
	}

	this->chess_game.history.add_past(this->chess_game.board.to_string());

	uint8_t result = this->chess_game.move(r,c,r2,c2,side);
	string check_marker = "";
	if(this->chess_game.timer.get_timeout()){
		this->winner = board::other_side(side);
		this->notify_game_over();
	}
	if(this->chess_game.in_check(side)){
		result = FALSE;
	}else if(this->chess_game.in_check(board::other_side(side))){
		check_marker = "+";
	}
	
	switch(result){
		case FALSE:
		{
			this->chess_game.history.remove_last_past();
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
			char fen = this->chess_game.board.get_tile(r2,c2).p.FEN;
			string mv = "";
			mv += fen;
			mv += columns[8-c2];
			mv += to_string(r2);
			mv += check_marker;
			this->next(mv);
			break;
		}
	}
	if(this->chess_game.is_checkmate(board::other_side(side))){
		this->winner = side;
		this->notify_game_over();
	}
	if(this->chess_game.is_draw()){
		this->winner = DRAW;
		this->notify_game_over();
	}
}

void chess::next(const string& mv)
{
	if(this->chess_game.board.taken != 'X'){
		if(this->chess_game.timer.get_turn() == WHITE){
			this->chess_game.history.add_white_taken(this->chess_game.board.taken);
		}else{
			this->chess_game.history->add_black_taken(this->chess_game.board.taken);
		}
	} 
	this->send_board();
	if(mv.compare("")!= 0){
		this->chess_game.history.add_move(mv);
		this->send_move(mv);
	}else{
		this->send_moves();
	}
	this->chess_game.timer.next();
	this->send_time();
	this->notify_turn();
	this->send_taken();
	this->save();
}


void chess::offer_draw(const string& user)
{
	frame f(1,0,0,0,0,TEXT);
	int sd = (this->chess_game.white.username.compare(user) != 0) ? this->chess_game.white.sd() : this->chess_game->black.sd();
	string msg = "DRAW_OFFERED";
	f.add((uint8_t*)msg.c_str());
	f.send(sd);
}

void chess::accept_draw(const string& user)
{
	frame f(1,0,0,0,0,TEXT);
	int sd = (this->chess_game.white.username.compare(user) != 0)? this->chess_game.white.sd() : this->chess_game.black.sd();
	string msg = "DRAW_ACCEPTED";
	f.add((uint8_t*)msg.c_str());
	f.send(sd);
}

void chess::decline_draw(const string& user){
	frame f(1,0,0,0,0,TEXT);
	int sd = (this->chess_game.white.username.compare(user) != 0)? this->chess_game.white.sd() : this->chess_game.black.sd();
	string msg = "DRAW_DECLINED";
	f.add((uint8_t*)msg.c_str());
	f.send(sd);
}

void chess::resign(const string& user)
{
	frame f(1,0,0,0,0,TEXT);
	int sd = (this->chess_game.white.username.compare(user) != 0)? this->chess_game.white.sd() : this->chess_game.black.sd();
	string msg = "RESIGN";
	f.add((uint8_t*)msg.c_str());
	f.send(sd);
}

void chess::take_back(const string& user){
	frame f(1,0,0,0,0,TEXT);
	if(!this->chess_game.history.has_past()){
		
		string msg = "NA";
		f.add((uint8_t*)msg.c_str());
		int sd = (this->chess_game->white.username.compare(user) == 0) ? this->chess_game.white.sd() : this->chess_game.black.sd();
		frame->send(sd);
		return;
	}
	string msg = "TAKEBACK_REQUESTED";
	f.add((uint8_t*)msg.c_str());
	int sd = (this->chess_game.white.username.compare(user) != 0) ? this->chess_game.white.sd() : this->chess_game.black.sd();
	f.send(sd);
}

void chess::message(const string& user,const string& msg){
	this->chat.add(user,msg);
	this->broadcast(frame::prepare_message(2,"CHAT",this->chat.get_last()));
}

void chess::send_board(){
	const string msg = frame::prepare_message(2,"BOARD",this->chess_game.board.generate_FEN());
	this->broadcast(msg);
}

void chess::send_time(){
	const string msg = frame::prepare_message(3,"TIME",timer::format_time(this->chess_game.timer.get_white_time()),
											timer::format_time(this->chess_game.timer.get_black_time()));
	this->broadcast(msg);
}

void chess::send_move(const string& move){
	this->broadcast(Frame::prepare_message(2,"MOVE",move));
}

void chess::send_moves(){
	if(this->chess_game.history.has_moves()>0){
		this->broadcast(frame::prepare_message(2,"MOVES_ALL",this->chess_game.history.get_moves()));
	}
}

void chess::send_taken(){
	if(this->chess_game.history.has_white_taken()){
		this->broadcast(frame::prepare_message(2,"WHITE_TAKEN_ALL",this->chess_game.history.get_white_taken()));
	}
	if(this->chess_game.history.has_black_taken()){
		this->broadcast(frame::prepare_message(2,"BLACK_TAKEN_ALL",this->chess_game.history.get_black_taken()));
	}
}

void chess::send_taken(uint8_t side,char piece){
	const string msg = frame::prepare_message(2,(side == WHITE)? "WHITE_TAKEN" : "BLACK TAKEN",piece);
	this->broadcast(msg);
}


void chess::send_promotion(){
	if(!this->waiting_for_promotion){
		return;
	}
	const string msg = Frame::prepare_message(3,"PROMOTION",to_string(this->promotion_row),to_string(this->promotion_col));
	frame f(1,0,0,0,0,TEXT);
	f.add((uint8_t*)msg.c_str());
	f.send((this->chess_game.timer.get_turn() == WHITE)? this->chess_game.white.sd(this->chess_game.id) : this->chess_game.black.sd(this->chess_game.id));
	delete frame;
}

void chess::send_all(int sd){
	this->chess_game.timer.update();
	this->notify_sides();
	this->chat.send_all(sd);
	this->send_board();
	this->notify_turn();
	this->send_time();
	this->send_taken();
	this->send_moves();
}

void chess::broadcast(const string& msg){
	frame f(1,0,0,0,0,TEXT);
	f.add((uint8_t*)msg.c_str());
	this->broadcast(f);
}

void chess::broadcast(const frame& f){
	f.send(this->chess_game.white.sd(this->chess_game.id));
	f.send(this->chess_game.black.sd(this->chess_game.id));
}

void chess::invalid_move(){
	frame f(1,0,0,0,0,TEXT);
	string msg = "INVALID_MOVE";
	f.add((uint8_t*)msg.c_str());
	int sd = (this->chess_game.timer.get_turn() == WHITE) ? this->chess_game.white.sd(this->chess_game.id) : this->chess_game.black.sd(this->chess_game.id);
	f->send(sd);
}

void chess::promote(char piece,const string& username){
	if(!this->waiting_for_promotion){
		return;
	}
	this->chess_game.board.force_change(this->promotion_row,this->promotion_col,piece);
	this->next();
	this->waiting_for_promotion = false;
}

void chess::save(){
	SQLConn* conn = new SQLConn("chessClub");
	string taken = "";
	taken += this->chess_game.board.taken;
	this->chess_game.timer.update();
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
		(this->chess_game.timer.get_turn() == WHITE) ? "w" : "b",							  
		this->chess_game.timer.get_white_time(),
		this->chess_game.timer.get_black_time(),
		this->chess_game.timer.get_undo(),
		this->chess_game.history.turns,
		(this->chess_game.timer.get_turn() == WHITE) ? this->game->board->special : "false",
		(this->chess_game.timer.get_turn() == BLACK) ? this->game->board->special : "false",
		this->chess_game.board.getCastleData(),
		taken,
		this->waiting_for_promotion ? 1 : 0,
		this->chess_game.history.get_white_taken(),
		this->chess_game.history.get_black_taken(),
		this->chess_game.board.generate_FEN(),
		this->chess_game.history.get_moves(),
		this->chess_game.history.get_past(),
		this->chess_game.id
	);
	delete conn;
}

char chess::get_side_of(const string& username){
	if(this->chess_game.white.username.compare(username) == 0){
		return WHITE;
	}
	return BLACK;
}	