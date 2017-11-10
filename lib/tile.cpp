#include "tile.h"
	
using namespace std;
Tile::Tile(int row, int col,char fen){
	if(fen != EMPTY_SPACE){
		this->p = new Piece(new Location(row,col),(fen>96)? BLACK : WHITE,fen);
	}else{
		this->p = nullptr;
	}
}

bool Tile::empty(){
	return (this->p == nullptr); 
}