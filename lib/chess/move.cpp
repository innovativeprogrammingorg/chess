#include "move.h"

using namespace std;


bool Move::valid_queen(Piece* p,Location* move,Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
	int length;
	Location* space;
	if(((mc != c && mr !=r) && abs((r-mr)/(c-mc))!=1)||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	vector<Location*>* spaces = Location::locationsBetween(p->loc,move);
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		delete spaces;
		return false;
	}
	if(spaces == nullptr){
		return true;
	}
	length = spaces->size();
	if(length==0){
		delete spaces;
		return false;
	}
	for(int i = 0;i<length;i++){
		space = spaces->at(i);
		if(!b->getTile(space->row,space->col)->empty()){
			delete spaces;
			return false;
		}
	}
	delete spaces;
	return true;
}

bool Move::valid_king(Piece* p, Location* move, Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
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
	if(abs(r-mr) + abs(c-mc) == 1 && !Game::tileCovered(b,move,Board::otherSide(p->side))){
		FEN = p->FEN;
		b->forceChange(r,c,'X');
		if(Game::tileCovered(b,move,Board::otherSide(p->side))){
			b->forceChange(r,c,FEN);
			return false;
		}
		b->forceChange(r,c,FEN);
		if(b->getTile(mr,mc)->empty()){
			cout<<"The move is valid since there are no pieces there and it is safe\n";
			return true;
		}else if(b->getTile(mr,mc)->p->side != p->side){
			if(Game::isProtected(move,b,Board::otherSide(p->side))){
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
			return Game::tileCovered(b,move,Board::otherSide(p->side));
		}else if(b->getTile(mr,mc)->p->side != p->side){
			cout<<"Can the king take the piece?\n";
			return Game::isProtected(move,b,Board::otherSide(p->side));		
		}else{
			return false;
		}
	}
	cout<<"The move is illegal by default\n change = ("<<t1<<","<<t2<<")"<<endl;
	return false;
}

bool Move::valid_pawn(Piece* p, Location* move,Board* b){

	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
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

char Move::specialMove(Piece* p,Location* move,Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
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

bool Move::valid_bishop(Piece* p,Location* move,Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
	int length;
	Location* l;
	if(c == mc||(abs((r-mr)/(c-mc)) != 1)||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		return false;
	}
	vector<Location*>* spaces = Location::locationsBetween(p->loc,move);
	length = spaces->size();
	if(length==0){
		return true;
	}
	for(int i = 0;i<length;i++){
		l = spaces->at(i);
		if(!b->getTile(l->row,l->col)->empty()){
			return false;
		}
	}
	return true;
}

bool Move::valid_knight(Piece* p,Location* move,Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
	return !((mr>8||mr<1||mc>8||mc<1)||abs((r-mr)*(c-mc))!=2||(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side==p->side));
}

bool Move::valid_rook(Piece* p,Location* move,Board* b){
	int r = p->loc->row;
	int c = p->loc->col;
	int mr = move->row;
	int mc = move->col;
	int length;
	Location* l;
	if((mc != c && mr !=r)||(mr>8||mr<1||mc>8||mc<1)){
		return false;
	}
	
	if(!b->getTile(mr,mc)->empty() && b->getTile(mr,mc)->p->side == p->side){
		return false;
	}
	vector<Location*>* spaces = Location::locationsBetween(p->loc,move);
	length = spaces->size();
	for(int i = 0;i<length;i++){
		l = spaces->at(i);
		if(!b->getTile(l->row,l->col)->empty()){
			delete spaces;
			return false;
		}
	}
	delete spaces;
	return true;
}

bool Move::valid(Piece* p,Location* move,Board* b){
	switch(p->name){
		case PAWN:
			return Move::valid_pawn(p,move,b);
		case KNIGHT:
			return Move::valid_knight(p,move,b);
		case BISHOP:
			return Move::valid_bishop(p,move,b);
		case ROOK:
			return Move::valid_rook(p,move,b);
		case QUEEN:
			return Move::valid_queen(p,move,b);
		case KING:
			return Move::valid_king(p,move,b);
	}
	return false;
}

bool Move::validMove(Piece* p,Location* move,Board* b){
	return Move::valid(p,move,b);
}

void Move::castle(Piece* p,Board* b,char side){
	int r = p->loc->row;
	int c = p->loc->col;
	if(side==KING_SIDE){
		b->forceChange(r,c+2,p->FEN);
		b->forceChange(r,c,EMPTY_SPACE);
		b->forceChange(r,c+1,b->getTile(r,8)->p->FEN);
		b->forceChange(r,8,EMPTY_SPACE);
		p->move(new Location(r,c+2));
		b->getTile(r,c+1)->p->move(new Location(r,c+1));
		
	}else{
		b->forceChange(r,c-2,p->FEN);
		b->forceChange(r,c,EMPTY_SPACE);
		b->forceChange(r,c-1,b->getTile(r,1)->p->FEN);
		b->forceChange(r,1,'X');
		p->move(new Location(r,c-2));
		b->getTile(r,c-1)->p->move(new Location(r,c-1));
	}
	return;
}
