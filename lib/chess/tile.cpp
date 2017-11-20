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

bool Tile::empty(){
	return (this->p == nullptr); 
}