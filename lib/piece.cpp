#include "piece.h"

using namespace std;


Piece::Piece(Location* loc,char side,char FEN){
	this->loc = loc;
	this->side = side;
	this->FEN = FEN;
	this->name = Piece::getName(FEN);
	if((this->name & PROMOTED_PAWN) != 0){
		this->promo = true;
		this->name -= PROMOTED_PAWN;
	}else{
		this->promo = false;
	}
	this->special = false;
}

bool Piece::is(int name){
	return this->name == name;
}

bool Piece::canSpecial(){
	return this->special;
}

int Piece::getName(char FEN){
	int out = 0;
	switch(FEN){
		case BLACK_PAWN:
		case WHITE_PAWN:
			return PAWN;
		case PROMOTED_BLACK_KNIGHT:
		case PROMOTED_WHITE_KNIGHT:
			out = PROMOTED_PAWN;
		case BLACK_KNIGHT:
		case WHITE_KNIGHT:
			return out | KNIGHT;
		case PROMOTED_BLACK_BISHOP:
		case PROMOTED_WHITE_BISHOP:
			out = PROMOTED_PAWN;
		case BLACK_BISHOP:
		case WHITE_BISHOP:
			return out | BISHOP;
		case PROMOTED_BLACK_ROOK:
		case PROMOTED_WHITE_ROOK:
			out = PROMOTED_PAWN;
		case BLACK_ROOK:
		case WHITE_ROOK:
			return out | ROOK;
		case PROMOTED_BLACK_QUEEN:
		case PROMOTED_WHITE_QUEEN:
			out = PROMOTED_PAWN;
		case BLACK_QUEEN:
		case WHITE_QUEEN:
			return out | QUEEN;
		case BLACK_KING:
		case WHITE_KING:
			return KING;	
	}
	return out;
}


void Piece::setSpecial(int side){
	if(!this->is(PAWN)){
		cout<<"Tried to set a special move for a piece that was not a pawn\n";
		return;
	}
	this->special = side;
}

void Piece::move(Location* loc){
	delete this->loc;
	this->loc = loc;
}