#include "chess/tile.h"
	
using namespace std;

tile::tile(char fen) {
	if(fen != EMPTY_SPACE){
		this->p = piece((fen>96)? BLACK : WHITE, fen);
	}
	
}

void Tile::change(char fen){
	if(fen != EMPTY_SPACE){
		this->p->change((fen>96)? BLACK : WHITE, fen);
	}else{
		this->p.clear();
	}
}

bool Tile::empty()
{
	return this->p.empty(); 
}

void tile::clear()
{
	this->p.clear();
}