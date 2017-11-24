#include "chess/board.h"

using namespace std;

Board::Board(string FEN, string spec, string castle){
	this->tiles = (Tile***)calloc(sizeof(Tile**),8);
	string f = FEN;
	char c;
	//cout<<"Processing the Castle data...";
	this->bCastle = ((castle.find("bk")==string::npos)? 0 : KING_SIDE) | ((castle.find("bq")==string::npos)? 0 : QUEEN_SIDE);
	this->wCastle = ((castle.find("wk")==string::npos)? 0 : KING_SIDE) | ((castle.find("wq")==string::npos)? 0 : QUEEN_SIDE);
	//cout<<"done, WHITE - "<<(int)this->wCastle<<" : BLACK - "<<(int)this->bCastle<<endl;
	this->taken = 'X';
	//cout<<"Creating the tiles\n";
	for(int i = 0;i<8;i++){
		this->tiles[i] = (Tile**)calloc(sizeof(Tile*),8);
		for(int j = 0;j<8;j++){
			c = getLast(f);
			f.pop_back();
			if(c == '/'){
				c = getLast(f);
				f.pop_back();
			}
			this->tiles[i][j] = new Tile(c);
		}	
	}
	//cout<<"Created tiles\n";
	if(spec.size()>1){
		this->specialData(spec);
	}
	this->special = spec;
	//cout<<"Processed the special data\n";
	cout<<"Given: "<<FEN<<endl;
	cout<<"Produced: "<<this->to_string()<<endl;
	
}

Board::~Board(){
	for(int i = 0;i<8;i++){
		for(int j = 0;j<8;j++){
			delete this->tiles[i][j];
		}
		free(this->tiles[i]);
	}
	free(this->tiles);
}

Tile* Board::getTile(int row, int col){
	if((uint32_t)row>8 || (uint32_t)col>8 ){
		cerr<<"Board::getTile:Error:Tried to get a tile which is out of bounds"<<endl;
		return nullptr;
	}
	return this->tiles[row-1][col-1];
}

bool Board::empty(int row,int col){
	if(row == 0 || col == 0){
		return true;
	}
	Tile* t = this->getTile(row,col);
	if(t== nullptr){
		return true;
	}
	return t->empty();
}

void Board::specialData(string data){
	cout<<"Getting the data from the string\n";
	int r = data[0] - 48;
	int c = data[1] - 48;
	cout<<"POS is ("<<r<<","<<c<<") data is "<<data<<endl;
	char side = data[2];
	cout<<"Checking to see if tile is valid\n";
	if(this->getTile(r,c)==nullptr || this->getTile(r,c)->empty()){
		return;
	}
	cout<<"The tile is not empty and not NULL\n";
	if(! this->getTile(r,c)->p->is(PAWN)){
		return;
	}
	cout<<"Setting the special bit\n";
	if(side =='r'){
		this->getTile(r,c)->p->setSpecial(RIGHT);
	}else{
		this->getTile(r,c)->p->setSpecial(LEFT);
	}
}

void Board::forceChange(int r,int c,char FEN){ 
	delete this->tiles[r-1][c-1];
	this->tiles[r-1][c-1] = new Tile(FEN);
	//this->tiles[r-1][c-1]->change(FEN);
}

void Board::forceMove(int r,int c,int r2, int c2){
	this->tiles[r2-1][c2-1]->p = this->tiles[r-1][c-1]->p;
	this->tiles[r-1][c-1]->p = nullptr;
}

bool Board::placePiece(int r,int c,char FEN){
	if(!this->getTile(r,c)->empty() || Piece::getName(FEN) == 0){
		return false;
	}
	this->forceChange(r,c,FEN);
	return true;
}

Location* Board::findKing(char side){
	Tile* t;
	for(int i = 1;i<9;i++)
		for(int j = 1;j<9;j++){
			t = this->getTile(i,j);
			if(!t->empty() && t->p->is(KING) && t->p->side == side){
				return new Location(i,j);
			}	
	}
	cerr<<"ERROR: Unable to the find the king"<<endl;
	return nullptr;
}

Piece* Board::getKing(char side){
	Location* loc = this->findKing(side);
	Piece* out = this->getTile(loc->row,loc->col)->p;
	delete loc;
	return out;
}

char Board::otherSide(char side){
	return (side == WHITE)? BLACK : WHITE;
}

string Board::generateFEN(){
	string output = "";
	Tile* t;
	for(int i = 8;i>0;i--){ 
		for(int j = 8;j>0;j--){
			t = this->getTile(i,j);
			if(t == nullptr){
				cerr<<"Tile is null"<<endl;
				return "ERROR";
			}
			if(t->empty()){
				output += EMPTY_SPACE;
			}else{
				output += t->p->FEN;			
			}
		}	
		if(i>1)
			output = output + "/";
	}
	return output;	
}

string Board::getCastleData(){
	string out = (this->bCastle & KING_SIDE) ? "bk" : "";
	out += (this->bCastle & QUEEN_SIDE)? "bq" : "";
	out += (this->wCastle & KING_SIDE)? "wk" : "";
	out += (this->wCastle & QUEEN_SIDE)? "wq" : "";
	return out;
}

string Board::to_string(){
	string out = this->generateFEN();
	out += ";";
	out += this->getCastleData();
	out += ";";
	out += this->special;
	return out;
}

char Board::numToCol(int c){
	return (char)(c + 96);
}