#include "chess/game.h"

using namespace std;


Game::Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,time_t duration,int inc){
	this->board = b;
	this->black = black;
	this->white = white;
	this->timer = new Timer(duration,duration,inc,turn);
	this->id = id;
}

Game::Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,int wtime,int btime,int last,int inc,int undo){
	this->black = black;
	this->white = white;
	this->id = id;
	this->board = b;
	this->timer = new Timer(wtime,btime,inc,turn,last,undo);
}
Game::Game(User* black, User* white, int64_t id, Board* b,Timer* t){
	this->black = black;
	this->white = white;
	this->id = id;
	this->board = b;
	this->timer = t;
}

Game::~Game(){
	delete this->board;
	delete this->black;
	delete this->white;
	delete this->timer;
}

bool Game::isDraw(){
	int wPieces = 0;
	int bPieces = 0;
	Tile* t;
	Piece* king;
	vector<Location*>* moves;
	int numMoves; 
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->board->getTile(i,j);
			if(t->empty()){
				continue;
			}
			if(t->p->is(PAWN) || t->p->is(QUEEN) ||t->p->is(ROOK)){
				return false;
			}
			if(t->p->side == WHITE){
				wPieces++;
			}else{
				bPieces++;
			}
	}
	if(wPieces<3 && bPieces<3){
		return true;
	}
	if(wPieces==1 && !this->inCheck(WHITE)){
		king = this->board->getKing(WHITE);
		moves = king->loc->getAdjacent();
		numMoves = moves->size();
		for(int i = 0;i<numMoves;i++){
			if(Move::validMove(king,moves->at(i),this->board)){
				return false;
			}
		}
		return true;
	}
	if(bPieces==1 && !this->inCheck(BLACK)){
		king = this->board->getKing(BLACK);
		moves = king->loc->getAdjacent();
		numMoves = moves->size();
		for(int i = 0;i<numMoves;i++){
			if(Move::validMove(king,moves->at(i),this->board)){
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Game::isCheckmate(char side){
	if(!this->inCheck(side))
		return false;
	
	Piece* king = this->board->getKing(side);
	vector<Location*>* moves = king->loc->getAdjacent();
	vector<Piece*> pieces;
	int l1 = 0;
	int l2 = 0;
	Location* block = nullptr;
	Location* s[2];
	Tile* t;
	int numMoves = moves->size();
	for(int i = 0;i<numMoves;i++){
		if(Move::validMove(king,moves->at(i),this->board)){
			return false;
		}
	}
	delete moves;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->board->getTile(i,j);
			if(!t->empty() && t->p->side != side && Move::validMove(t->p,king->loc,this->board)){
				pieces.push_back(t->p);
			}
	}
	if(pieces.size() == 2){
		if(pieces.at(0)->is(KNIGHT) || pieces.at(1)->is(KNIGHT)){
			return true;
		}
		vector<Location*>* v1 = Location::locationsBetween(king->loc,pieces.at(0)->loc);
		vector<Location*>* v2 = Location::locationsBetween(king->loc,pieces.at(1)->loc);
		l1 = v1->size();
		l2 = v2->size();
		
		if(l1 == 0 || l2 ==0){
			return true;
		}
		for(int i = 0;i<l1;i++){
			s[0] = v1->at(i);
			for(int j = 0;j<l2;j++){
				s[1] = v2->at(j);
				if(s[0]->equals(s[1])){
					//block = s[1];
					return true;
				}
			}
		}
		return Game::tileCovered(this->board,block,side);
	}else{
		Piece* p = pieces.at(0);
		vector<Location*>* blockSpots = Location::locationsBetween(king->loc,p->loc);
		l1 = blockSpots->size();
		for(int i = 0;i<l1;i++){
			block = blockSpots->at(i);
			if(Game::tileCovered(this->board,block,side)){
				return false;
			}
		}
		return true;
	}
	return false;
}


uint8_t Game::move(int r,int c,int r2,int c2,char side){ 
		if(r == r2 && c == c2){
			return FALSE;
		}

		if(this->board->getTile(r,c) == NULL){
			cerr<<"Game::move: tile at ("<<r<<","<<c<<")"<<" is null"<<endl;
			return FALSE;
		}

		this->board->taken = 'X';
		Piece* p = this->board->getTile(r,c)->p;
		char t;
		if(p == nullptr||p->side != side){
			return FALSE;
		}
		Location* move = new Location(r2,c2);
		/**Check if it is a special move**/
		if(!Move::validMove(p,move,this->board)){
			cout<<"IT is not a valid move!\n";
			if(p->is(PAWN) && p->canSpecial()){
				t = Move::specialMove(p,move,this->board);
				delete move;
				if(t == 0){
					return FALSE;
				}
				this->board->taken = t;
				
			}
			delete move;
			return FALSE;
		}
		delete move;

		if(!this->board->getTile(r2,c2)->empty()){
			this->board->taken = this->board->getTile(r2,c2)->p->FEN;
		}
		if(p->is(KING) && abs(c-c2) ==2){
			if(p->side==WHITE){
				if(c-c2==2 && (this->board->wCastle & 1)){
					this->board->wCastle = FALSE;
					Move::castle(p,this->board,QUEEN_SIDE);
					return CASTLE;//f
				}
				if(c2-c==2 && ((this->board->wCastle >> 1) & 1)){
					this->board->wCastle = FALSE;
					Move::castle(p,this->board,KING_SIDE);
					return CASTLE;
				}
				return FALSE;
			}else{
				if(c-c2==2 && (this->board->bCastle & 1)){
					this->board->bCastle = FALSE;
					Move::castle(p,this->board,QUEEN_SIDE);
					return CASTLE;
				}
				if(c2-c==2 && ((this->board->bCastle >> 1) & 1)){
					this->board->bCastle = FALSE;
					Move::castle(p,this->board,KING_SIDE);
					return CASTLE;//f
				}				
				return FALSE;
			}
		}
		this->board->forceChange(r2,c2,this->board->getTile(r,c)->p->FEN);
		this->board->forceChange(r,c,EMPTY_SPACE);
		this->board->special = "false";
		if(p->is(PAWN) && abs(r-r2)==2){
			cout<<"possible special pawn move\n";
			if(!this->board->getTile(r2,c2+1)->empty() && this->board->getTile(r2,c2+1)->p->is(PAWN) && this->board->getTile(r2,c2+1)->p->side != p->side){
				this->board->special = itoa(r2) + itoa(c2+1) +"r";
			}
			if(!this->board->getTile(r2,c2-1)->empty() && this->board->getTile(r2,c2-1)->p->is(PAWN) && this->board->getTile(r2,c2-1)->p->side != p->side){
				if(this->board->special.compare("false") != 0){
					this->board->special = itoa(r2) + itoa(c2-1) + "r";
				}else{
					this->board->special = this->board->special + "|" + itoa(r2) +itoa(c2-1) + "r";
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
			if(p->side==WHITE){
				this->board->wCastle = false;
			}else{
				this->board->bCastle = false;
			}
		}
		if((p->FEN==WHITE_PAWN && r2==8) || (p->FEN==BLACK_PAWN && r2==1)){
			delete this->board->getTile(r,c);
			this->board->forceChange(r,c,'X');
			return PROMOTION;
		}
		return TRUE;
	}

bool Game::inCheck(char side){
	return Game::inCheck(this->board,side);
}

bool Game::inCheck(Board* b,char side){
	Location* loc = b->findKing(side);
	Tile* t;
	int i,j;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side != side && t->p->is(KING) && Move::validMove(t->p,loc,b)){
				return true;
			}
	}
	return false;
}


bool Game::isProtected(Location* loc,Board* b,char side){
	Tile* t;
	char FEN; 
	for(int i = 1;i<9;i++) 
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side == side && !t->p->loc->equals(loc)){
					FEN = b->getTile(loc->row,loc->col)->p->FEN;
					b->forceChange(loc->row,loc->col,EMPTY_SPACE);
					if(Move::validMove(t->p,loc,b)){
						return true;
					}
					b->forceChange(loc->row,loc->col,FEN);
			}
	}
	return false;
}


bool Game::tileCovered(Board* b,Location* loc,char side){
	Tile* t;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side == side && t->p->is(KING) && Move::validMove(t->p,loc,b)){
				return true;
			}
		}
	return false;
}

bool Game::tileCovered(Board* b,int r,int c,char side){
	Location* loc = new Location(r,c);
	bool out = Game::tileCovered(b,loc,side);
	delete loc;
	return out;
}
