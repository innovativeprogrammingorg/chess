#include "chess/game.h"

using namespace std;

user black;
user white;
board game_board;
timer game_timer;
int64_t id;
history hist;

game::game(user* black, user* white, int64_t id, board* b,uint8_t turn,time_t duration,int inc) : 
	black(*block),white(*white),game_board(*b),game_timer(duration,duration,inc,turn),id(id){}

game::game(user* black, user* white, int64_t id, board* b,uint8_t turn,int wtime,int btime,int last,int inc,int undo) :
	black(*block),white(*white),game_board(*b),game_timer(wtime,btime,inc,turn,last,undo),id(id){}

game::game(user* black, user* white, int64_t id, board* b,timer* t,history* hist) :
	black(*block),white(*white),game_board(*b),game_timer(*it),id(id), hist(*hist){}

bool game::isDraw(){
	int white_pieces = 0;
	int black_pieces = 0;
	tile t;
	piece king;
	location king_loc;
	bool has_pawn = false;
	vector<location> moves;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->game_board.get_tile(i,j);
			if(t.empty()){
				continue;
			}
			if(t.p.is(QUEEN) ||t.p.is(ROOK)){
				return false;
			}
			if(t.p.is(PAWN)){
				has_pawn = true;
			}
			if(t.p.side == WHITE){
				wPieces++;
			}else{
				bPieces++;
			}
	}
	if(wPieces < 3 && bPieces < 3 && !has_pawn){
		return true;
	}
	if(wPieces==1 && !this->in_check(WHITE)){
		king = this->game_board.get_king(WHITE);
		king_loc = this->game_board.find_king(WHITE);
		moves = king_loc.get_adjacent();
		for(auto move : moves){
			if(Move::valid(king,king_loc->row,king_loc->col,move->row,move->col,this->game_board)){
				return false;
			}
		}
		return true;
	}
	if(bPieces==1 && !this->in_check(BLACK)){
		king = this->game_board.get_king(BLACK);
		king_loc = this->game_board.find_king(BLACK);
		moves = king_loc->get_adjacent();
		for(auto move : moves){
			if(Move::valid(king,king_loc->row,king_loc->col,move->row,move->col,this->game_board)){
				return false;
			}
		}
		return true;
	}
	if(this->history.turns < 12){
		return false;
	}
	vector<string> last_moves = this->history->get_last_n_moves(12);
	bool three_move_draw = true;
	for(int i = 0;i<4;i++){
		if(last_moves.at(i).compare(last_moves.at(i + 4)) != 0 || last_moves.at(i + 4).compare(last_moves.at(i + 8)) != 0){
			three_move_draw = false;
			break;
		}
	}
	return three_move_draw;
}

bool game::is_checkmate(char side){
	if(!this->in_check(side))
		return false;
	
	location king_loc = this->game_board.find_king(side);
	
	vector<location> locations;
	vector<piece> pieces;
	{
		vector<location> spots = king_loc.get_adjacent();//will never be null
		//checks to see if the king can move to any square
		for(auto spot : spots){
			if(Move::valid(king,king_loc.row,king_loc.col,spot->row,spot->col,this->game_board)){
				return false;
			}
		}
	}
	//fetch the pieces threatening the king
	for(int i = 1;i < 9;i++)
		for(int j = 1;j < 9;j++){
			tile t = this->game_board.get_tile(i,j);
			if(!t->empty() && t.p.side != side && Move::valid(t.p,i,j,king_loc.row,king_loc.col,this->game_board)){
				pieces.push_back(t->p);
				locations.push_back(location(i,j));
			}
	}

	if(pieces.size() == 2){
		if(pieces.at(0).is(KNIGHT) || pieces.at(1).is(KNIGHT)){
			return true;
		}
		vector<location> v1 = location::locations_between(king_loc.row,king_loc.col,locations.at(0).row,locations.at(0).col);
		vector<location> v2 = location::locations_between(king_loc.row,king_loc.col,locations.at(1).row,locations.at(1).col);
		if(v1.empty() || v2.empty()){
			return true;
		}

		for(auto it = v1.begin();it != v1.end();it++)
			for(auto jt = v2.begin();jt != v2.end();jt++){
				if(it->row == jt->row && it->col == jt->col){
					if(game::tile_covered(this->game_board,jt->row,jt->col,side)){
						return false;
					}
				}
		}
		return true;
		
	}else{
		piece p = pieces.at(0);
		vector<Location> block_spots = location::locations_between(king_loc.row,king_loc.col,locations.at(0).row,locations.at(0).col);
		
		for(auto spot :block_spots){
			if(game::tileCovered(this->game_board,spot->row,spot->col,side)){
				return false;
			}
		}
		return true;
	}
	return false;
}


uint8_t game::move(int r,int c,int r2,int c2,char side){ 
		if(!location::validate(r,c) || !location::validate(r2,c2)){
			return FALSE;
		}
		if(r == r2 && c == c2){
			return FALSE;
		}

		if(this->game_board.get_tile(r,c) == nullptr){
			cerr<<"game::move: tile at ("<<r<<","<<c<<")"<<" is null"<<endl;
			return FALSE;
		}
		Piece* p = this->game_board.get_tile(r,c)->p;
		if(p == nullptr||p->side != side){
			cout<<"Unable to find piece to move"<<endl;
			Location::debug(r,c);
			return FALSE;
		}
		this->game_board.taken = 'X';
		
		/**Check if it is a special move**/
		if(!Move::valid(p,r,c,r2,c2,this->game_board)){
			cout<<"IT is not a valid move!\n";
			if(p->is(PAWN) && p->canSpecial()){
				char t = Move::specialMove(p,r,c,r2,c2,this->game_board);
				if(t == 0){
					return FALSE;
				}
				this->game_board.taken = t;
			}
			return FALSE;
		}

		if(!this->game_board.get_tile(r2,c2)->empty()){
			this->game_board.taken = this->game_board.get_tile(r2,c2)->p->FEN;
		}
		if(p->is(KING) && abs(c-c2) ==2){
			if(p->side==WHITE){
				if(c-c2==2 && (this->game_board.white_castle & 1)){
					this->game_board.white_castle = FALSE;
					Move::castle(this->game_board,QUEEN_SIDE,side);
					return QUEEN_CASTLE;//f
				}
				if(c2-c==2 && ((this->game_board.white_castle >> 1) & 1)){
					this->game_board.white_castle = FALSE;
					Move::castle(this->game_board,KING_SIDE,side);
					return KING_CASTLE;
				}
				return FALSE;
			}else{
				if(c-c2==2 && (this->game_board.black_castle & 1)){
					this->game_board.black_castle = FALSE;
					Move::castle(this->game_board,QUEEN_SIDE,side);
					return QUEEN_CASTLE;
				}
				if(c2-c==2 && ((this->game_board.black_castle >> 1) & 1)){
					this->game_board.black_castle = FALSE;
					Move::castle(this->game_board,KING_SIDE,side);
					return KING_CASTLE;
				}				
				return FALSE;
			}
		}
		this->game_board.forceMove(r,c,r2,c2);
		this->game_board.special = "false";
		if(p->is(PAWN) && abs(r-r2)==2){
			cout<<"possible special pawn move\n";
			if(!this->game_board.empty(r2,c2+1) && this->game_board.get_tile(r2,c2+1)->p->is(PAWN) && this->game_board.get_tile(r2,c2+1)->p->side != p->side){
				this->game_board.special = to_string(r2) + to_string(c2+1) + "r";
			}
			if(!this->game_board.empty(r2,c2-1) && this->game_board.get_tile(r2,c2-1)->p->is(PAWN) && this->game_board.get_tile(r2,c2-1)->p->side != p->side){
				if(this->game_board.special.compare("false") != 0){
					this->game_board.special = to_string(r2) + to_string(c2-1) + "r";
				}else{
					this->game_board.special = this->game_board.special + "|" + to_string(r2) +to_string(c2-1) + "r";
				}
			}
		}
		if(this->inCheck(side)){
			cout<<"The king is in check\n";
			return FALSE;
		}else{
			cout<<"The king is not in check\n";
		}
		if(p->is(KING)){
			if(p->side == WHITE){
				this->game_board.white_castle = FALSE;
			}else{
				this->game_board.black_castle = FALSE;
			}
		}
		if((p->FEN == WHITE_PAWN && r2==8) || (p->FEN==BLACK_PAWN && r2==1)){
			this->game_board.forceChange(r,c,'X');
			return PROMOTION;
		}
		return TRUE;
	}

bool game::in_check(char side){
	return game::inCheck(this->game_board,side);
}

bool game::in_check(const board& b,char side){
	location loc = b.find_king(side);
	int r = loc.row;
	int c = loc.col;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			tile t = b.get_tile(i,j);
			if(!t.empty() && t.p.side != side && !t.p.is(KING) && Move::valid(t.p,i,j,r,c,b)){
				return true;
			}
	}
	return false;
}

bool game::is_protected(board b,int r,int c,char side){
	for(int i = 1;i < 9;i++) 
		for(int j = 1;j < 9;j++){

			tile t = b.get_tile(i,j);
			if(!t.empty() && t.p.side == side && !(i == r && j == c)){
				char FEN = b.get_tile(i,j).p.FEN;
				b.forceChange(r,c,EMPTY_SPACE);
				if(Move::valid(t.p,i,j,r,c,b)){
					b.force_change(r,c,FEN);
					return true;
				}

				b.force_change(r,c,FEN);
			}
	}
	return false;
}

bool game::is_protected(const board& b,const location& loc,char side){
	return game::isProtected(b,loc.row,loc.col,side);
}


bool game::tile_covered(const board& b,int r,int c,char side){

	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			tile t = b.get_tile(i,j);
			if(!t.empty() && t.p.side == side && !t.p.is(KING) && Move::valid(t.p,i,j,r,c,b)){
				cout << "The tile (%d,%d) is covered by %c at (%d,%d)\n",r,c,t.p.FEN,i,j<<endl;
				return true;
			}
		}
	return false;
}

bool game::tile_covered(const board& b,const location& loc,char side){
	return game::tile_covered(b,loc.row,loc.col,side);
}
