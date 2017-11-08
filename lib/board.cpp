#include "board.h"
#include "move.h"
//const char columns[8] = {'a','b','c','d','e','f','g','h'};

using namespace std;

Board::Board(string FEN, string spec, string castle){
	this->tiles = (Tile***)calloc(sizeof(Tile**),8);
	string f = FEN;
	char c;
	cout<<"Processing the Castle data...";
	this->bCastle = ((castle.find("bk")==string::npos)? 0 : KING_SIDE) | ((castle.find("bq")==string::npos)? 0 : QUEEN_SIDE);
	this->wCastle = ((castle.find("wk")==string::npos)? 0 : KING_SIDE) | ((castle.find("wq")==string::npos)? 0 : QUEEN_SIDE);
	cout<<"done, WHITE - "<<this->wCastle<<" : BLACK - "<<this->bCastle<<endl;
	this->taken = -1;
	cout<<"Creating the tiles\n";
	for(int i = 0;i<8;i++){
		this->tiles[i] = (Tile**)calloc(sizeof(Tile*),8);
		for(int j = 0;j<8;j++){
			c = getLast(f);
			f = f.substr(0,f.size()-2);
			if(c=='/'){
				c = getLast(f);
				f = f.substr(0,f.size()-2);
			}
			this->tiles[i][j] = new Tile(i+1,j+1,c);
		}	
	}
	cout<<"Created tiles\n";
	if(spec.size()>1){
		this->specialData(spec);
	}
	this->special = spec;
	cout<<"Processed the special data\n";
	
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
	return this->tiles[row-1][col-1];
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
	this->tiles[r-1][c-1] = new Tile(r,c,FEN);
}

bool Board::placePiece(int r,int c,char FEN){
	if(!this->getTile(r,c)->empty() || Piece::getName(FEN)==0){
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
				return t->p->loc;
			}	
	}
	cerr<<"ERROR: Unable to the find the king\n";
	return nullptr;
}

Piece* Board::getKing(char side){
	Location* loc = this->findKing(side);
	return this->getTile(loc->row,loc->col)->p;
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
	string out = ((this->bCastle >> 1)&1)?"bk":"";
	out += (this->bCastle & 1)?"bq":"";
	out += ((this->wCastle >> 1)&1)?"wk":"";
	out += (this->wCastle & 1)?"wq":"";
	return out;
}

string Board::getBoardData(){
	JSON* data = new JSON("string");
	string history = "";
	data->add("FEN",this->generateFEN());
	data->add("Castle",this->getCastleData());
	data->add("Taken",new string((char*)&(this->taken)));
	data->add("Special",this->special);
	string out = data->to_string();
	delete data;
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




