#include "move.h"

using namespace std;
bool Move::valid_queen(Piece* p,Move* move,Board* b){
	return Move::valid_queen(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_queen(Piece* p,int r,int c,int mr,int mc,Board* b){
	if(((mc != c && mr !=r) && abs((r-mr)/(c-mc))!=1)||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		return false;
	}

	vector<unique_ptr<Location>>* spaces = Location::locationsBetween(r,c,mr,mc);
	if(spaces == nullptr){
		return true;
	}
	for(auto it = spaces->begin();it != spaces->end(); it++){
		if(!b->getTile((*it)->row,(*it)->col)->empty()){
			delete spaces;
			return false;
		}
	}
	delete spaces;
	return true;
}

bool Move::valid_king(Piece* p, Move* move, Board* b){
	return Move::valid_king(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_king(Piece* p,int r,int c,int mr,int mc, Board* b){
	int t1,t2;
	char FEN;
	printf("Checking if (%d,%d)->(%d,%d) is a valid move for the king\n",r,c,mr,mc);
	if(mr>8||mr<1||mc>8||mc<1){
		cout<<"The move is not on the board\n";
		return false;
	}
	if(abs(c-mc) == 2){
		if(c>mc && !Game::tileCovered(b,r,c-1,Board::otherSide(p->side)) && !Game::tileCovered(b,r,c-2,Board::otherSide(p->side)) && !Game::inCheck(b,p->side)){
			return true;
		}
		if(c<mc && !Game::tileCovered(b,r,c+1,Board::otherSide(p->side)) && !Game::tileCovered(b,r,c+2,Board::otherSide(p->side)) && !Game::inCheck(b,p->side)){
			return false;
		}
		cout<<"The move is a failed attempt to castle\n";
		return false;
	}
	if(abs(r-mr) + abs(c-mc) == 1 && !Game::tileCovered(b,mr,mc,Board::otherSide(p->side))){
		FEN = p->FEN;
		b->forceChange(r,c,'X');
		if(Game::tileCovered(b,mr,mc,Board::otherSide(p->side))){
			b->forceChange(r,c,FEN);
			return false;
		}
		b->forceChange(r,c,FEN);
		if(b->getTile(mr,mc)->empty()){
			cout<<"The move is valid since there are no pieces there and it is safe\n";
			return true;
		}else if(b->getTile(mr,mc)->p->side != p->side){
			if(Game::isProtected(b,mr,mc,Board::otherSide(p->side))){
				cout<<"The king can't take the piece\n";
				return false;
			}
			cout<<"The king can take the piece\n";
			return true;
		}
		return false;
	}
	t1 = abs(c-mc);
	t2 = abs(r-mr);
	if(t1 > 1 || t2 > 1){
		cout<<"The move is illegal for the king to make\n";
		return false;
	}
	if(t1==1 && t2==1){
		if(b->getTile(mr,mc)->empty()){
			cout<<"Diagonal Move\n";
			return Game::tileCovered(b,mr,mc,Board::otherSide(p->side));
		}else if(b->getTile(mr,mc)->p->side != p->side){
			cout<<"Can the king take the piece?\n";
			return Game::isProtected(b,mr,mc,Board::otherSide(p->side));		
		}else{
			return false;
		}
	}
	cout<<"The move is illegal by default\n change = ("<<t1<<","<<t2<<")"<<endl;
	return false;
}

bool Move::valid_pawn(Piece* p, Move* move,Board* b){
	return Move::valid_pawn(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_pawn(Piece* p,int r,int c,int mr,int mc,Board* b){
	cout<<"Seeing if it is a valid pawn move ("<<r<<","<<c<<")-->("<<mr<<","<<mc<<")"<<endl;
	if(mr>8||mr<1||mc>8||mc<1){
		return false;
	}
	if(mc == c){
		if(!b->getTile(mr,mc)->empty()){
			return false;
		}
		if( (p->side == WHITE && (mr-r == 1 || mr == 4)) || ((r-mr ==1 || mr == 5) && p->side == BLACK)){
			return true;
		}
		return false;
	}else if(abs(mc-c)==1 && !b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side != p->side){
		if(p->side == WHITE && mr-r == 1){
			return true;
		}
		if(p->side == BLACK && r-mr == 1){
			return true;
		}
	}
	cout<<"Move is invalid by default!\n";
	return false;
}

char Move::specialMove(Piece* p,Move* move,Board* b){
	return Move::specialMove(p,move->r1,move->c1,move->r2,move->c2,b);
}
char Move::specialMove(Piece* p,int r,int c,int mr,int mc,Board* b){
	if(p->special == RIGHT && c-mc==1){
		if(p->side == WHITE && mr-r == 1){
			b->forceChange(mr,mc,WHITE_PAWN);
			b->forceChange(r,c,EMPTY_SPACE);
			b->forceChange(mr-1,mc,EMPTY_SPACE);
			return BLACK_PAWN;
		}
		if(p->side == BLACK && r-mr == 1){
			b->forceChange(mr,mc,BLACK_PAWN);
			b->forceChange(r,c,EMPTY_SPACE);
			b->forceChange(mr+1,mc,EMPTY_SPACE);
			return WHITE_PAWN;
		}
	}
	if(p->special == LEFT && mc-c == 1){
		if(p->side==WHITE && mr-r == 1){
			b->forceChange(mr,mc,WHITE_PAWN);
			b->forceChange(r,c,EMPTY_SPACE);
			b->forceChange(mr-1,mc,EMPTY_SPACE);
			return BLACK_PAWN;
		}
		if(p->side==BLACK && r-mr==1){
			b->forceChange(mr,mc,BLACK_PAWN);
			b->forceChange(r,c,EMPTY_SPACE);
			b->forceChange(mr+1,mc,EMPTY_SPACE);
			return WHITE_PAWN;
		}
	}
	return 0;	
}


bool Move::valid_bishop(Piece* p,Move* move,Board* b){
	return Move::valid_bishop(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_bishop(Piece* p,int r,int c,int mr,int mc,Board* b){
	if((abs(r-mr) != abs(c-mc))||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		return false;
	}
	vector<unique_ptr<Location>>* spaces = Location::locationsBetween(r,c,mr,mc);
	if(spaces == nullptr){
		return true;
	}
	for(auto it = spaces->begin();it != spaces->end();it++){
		if(!b->getTile((*it)->row,(*it)->col)->empty()){
			delete spaces;
			return false;
		}
	}
	delete spaces;
	return true;
}
bool Move::valid_knight(Piece* p,Move* move,Board* b){
	return Move::valid_knight(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_knight(Piece* p,int r,int c,int mr,int mc,Board* b){
	return !((mr>8||mr<1||mc>8||mc<1)||abs((r-mr)*(c-mc))!=2||(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side==p->side));
}

bool Move::valid_rook(Piece* p,Move* move,Board* b){
	return Move::valid_rook(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid_rook(Piece* p,int r,int c,int mr,int mc,Board* b){
	if((mc != c && mr !=r)||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		return false;
	}
	vector<unique_ptr<Location>>* spaces = Location::locationsBetween(r,c,mr,mc);
	if(spaces == nullptr){
		return true;
	}
	for(auto it = spaces->begin();it != spaces->end();it++){
		if(!b->getTile((*it)->row,(*it)->col)->empty()){
			delete spaces;
			return false;
		}
	}
	delete spaces;
	return true;
}
bool Move::valid(Piece* p,Move* move,Board* b){
	return Move::valid(p,move->r1,move->c1,move->r2,move->c2,b);
}

bool Move::valid(Piece* p,int r,int c,int mr,int mc,Board* b){
	switch(p->name){
		case PAWN:
			return Move::valid_pawn(p,r,c,mr,mc,b);
		case KNIGHT:
			return Move::valid_knight(p,r,c,mr,mc,b);
		case BISHOP:
			return Move::valid_bishop(p,r,c,mr,mc,b);
		case ROOK:
			return Move::valid_rook(p,r,c,mr,mc,b);
		case QUEEN:
			return Move::valid_queen(p,r,c,mr,mc,b);
		case KING:
			return Move::valid_king(p,r,c,mr,mc,b);
	}
	return false;
}

bool Move::validMove(Piece* p,Move* move,Board* b){
	cout<<"Warning: Move::validMove is depricated. Use Move::valid instead"<<endl;
	return Move::valid(p,move,b);
}

void Move::castle(Board* b,char side,char player){
	Location* king_loc = b->findKing(player);
	int r = king_loc->row;
	int c = king_loc->col;
	delete king_loc;
	Piece* p = b->getTile(r,c)->p;
	if(side==KING_SIDE){
		b->forceChange(r,c+2,p->FEN);
		b->forceChange(r,c,EMPTY_SPACE);
		b->forceChange(r,c+1,b->getTile(r,8)->p->FEN);
		b->forceChange(r,8,EMPTY_SPACE);
		
	}else{
		b->forceChange(r,c-2,p->FEN);
		b->forceChange(r,c,EMPTY_SPACE);
		b->forceChange(r,c-1,b->getTile(r,1)->p->FEN);
		b->forceChange(r,1,'X');
	}
	return;
}
