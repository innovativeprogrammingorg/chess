#include "chess/piece.h"

using namespace std;

piece::~piece() : side(0),FEN(EMPTY_SPACE),name(0),promo(false),special(false){}

piece::piece(char side,char FEN): side(side),FEN(FEN),name(piece::get_name(FEN)),promo(false),special(false)
{
	if((this->name & PROMOTED_PAWN) != 0){
		this->promo = true;
		this->name ^= PROMOTED_PAWN;
	}
}


void piece::change(char side,char FEN){
	this->side = side;
	this->FEN = FEN;
	this->name = piece::get_name(FEN);
	if((this->name & PROMOTED_PAWN) != 0){
		this->promo = true;
		this->name -= PROMOTED_PAWN;
	}else{
		this->promo = false;
	}
	this->special = false;
}

bool piece::is(int name){
	return this->name == name;
}



bool piece::can_special(){
	return this->special;
}

void piece::set_special(int side){
	if(!this->is(PAWN)){
		cout<<"Tried to set a special move for a piece that was not a pawn\n";
		return;
	}
	this->special = side;
}

bool piece::empty()
{
	return this->name == 0;
}

void piece::clear()
{
	this->side = 0;
	this->FEN = EMPTY_SPACE;
	this->name = 0;
	this->promo = false;
	this->special = false;
}

int piece::get_name(char FEN){
	
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
	return 0;
}