#include "chess/game.h"

using namespace std;

Game::Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,time_t duration,int inc){
	this->board = b;
	this->black = black;
	this->white = white;
	this->timer = new Timer(duration,duration,inc,turn);
	this->id = id;
	this->history = new History();
}

Game::Game(User* black, User* white, int64_t id, Board* b,uint8_t turn,int wtime,int btime,int last,int inc,int undo){
	this->black = black;
	this->white = white;
	this->id = id;
	this->board = b;
	this->timer = new Timer(wtime,btime,inc,turn,last,undo);
	this->history = new History();;
}
Game::Game(User* black, User* white, int64_t id, Board* b,Timer* t,History* hist){
	this->black = black;
	this->white = white;
	this->id = id;
	this->board = b;
	this->timer = t;
	this->history = hist;
}

Game::~Game(){
	delete this->board;
	delete this->black;
	delete this->white;
	delete this->timer;
	delete this->history;
}

bool Game::isDraw(){
	int wPieces = 0;
	int bPieces = 0;
	Tile* t;
	Piece* king;
	Location* king_loc;
	vector<unique_ptr<Location>>* moves;
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
		king_loc = this->board->findKing(WHITE);

		moves = king_loc->getAdjacent();

		for(auto it = moves->begin();it != moves->end(); it++){
			if(Move::valid(king,king_loc->row,king_loc->col,(*it)->row,(*it)->col,this->board)){
				delete moves;
				delete king_loc;
				return false;
			}
		}
		delete moves;
		delete king_loc;
		return true;
	}
	if(bPieces==1 && !this->inCheck(BLACK)){
		king = this->board->getKing(BLACK);
		king_loc = this->board->findKing(BLACK);
		moves = king_loc->getAdjacent();
		for(auto it = moves->begin();it != moves->end(); it++){
			if(Move::valid(king,king_loc->row,king_loc->col,(*it)->row,(*it)->col,this->board)){
				delete moves;
				delete king_loc;
				return false;
			}
		}
		delete moves;
		delete king_loc;
		return true;
	}
	/**
	 * Needs to handle more cases
	 */
	return false;
}

bool Game::isCheckmate(char side){
	if(!this->inCheck(side))
		return false;
	
	Location* king_loc = this->board->findKing(side);
	Piece* king = this->board->getTile(king_loc->row,king_loc->col)->p;
	vector<unique_ptr<Location>>* spots = king_loc->getAdjacent();//will never be null
	vector<unique_ptr<Location>> locations;
	vector<Piece*> pieces;
	Tile* t;

	//checks to see if the king can move to any square
	for(auto it = spots->begin();it != spots->end();it++){
		if(Move::valid(king,king_loc->row,king_loc->col,(*it)->row,(*it)->col,this->board)){
			delete king_loc;
			return false;
		}
	}
	delete spots;

	//fetch the pieces threatening the king
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->board->getTile(i,j);
			if(!t->empty() && t->p->side != side && Move::valid(t->p,i,j,king_loc->row,king_loc->col,this->board)){
				pieces.push_back(t->p);
				locations.push_back(make_unique<Location>(i,j));
			}
	}
	if(pieces.size() == 2){
		if(pieces.at(0)->is(KNIGHT) || pieces.at(1)->is(KNIGHT)){
			delete king_loc;
			return true;
		}
		vector<unique_ptr<Location>>* v1 = Location::locationsBetween(king_loc->row,king_loc->col,locations.at(0)->row,locations.at(0)->col);
		vector<unique_ptr<Location>>* v2 = Location::locationsBetween(king_loc->row,king_loc->col,locations.at(1)->row,locations.at(1)->col);
		if(v1 == nullptr || v2 == nullptr){
			delete king_loc;
			if(v1 != nullptr){
				delete v1;
			}else if(v2 != nullptr){
				delete v2;
			}
			return true;
		}

		for(auto it = v1->begin();it != v1->end();it++)
			for(auto jt = v2->begin();jt != v2->end();jt++){
				if((*it)->row == (*jt)->row && (*it)->col == (*jt)->col){
					if(Game::tileCovered(this->board,(*jt)->row,(*jt)->col,side)){
						delete king_loc;
						delete v1;
						delete v2;
						return false;
					}
				}
		}
		delete v1;
		delete v2;
		delete king_loc;
		return true;
		
	}else{
		Piece* p = pieces.at(0);
		vector<unique_ptr<Location>>* blockSpots = Location::locationsBetween(king_loc->row,king_loc->col,locations.at(0)->row,locations.at(0)->col);
		delete king_loc;
		if(blockSpots == nullptr){
			return true;
		}
		
		for(auto it = blockSpots->begin();it != blockSpots->end();it++){
			if(Game::tileCovered(this->board,(*it)->row,(*it)->col,side)){
				delete blockSpots;
				return false;
			}
		}
		delete blockSpots;
		return true;
	}
	return false;
}


uint8_t Game::move(int r,int c,int r2,int c2,char side){ 
		if(!Location::validate(r,c) || !Location::validate(r2,c2)){
			return FALSE;
		}
		if(r == r2 && c == c2){
			return FALSE;
		}

		if(this->board->getTile(r,c) == nullptr){
			cerr<<"Game::move: tile at ("<<r<<","<<c<<")"<<" is null"<<endl;
			return FALSE;
		}
		Piece* p = this->board->getTile(r,c)->p;
		if(p == nullptr||p->side != side){
			cout<<"Unable to find piece to move"<<endl;
			Location::debug(r,c);
			return FALSE;
		}
		this->board->taken = 'X';
		
		/**Check if it is a special move**/
		if(!Move::valid(p,r,c,r2,c2,this->board)){
			cout<<"IT is not a valid move!\n";
			if(p->is(PAWN) && p->canSpecial()){
				char t = Move::specialMove(p,r,c,r2,c2,this->board);
				if(t == 0){
					return FALSE;
				}
				this->board->taken = t;
			}
			return FALSE;
		}

		if(!this->board->getTile(r2,c2)->empty()){
			this->board->taken = this->board->getTile(r2,c2)->p->FEN;
		}
		if(p->is(KING) && abs(c-c2) ==2){
			if(p->side==WHITE){
				if(c-c2==2 && (this->board->wCastle & 1)){
					this->board->wCastle = FALSE;
					Move::castle(this->board,QUEEN_SIDE,side);
					return QUEEN_CASTLE;//f
				}
				if(c2-c==2 && ((this->board->wCastle >> 1) & 1)){
					this->board->wCastle = FALSE;
					Move::castle(this->board,KING_SIDE,side);
					return KING_CASTLE;
				}
				return FALSE;
			}else{
				if(c-c2==2 && (this->board->bCastle & 1)){
					this->board->bCastle = FALSE;
					Move::castle(this->board,QUEEN_SIDE,side);
					return QUEEN_CASTLE;
				}
				if(c2-c==2 && ((this->board->bCastle >> 1) & 1)){
					this->board->bCastle = FALSE;
					Move::castle(this->board,KING_SIDE,side);
					return KING_CASTLE;
				}				
				return FALSE;
			}
		}
		this->board->forceMove(r,c,r2,c2);
		this->board->special = "false";
		if(p->is(PAWN) && abs(r-r2)==2){
			cout<<"possible special pawn move\n";
			if(!this->board->empty(r2,c2+1) && this->board->getTile(r2,c2+1)->p->is(PAWN) && this->board->getTile(r2,c2+1)->p->side != p->side){
				this->board->special = itoa(r2) + itoa(c2+1) + "r";
			}
			if(!this->board->empty(r2,c2-1) && this->board->getTile(r2,c2-1)->p->is(PAWN) && this->board->getTile(r2,c2-1)->p->side != p->side){
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
			if(p->side == WHITE){
				this->board->wCastle = false;
			}else{
				this->board->bCastle = false;
			}
		}
		if((p->FEN == WHITE_PAWN && r2==8) || (p->FEN==BLACK_PAWN && r2==1)){
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
	int r = loc->row;
	int c = loc->col;
	delete loc;
	int i,j;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side != side && !t->p->is(KING) && Move::valid(t->p,i,j,r,c,b)){
				return true;
			}
	}
	return false;
}

bool Game::isProtected(Board* b,int r,int c,char side){
	Tile* t;
	char FEN; 
	for(int i = 1;i<9;i++) 
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side == side && !(i == r && j == c)){
					FEN = b->getTile(i,j)->p->FEN;
					b->forceChange(r,c,EMPTY_SPACE);
					if(Move::valid(t->p,i,j,r,c,b)){
						b->forceChange(r,c,FEN);
						return true;
					}

					b->forceChange(r,c,FEN);
			}
	}
	return false;
}

bool Game::isProtected(Board* b,Location* loc,char side){
	return Game::isProtected(b,loc->row,loc->col,side);
}


bool Game::tileCovered(Board* b,int r,int c,char side){
	Tile* t;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side == side && !t->p->is(KING) && Move::valid(t->p,i,j,r,c,b)){
				printf("The tile (%d,%d) is covered by %c at (%d,%d)\n",r,c,t->p->FEN,i,j);
				return true;
			}
		}
	return false;
}

bool Game::tileCovered(Board* b,Location* loc,char side){
	return Game::tileCovered(b,loc->row,loc->col,side);
}
