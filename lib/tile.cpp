#include "tile.h"
	
using namespace std;
Tile::Tile(int row, int col,char p){
	if(p != EMPTY_SPACE){
		this->p = new Piece(new Location(row,col),(p>96)? BLACK : WHITE,p);
	}else{
		this->p = nullptr;
	}
}

bool Tile::empty(){
	return this->p == nullptr; 
}