#include "chess/tile.h"
	
using namespace std;

Tile::Tile(char fen){
	if(fen != EMPTY_SPACE){
		this->p = new Piece((fen>96)? BLACK : WHITE, fen);
	}else{
		this->p = nullptr;
	}
}

Tile::~Tile(){
	if(this->p != nullptr){
		delete this->p;
	}
}

void Tile::change(char fen){
	if(this->p != nullptr && fen != EMPTY_SPACE){
		this->p->change((fen>96)? BLACK : WHITE, fen);
	}else if(this->p != nullptr){
		delete this->p;
	}
	if(fen != EMPTY_SPACE){
		this->p = new Piece((fen>96)? BLACK : WHITE, fen);
	}else{
		this->p = nullptr;
	}
}

bool Tile::empty(){
	return (this->p == nullptr); 
}