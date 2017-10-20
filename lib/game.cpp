#include "game.h"

using namespace std;

bool isDraw(){
	int wPieces = 0;
	int bPieces = 0;
	Tile* t;
	Piece* king;
	Vector moves;
	int numMoves; 
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->board->getTile(b,i,j);
			if(t->empty(t)){
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
		moves = getAdjacent(king->loc);
		numMoves = vector_length(moves);
		for(i = 0;i<numMoves;i++){
			if(validMove(king,(Location)vector_get(moves,i),b)){
				return false;
			}
		}
		return true;
	}
	if(bPieces==1 && !inCheck(b,BLACK)){
		king = getKing(b,BLACK);
		moves = getAdjacent(king->loc);
		numMoves = vector_length(moves);
		for(i = 0;i<numMoves;i++){
			if(validMove(king,(Location)vector_get(moves,i),b)){
				return false;
			}
		}
		return true;
	}
	return false;
}



bool isCheckmate(char side){
	if(!this->inCheck(side))
		return false;
	
	Piece* king = this->board->getKing(side);
	vector<Location*>* moves = king->loc->getAdjacent();
	vector<Piece*> pieces;
	int i = 0;
	int j = 0;
	int l1 = 0;
	int l2 = 0;
	Location* block = nullptr;
	Location* s[2];
	Tile* t;
	int numMoves = moves->size();
	for(i = 0;i<numMoves;i++){
		if(Move::validMove(king,moves->at(i),b)){
			return false;
		}
	}
	delete moves;
	for(i = 1;i<9;i++)
		for(j = 1;j<9;j++){
			t = getTile(b,i,j);
			if(!tileIsEmpty(t) && t->p->side != side && validMove(t->p,king->loc,b)){
				pieces.push_back(t->p);
			}
	}
	if(pieces.size() == 2){
		if(pieces.at(0)->is(KNIGHT) || pieces.at(1)->is(KNIGHT)){
			return true;
		}
		vector<Location*>* v1 = locationsBetween(king->loc,pieces.at(0)->loc);
		vector<Location*>* v2 = locationsBetween(king->loc,pieces.at(1)->loc);
		l1 = v1->size();
		l2 = v2->size();
		
		if(l1 == 0 || l2 ==0){
			return true;
		}
		for(i = 0;i<l1;i++){
			s[0] = v1->at(i);
			for(j = 0;j<l2;j++){
				s[1] = v2.at(j);
				if(s[0].equals(s[1])){
					//block = s[1];
					return true;
				}
			}
		}
		if(Game::tileCovered(b,block,side)){
			return FALSE;
		}else{
			return TRUE;
		}
	}else{
		Vector blockSpots = NULL;
		Piece* p = vector_get(pieces,0);
		blockSpots = locationsBetween(king->loc,p->loc);
		l1 = vector_length(blockSpots);
		for(i = 0;i<l1;i++){
			block = (Location)vector_get(blockSpots,i);
			if(tileCovered(b,block,side)){
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}

uint8_t move(Board b,int r,int c,int r2,int c2,char side){ 
		Piece p = getTile(b,r,c)->p;
		Location move = createLocation(r2,c2);
		char t;
		if(p == NULL||p->side != side){
			return FALSE;
		}
		if(!validMove(p,move,b)){
			printf("IT is not a valid move!\n");
			if(isPiece(p,PAWN) && canSpecial(b,p)){
				t = specialMove(p,move,b);
				if(t == 0){
					return FALSE;
				}
				b->taken = t;
				return TRUE;
			}
			return FALSE;
		}
		if(!tileIsEmpty(getTile(b,r2,c2))){
			b->taken = getTile(b,r2,c2)->p->FEN;
		}
		if(isPiece(p,KING) && abs(c-c2) ==2){
			if(p->side==WHITE){
				if(c-c2==2 && (b->wCastle & 1)){
					b->wCastle = FALSE;
					castle(p,b,QUEEN_SIDE);
					return TRUE;//f
				}
				if(c2-c==2 && ((b->wCastle >> 1) & 1)){
					b->wCastle = FALSE;
					castle(p,b,KING_SIDE);
					return TRUE;
				}
				return FALSE;
			}else{
				if(c-c2==2 && (b->bCastle & 1)){
					b->bCastle = FALSE;
					castle(p,b,QUEEN_SIDE);
					return TRUE;
				}
				if(c2-c==2 && ((b->bCastle >> 1) & 1)){
					b->bCastle = FALSE;
					castle(p,b,KING_SIDE);
					return TRUE;//f
				}				
				return FALSE;
			}
		}
		forceChange(b,r2,c2,getTile(b,r,c)->p->FEN);
		forceChange(b,r,c,EMPTY_SPACE);
		if(isPiece(p,PAWN) && abs(r-r2)==2){
			printf("possible special pawn move\n");
			if(!tileIsEmpty(getTile(b,r2,c2+1)) && isPiece(getTile(b,r2,c2+1)->p,PAWN) && getTile(b,r2,c2+1)->p->side != p->side){
				b->special = stradd(concat(itoa(r2),itoa(c2+1),FALSE),'r');
			}
			if(!tileIsEmpty(getTile(b,r2,c2-1)) && isPiece(getTile(b,r2,c2-1)->p,PAWN) && getTile(b,r2,c2-1)->p->side != p->side){
				if(!strcompare(b->special,"false")){
					b->special = stradd(concat(itoa(r2),itoa(c2-1),FALSE),'r');
				}else{
					b->special = concat(b->special,strappend('|',stradd(concat(itoa(r2),itoa(c2-1),FALSE),'r')),FIRST);
				}
				
			}
		}
		if(inCheck(b,side)){
			printf("The king is in check\n");
			return FALSE;
		}else{
			printf("The king is not in check\n");
		}
		if(isPiece(p,KING)){
			if(p->side==WHITE){
				b->wCastle = FALSE;
			}else{
				b->bCastle = FALSE;
			}
		}
		if((p->FEN==WHITE_PAWN && r2==8) || (p->FEN==BLACK_PAWN && r2==1)){
			free(getTile(b,r,c));
			forceChange(b,r,c,'X');
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


bool Game::isProtected(Board* b,Location* loc,char side){
	Tile t;
	char FEN; 
	for(int i = 1;i<9;i++) 
		for(int j = 1;j<9;j++){
			t = b->getTile(i,j);
			if(!t->empty() && t->p->side == side && !t->p->loc->equal(loc)){
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
	int i,j;
	for(i = 1;i<9;i++)
		for(j = 1;j<9;j++){
			t = this->getTile(i,j);
			if(!t->empty() && t->p->side == side && t->p->is(t->p,KING) && validMove(t->p,loc,b)){
				return true;
			}
		}
	return false;
}

bool Game::tileCovered(int r,int c,char side){
	Location* loc = new Location(r,c);
	bool out = this->tileCovered(loc,side);
	delete loc;
	return out;
}